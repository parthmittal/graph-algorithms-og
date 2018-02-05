#include "graph.hpp"
#include <stack>
#include <chrono>

#ifndef __SSSP_hpp__
#define __SSSP_hpp__

typedef long long ll;

/* counts total time spent inside sssp function */
extern long long sssp_profile;

void
sssp(int source, const reduced_graph_t &G, std::vector<int> &S,
		std::vector<int> &dist, std::vector<ll> &num_paths);

void
sssp_khops(int source, int u, std::vector<int> &dist, reduced_graph_t &G,
		std::stack<int> &reset, int k);

#endif // __SSSP_hpp__
