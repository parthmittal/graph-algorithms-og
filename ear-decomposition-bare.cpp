#include <vector>
#include <cassert>
#include <iostream>

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


void
dfs(
	const undirected_graph_t &G,
	std::vector<int> &visited,
	std::vector<int> &entry_time,
	std::vector<int> &exit_time,
	std::vector< std::vector<vertex_t> > &back_edges,
	std::vector<vertex_t> &parent,
	std::vector<vertex_t> &vert, //reverse map of discovery time to vertices.
	vertex_t root,
	int &current_time)
{
	visited[root.id] = 1;
	entry_time[root.id] = current_time;
	vert[current_time] = root;

	for (const vertex_t &v : G.adj_list[root.id]) {
		if (!visited[v.id]) {
			parent[v.id] = root;
			dfs(G, visited, entry_time, exit_time, back_edges, parent, vert, v, ++current_time);
		} else {
			//we want to add all the non-tree edges, in direction from ancestor to descendant.
			if (v != parent[root.id] && entry_time[v.id] >= entry_time[root.id]) {
				back_edges[root.id].push_back(v);
			}
		}
	}

	exit_time[root.id] = current_time;
}

typedef std::vector<edge_t> path_t;

std::vector<path_t>
ear_decomposition(undirected_graph_t G)
{
	using std::vector;

	vector<int> visited(G.N), entry_time(G.N), exit_time(G.N);
	vector< vector<vertex_t> > back_edges(G.N);
	vector<vertex_t> parent(G.N), vert(G.N + 1);

	int current_time = 1;
	dfs(G, visited, entry_time, exit_time, back_edges, parent, vert, {0}, current_time);

	if (current_time < G.N) {
		/* current_time is equal to number of vertices discovered by dfs.
		 * if this is less than N, then graph is disconnected.
		 * TODO: change return type of function/throw exception?
		 */
		return {};
	} else {
		assert(current_time == G.N);
		fill(visited.begin(), visited.end(), 0);
		vector<path_t> ret;
		for (int i = 1; i <= G.N; ++i) {
			vertex_t u = vert[i];
			if (!back_edges[u.id].empty()) {
				visited[u.id] = 1;
			}
			for (vertex_t &v : back_edges[u.id]) {
				path_t ear;
				vertex_t curr = v, prev = u;
				while(!visited[curr.id]) { //break on first vertex already visited.
					ear.push_back({prev, curr});
					visited[curr.id] = 1;
					prev = curr;
					curr = parent[curr.id];
				}
				//still have to push edge we used to get to first already
				//visited vertex to ear.
				ear.push_back({prev, curr});
				ret.push_back(ear);
			}
		}

		for (int i = 0; i < G.N; ++i) {
			if (!visited[i]) {
				return {};
			}
		}

		return ret;
	}
}

int main()
{
	using namespace std;
	ios_base::sync_with_stdio(false);
	freopen("ear.in", "r", stdin);
	freopen("ear.out", "w", stdout);

	int N, M;
	cin >> N >> M;
	undirected_graph_t g(N);
	for (int i = 0; i < M; ++i) {
		int u, v;
		cin >> u >> v;
		g.add_edge({u - 1}, {v - 1});
	}

	auto vec = ear_decomposition(g);
	/* code used to print output in format for asc24b
	 * problem B over here(http://codeforces.com/gym/100340)
	 * this is useful to test any changes we make to algorithm,
	 * so I'm leaving it on here.
	 */
//	if (vec.empty()) {
//		cout << -1 << endl;
//		return 0;
//	}
//	cout << vec.size() << endl;
//	for (auto it = vec.rbegin(); it != vec.rend(); ++it) {
//		auto p = *it;
//		cout << p.size() << ' ';
//		for (auto &e : p) {
//			cout << e.u.id + 1 << ' ';
//		}
//		cout << p.rbegin()->v.id + 1 << '\n';
//	}

	for (auto &p : vec) {
		cout << p.size() << endl;
		for (auto &e : p) {
			cout << '\t' << e.u.id << " -> " << e.v.id << endl;
		}
	}
}
