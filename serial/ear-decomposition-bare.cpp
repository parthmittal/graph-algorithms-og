#include <vector>
#include <cassert>
#include <iostream>
#include <chrono>
#include <stack>

struct vertex_t {
	int id;

	bool operator==(const vertex_t &other) const
	{
		return id == other.id;
	}

	bool operator!=(const vertex_t &other) const
	{
		return id != other.id;
	}
};

struct edge_t {
	vertex_t u, v;
};

typedef std::vector<edge_t> path_t;

struct undirected_graph_t {
	undirected_graph_t (int N)
	{
		this -> N = N;
		adj_list.resize(N);
	}

	void add_edge (vertex_t u, vertex_t v)
	{
		adj_list[u.id].push_back(v);
		adj_list[v.id].push_back(u);
	}
	
	int N;
	std::vector< std::vector< vertex_t > > adj_list;
};

/* it was rather ugly to keep passing around parameters,
 * and global variables are even uglier,
 * so I am using a wrapper class for this.
 */
class two_connected_prop {
public:
	undirected_graph_t G;
	std::vector<int> visited;
	std::vector<vertex_t> parent;
	std::vector< std::vector<vertex_t> > back_edges;
	std::vector<int> entry_time, exit_time;
	std::vector<vertex_t> vert;

	std::vector<int> visited_ear_decomposition;
	std::vector< std::vector<path_t> > ear_decomposition; //each element is at least 2-edge-connected.
	/* we need to know the 2-connected-component of
	 * the parent of arbitrary vertices.
	 * This ugly type is the easiest solution I could work out
	 */
	typedef std::vector< std::vector<path_t> >::iterator ear_it_t;
	std::vector<ear_it_t> ears_of;

	two_connected_prop(const undirected_graph_t &G)
		:G(G)
	{
		visited.resize(G.N);
		parent.resize(G.N);
		back_edges.resize(G.N);
		entry_time.resize(G.N);
		exit_time.resize(G.N);
		vert.resize(G.N + 1); //entry times range from 1 to G.N
		visited_ear_decomposition.resize(G.N);
		ears_of.resize(G.N);
	}

	struct dfs_stack_elem_t {
		vertex_t u;
		std::vector<vertex_t>::iterator it;
	};

	void
	dfs(vertex_t root, int &current_time)
	{
		std::stack<dfs_stack_elem_t> dfs_stack;
		dfs_stack.push({root, G.adj_list[root.id].begin()});
		visited[root.id] = 1;
		entry_time[root.id] = ++current_time;
		vert[current_time] = root;

		while(!dfs_stack.empty()) {
			vertex_t u = dfs_stack.top().u;
			std::vector<vertex_t>::iterator &it = dfs_stack.top().it;
			if (it == G.adj_list[u.id].end()) {
				dfs_stack.pop();
				exit_time[u.id] = current_time;
			} else {
				vertex_t v = *it;
				if (!visited[v.id]) {
					visited[v.id] = 1;
					parent[v.id] = u;
					entry_time[v.id] = ++current_time;
					vert[current_time] = v;
					dfs_stack.push({v, G.adj_list[v.id].begin()});
				} else if (v != parent[u.id]) {
					if (entry_time[v.id] >= entry_time[u.id]) {
						back_edges[u.id].push_back(v);
					}
				}
				++it;
			}
		}
	}

	ear_it_t
	find_ears(vertex_t u, ear_it_t parent_ears)
	{
		ear_it_t curr_ears;
		if (!visited_ear_decomposition[u.id]) {
			ear_decomposition.push_back({});
			curr_ears = ear_decomposition.end() - 1;
		} else {
			curr_ears = parent_ears;
		}

		for (vertex_t &v : back_edges[u.id]) {
			visited_ear_decomposition[u.id] = 1;
			path_t ear;
			vertex_t curr = v, prev = u;
			while(!visited_ear_decomposition[curr.id]) { //break on first vertex already visited.
				ear.push_back({prev, curr});
				visited_ear_decomposition[curr.id] = 1;
				prev = curr;
				curr = parent[curr.id];
			}
			//still have to push edge we used to get to first already
			//visited vertex to ear.
			ear.push_back({prev, curr});
			curr_ears->push_back(ear);
		}

		return curr_ears;
	}

	void
	ear_decompose()
	{
		int total_back_edges = 0;
		int current_time = 0;
		for (int i = 0; i < G.N; ++i) {
			if (!visited[i]) {
				dfs({i}, current_time);
			}
			total_back_edges += back_edges[i].size();
		}
		ear_decomposition.reserve(total_back_edges + 5); 
		//		std::cerr << G.N << ' ' << current_time << std::endl;
		assert(current_time == G.N);

		ear_decomposition.push_back({});
		ear_decomposition.end() - 1;
		vert[1];
		ears_of[vert[1].id] = find_ears(vert[1], ear_decomposition.end() - 1);
		for (int i = 2; i <= current_time; ++i) {
			int u = vert[i].id;
			int p = parent[u].id;
			ears_of[u] = find_ears({u}, ears_of[p]);
		}

		ear_decomposition.erase(ear_decomposition.begin());
	}
};

int main()
{
	using namespace std;
	ios_base::sync_with_stdio(false);
	//	freopen("ear.in", "r", stdin);
	//	freopen("ear.out", "w", stdout);

	int N, M;
	scanf("%d%d", &N, &M);
	undirected_graph_t g(N);
	for (int i = 0; i < M; ++i) {
		int u, v;
		scanf("%d%d", &u, &v);
		g.add_edge({u - 1}, {v - 1});
	}

	auto start = chrono::steady_clock::now();

	two_connected_prop T(g);
	T.ear_decompose();

	/*
	 * at this point, we are done with computing the ear-decomposition,
	 * we will just spend the rest of the time pretty printing
	 */

	auto finish = chrono::steady_clock::now();
	auto int_ms = chrono::duration_cast<chrono::milliseconds>(finish - start);

	cerr << "computed ear decomposition in " 
		<< int_ms.count() << " milliseconds" << endl;

	auto vec = T.ear_decomposition;
	/* code used to print output in format for asc24b
	 * problem B over here(http://codeforces.com/gym/100340)
	 * this is useful to test any changes we make to algorithm,
	 * so I'm leaving it on here.
	 */
	//	if (vec.size() > 2) {
	//		cout << -1 << endl;
	//		return 0;
	//	}
	//	cout << vec[1].size() << endl;
	//	for (auto it = vec[1].rbegin(); it != vec[1].rend(); ++it) {
	//		auto p = *it;
	//		cout << p.size() << ' ';
	//		for (auto &e : p) {
	//			cout << e.u.id + 1 << ' ';
	//		}
	//		cout << p.rbegin()->v.id + 1 << '\n';
	//	}

	int conn_id = 1;
	for (auto &two_conn : vec) {
		printf("component %d\n", conn_id);
		int ear_id = 1;
		for (auto &ear : two_conn) {
			printf("ear %d\n", ear_id);
			for (auto &e : ear) {
				printf("\t%d -> %d\n", e.u.id, e.v.id);
			}
			++ear_id;
		}
		printf("\n");
		++conn_id;
	}
}
