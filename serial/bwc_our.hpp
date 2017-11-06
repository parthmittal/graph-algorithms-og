#include <vector>
#include "graph.hpp"
#include "ear-decomposition-dfs.hpp"

#ifndef __BWC_OUR_hpp__
#define __BWC_OUR_hpp__

#ifdef __OUR__

class bwc_our {
public:
	undirected_graph_t &G;
	std::vector<int> id;
	std::vector<int> rid;
	two_connected_prop two;
	reduced_graph_t Gr;
	std::vector<int> leftV, rightV;
	std::vector<int> distL, distR;

	std::vector<double> bwc;

	void add_vertex_reduced_graph(int v);
	void compute_reduced_graph();
	void forward_phase_reduced_graph();
	void sim_forward_phase(int u);
	void sim_brandes1(int u, const rgraph_vinfo &Lrv, const rgraph_vinfo &Rrv);
	rgraph_vinfo get_node_info(int u);
	void sim_brandes_all();

	bwc_our(undirected_graph_t &G)
		:G(G), id(G.N, -1), rid(G.N), two(G), Gr(), leftV(G.N), rightV(G.N),
		distL(G.N), distR(G.N), bwc(G.N)
	{
		compute_reduced_graph();
	}

};

#endif // __OUR__

#endif // __BWC_OUR_hpp__
