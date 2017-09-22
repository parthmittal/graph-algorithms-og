#include "graph.hpp"
#include <stack>

#ifndef __SSSP_hpp__
#define __SSSP_hpp__

typedef long long ll;

void
sssp(int source, const w_undirected_graph_t &G, std::vector<int> &S,
		std::vector<int> &dist, std::vector<ll> &num_paths,
		std::vector< std::vector<int> > &P);

#endif // __SSSP_hpp__
