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
	std::vector<int> rid;
	two_connected_prop two;
	w_undirected_graph_t Gr;
	std::vector<int> leftV, rightV;
	std::vector<int> distL, distR;

	std::vector< std::vector<int> >        dist;
	std::vector< std::vector<long long > > num_paths;
	std::vector< std::vector<int> >        inorder;

	std::vector< std::vector< std::vector<int> > > parents;

	void compute_reduced_graph();
	void forward_phase_reduced_graph();
	void sim_forward_phase(int u);

	bwc_our(const undirected_graph_t &G)
		:G(G), id(G.N, -1), rid(G.N), two(G), Gr(), leftV(G.N), rightV(G.N),
		distL(G.N), distR(G.N)
	{
		compute_reduced_graph();
	}

};

#endif // __OUR__

#endif // __BWC_OUR_hpp__
