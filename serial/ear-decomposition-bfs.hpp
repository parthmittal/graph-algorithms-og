#include <vector>
#include "graph.hpp"

#ifndef __EAR_DECOMPOSITION_BFS_hpp__
#define __EAR_DECOMPOSITION_BFS_hpp__

#ifndef __USE_DFS_TREE__
class two_connected_prop {
	const int inf = __INT32_MAX__ >> 1;

public:
	undirected_graph_t G;
	std::vector<int> dist;
	std::vector<vertex_t> parent;
	std::vector< std::vector<int> > tree_edges;
	std::vector< std::vector<int> > non_tree_edges;
	std::vector<std::vector<std::pair<int, int>>> edges_by_lca_dist;
	std::vector< std::vector<std::pair<int, int>> > ears;

	two_connected_prop(const undirected_graph_t &G);

	void
	bfs(vertex_t root);

	void ear_decompose();
};
#endif // not __USE_DFS_TREE__

#endif // __EAR_DECOMPOSITION_BFS_hpp__
