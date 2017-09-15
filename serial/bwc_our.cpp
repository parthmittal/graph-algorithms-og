#include "bwc_our.hpp"
#include "ear-decomposition-dfs.hpp"
#include "graph.hpp"

#include <queue>
#include <stack>
#include <iostream>

#include <cassert>

#ifdef __OUR__

void
bwc_our::compute_reduced_graph()
{
	using namespace std;
	two.ear_decompose();

	/* NB: currently we expect that G is two-connected */
	assert(two.ear_decomposition.size() == 1);

	auto &ED = two.ear_decomposition[0];
	for (path_t &ear : ED) {
		assert(ear.size() >= 1);
		int weight = 1, u = ear[0].u.id;
		for (auto &e : ear) {
			if (weight > 0 && G.adj_list[e.v.id].size() >= 3) {
				/* add an edge between u and e.v */
				int v = e.v.id;
				if (id[u] == -1) {
					id[u] = Gr.N;
					Gr.add_vertex();
				}
				if (id[v] == -1) {
					id[v] = Gr.N;
					Gr.add_vertex();
				}
				Gr.add_edge(id[u], id[v], weight);
				Gr.add_edge(id[v], id[u], weight);

				cerr << u << ' ' << v << ' ' << weight << endl;

				u = v;
				weight = 1;
			} else {
				++weight;
			}
		}
		if (u != ear.back().v.id) {
			int v = ear.back().v.id;
			if (id[u] == -1) {
				id[u] = Gr.N;
				Gr.add_vertex();
			}
			if (id[v] == -1) {
				id[v] = Gr.N;
				Gr.add_vertex();
			}

			Gr.add_edge(id[u], id[v], weight);
			Gr.add_edge(id[v], id[u], weight);

			cerr << u << ' ' << v << ' ' << weight << endl;
		}
	}
}

#endif //__OUR__
