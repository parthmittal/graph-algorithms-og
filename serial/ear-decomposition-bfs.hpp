#include <vector>
#include "graph.hpp"

#ifndef __EAR_DECOMPOSITION_BFS_hpp__
#define __EAR_DECOMPOSITION_BFS_hpp__

class two_connected_prop {
	const int inf = INT_MAX >> 1;

	void
	__build_tree_info_1(vertex_t root, int &poid);

	void
	__build_tree_info_2(vertex_t root);

public:
	undirected_graph_t G;
	std::vector<int> dist;
	std::vector<vertex_t> parent;
	std::vector<int> low;
	std::vector<int> high, __high;
	std::vector<int> id; //pre-order index
	std::vector<int> nd; //number of descendents
	std::vector<int> mark;
	std::vector< std::vector<int> > tree_edges;
	std::vector< std::vector<int> > non_tree_edges;

	two_connected_prop(const undirected_graph_t &G);

	void
	bfs(vertex_t root);

	/* finds pre-order indices, low-points,
	 * high-points, and number of descendents */
	void
	build_tree_info(vertex_t root, int &poid);

	void ear_decompose(vertex_t root);
};

#endif // __EAR_DECOMPOSITION_BFS_hpp__
