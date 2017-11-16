#include "graph.hpp"

#ifndef __BWC_BRANDES_hpp__
#define __BWC_BRANDES_hpp__

void
brandes1(const undirected_graph_t &G, int source, std::vector<double> &BC);

std::vector<double>
compute_bwc(const undirected_graph_t &G);

#endif // __BWC_BRANDES_hpp__
