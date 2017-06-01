#include <queue>
#include <algorithm>
#include <vector>
#include <iostream>

#include "ear-decomposition-bfs.hpp"
#include "lca.hpp"

#ifndef __USE_DFS_TREE__

two_connected_prop::two_connected_prop(const undirected_graph_t &G)
	:G(G), dist(G.N, inf), parent(G.N), tree_edges(G.N),
	non_tree_edges(G.N), edges_by_lca_dist(G.N)
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
two_connected_prop::ear_decompose()
{
	using std::vector;
	using std::pair;

	#ifdef __TARJAN_LCA__
	lca_wrapper L(G.N);
	for (int i = 0; i < G.N; ++i) {
		if (dist[i] == inf) {
			bfs({i});
			L.lca(i, non_tree_edges, tree_edges, dist, edges_by_lca_dist);
		}
	}
	#elif defined(__DP_LCA__)
	lca_wrapper L;
	for (int i = 0; i < G.N; ++i) {
		if (dist[i] == inf) {
			bfs({i});
		}
	}
	L.lca(non_tree_edges, parent, dist, edges_by_lca_dist);
	#endif // __TARJAN_LCA__

	vector<int> done(G.N); /*indicates whether tree-edge is used*/
	for (int d = 0; d < G.N; ++d) {
		for (pair<int, int> e : edges_by_lca_dist[d]) {
			ears.push_back({e});
			int u = e.first, v = e.second;
			while(dist[u] > d && !done[u]) {
				done[u] = 1;
				ears.back().push_back({u, parent[u].id});
				u = parent[u].id;
			}
			while(dist[v] > d && !done[v]) {
				done[v] = 1;
				ears.back().push_back({v, parent[v].id});
				v = parent[v].id;
			}
		}
	}
}

#endif // not __USE_DFS_TREE__
