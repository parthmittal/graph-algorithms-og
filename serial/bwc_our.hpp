#include <vector>
#include "graph.hpp"
#include "ear-decomposition-dfs.hpp"

#ifndef __BWC_OUR_hpp__
#define __BWC_OUR_hpp__

#ifdef __OUR__

class bwc_our {
public:
	const undirected_graph_t &G;
	std::vector<int> id;
	two_connected_prop two;
	w_undirected_graph_t Gr;

	void compute_reduced_graph();

	bwc_our(const undirected_graph_t &G)
		:G(G), id(G.N, -1), two(G), Gr()
	{
		compute_reduced_graph();
	}
};

#endif // __OUR__

#endif // __BWC_OUR_hpp__
