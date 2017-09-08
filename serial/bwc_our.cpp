#include "bwc_our.hpp"
#include "ear-decomposition-dfs.hpp"
#include "graph.hpp"

#include <queue>
#include <stack>
#include <iostream>

#include <cassert>

#ifndef __BRANDES__

std::vector<double>
compute_bwc(const undirected_graph_t &G)
{
	using namespace std;
#ifdef __USE_DFS_TREE__
	two_connected_prop two(G);
	two.ear_decompose();

	auto &GED = two.ear_decomposition;
	if (GED.size() > 1) {
		assert(0);
		/*
		 * TODO: this is situation where G is not 2-connected,
		 * deal with this at some point
		 */
	} else {
		vector<int> id(G.N);
		w_undirected_graph_t Gr;

		auto &ED = GED[0];
		for (path_t &ear : ED) {
			assert(ear.size() >= 1);
			int u = ear[0].u.id, v = ear[0].v.id;
			int weight = 1;
			int idx = 1;
			while(idx < ear.size() && G.adj_list[v].size() < 3) {
				/* this is true unless v is a start/end point
				 * for some ear.
				 */
				assert(v == ear[idx].u.id);
				v = ear[idx].v.id;

				++weight;
				++idx;
			}
			if (!id[u]) {
				Gr.add_vertex();
				id[u] = Gr.N - 1;
			}
			if (!id[v]) {
				Gr.add_vertex();
				id[v] = Gr.N - 1;
			}
			Gr.add_edge(id[u], id[v], weight);
			cerr << u << ' ' << v << ' ' << weight << endl;
		}
	}
#endif //__USE_DFS_TREE__
}

#endif //__BRANDES__
