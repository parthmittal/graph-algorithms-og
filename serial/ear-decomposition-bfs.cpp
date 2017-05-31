#include <queue>
#include <algorithm>
#include "ear-decomposition-bfs.hpp"
#include "lca.hpp"

two_connected_prop::two_connected_prop(const undirected_graph_t &G)
	:G(G), dist(G.N, inf), parent(G.N), low(G.N), high(G.N),
	__high(G.N), id(G.N), nd(G.N), mark(G.N), tree_edges(G.N),
	non_tree_edges(G.N)
{}

void
two_connected_prop::bfs(vertex_t root)
{
	std::queue<vertex_t> bfq;
	bfq.push(root);
	dist[root.id] = 0;

	while(!bfq.empty()) {
		int u = bfq.front().id;
		bfq.pop();
		for (auto &v : G.adj_list[u]) {
			if (dist[v.id] == inf) {
				dist[v.id] = dist[u] + 1;
				parent[v.id] = {u};
				bfq.push(v);
				tree_edges[u].push_back(v.id);
			} else if (v != parent[u]) {
				non_tree_edges[u].push_back(v.id);
			}
		}
	}
}

void
two_connected_prop::build_tree_info(vertex_t root, int &poid)
{
	__build_tree_info_1(root, poid);
	__build_tree_info_2(root);
}

/* NB: this implementation is recursive,
 * and the stack size is O(diameter of tree).
 * should work well enough on BFS-trees of real
 * life graphs */
void
two_connected_prop::__build_tree_info_1(vertex_t root, int &poid)
{
	nd[root.id] = 1;
	id[root.id] = poid;
	__high[root.id] = poid;
	for (int &v : tree_edges[root.id]) {
		__build_tree_info_1({v}, ++poid);
		nd[root.id] += nd[v];
		__high[root.id] = std::max(__high[root.id], __high[v]);
	}
}

void
two_connected_prop::__build_tree_info_2(vertex_t root)
{
	low[root.id] = id[root.id];
	high[root.id] = __high[root.id];
	for (int &v : tree_edges[root.id]) {
		__build_tree_info_2({v});
		low[root.id] = std::min(low[root.id], low[v]);
		high[root.id] = std::max(high[root.id], high[v]);
	}
	for (int &v : non_tree_edges[root.id]) {
		low[root.id] = std::min(low[root.id], id[v]);
		high[root.id] = std::max(high[root.id], __high[v]);
	}
	if (low[root.id] < id[parent[root.id].id] ||
			high[root.id] > id[root.id] + nd[root.id]) {
		mark[root.id] = 1;
	}
}

/* NB: this will perform very poorly for a graph with a lot of
 * components. TODO: fix
 */
void
two_connected_prop::ear_decompose(vertex_t root)
{
	using std::vector;
	using std::pair;

	bfs(root);
	lca_wrapper L(G.N);
	auto lca = L.lca(root.id, non_tree_edges, tree_edges);
	vector< vector<pair<int, int>> > non_tree_with_lca(G.N);
	for (int i = 0; i < G.N; ++i) {
		for (int j = 0; j < non_tree_edges[i].size(); ++j) {
			int v = non_tree_edges[i][j];
			int d = dist[lca[i][j]];
			non_tree_with_lca[d].push_back({i, v});
		}
	}
	vector<int> done(G.N); /*indicates whether tree-edge is used*/
	vector<int> ear_number(G.N); 
	int eid = 0;
	for (int d = 0; d < G.N; ++d) {
		for (pair<int, int> e : non_tree_with_lca[d]) {
			int u = e.first, v = e.second;
			while(dist[u] > d && !done[u]) {
				ear_number[u] = eid;
				done[u] = 1;
				u = parent[u].id;
			}
			while(dist[v] > d && !done[v]) {
				ear_number[v] = eid;
				done[v] = 1;
				v = parent[v].id;
			}
			++eid;
		}
	}
}
