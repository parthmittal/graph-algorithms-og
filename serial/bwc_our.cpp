#include "bwc_our.hpp"
#include "ear-decomposition-dfs.hpp"
#include "graph.hpp"
#include "sssp.hpp"

#include <queue>
#include <stack>
#include <iostream>
#include <algorithm>

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
					rid[Gr.N] = u;
					id[u] = Gr.N;
					Gr.add_vertex();
				}
				if (id[v] == -1) {
					rid[Gr.N] = v;
					id[v] = Gr.N;
					Gr.add_vertex();
				}
				Gr.add_edge(id[u], id[v], weight);

				cerr << id[u] << ' ' << id[v] << ' ' << weight << endl;

				u = v;
				weight = 1;
			} else {
				++weight;
			}
		}
		if (u != ear.back().v.id) {
			int v = ear.back().v.id;
			if (id[u] == -1) {
				rid[Gr.N] = u;
				id[u] = Gr.N;
				Gr.add_vertex();
			}
			if (id[v] == -1) {
				rid[Gr.N] = v;
				id[v] = Gr.N;
				Gr.add_vertex();
			}

			Gr.add_edge(id[u], id[v], weight);

			cerr << id[u] << ' ' << id[v] << ' ' << weight << endl;
		}
	}

	cerr << "reduced graph constructed" << endl;
}

	void
bwc_our::forward_phase_reduced_graph()
{
	using namespace std;
	dist.resize(Gr.N);
	num_paths.resize(Gr.N);
	inorder.resize(Gr.N);
	parents.resize(Gr.N);

	for (int i = 0; i < Gr.N; ++i) {
		stack<int> S;
		sssp(i, Gr, S, dist[i], num_paths[i], parents[i]);
		while(!S.empty()) {
			inorder[i].push_back(S.top());
			S.pop();
		}
		reverse(inorder[i].begin(), inorder[i].end());

		fprintf(stderr, "source is (%d)\n", rid[i]);
		for (int j = 0; j < Gr.N; ++j) {
			fprintf(stderr, "%d: (%d, %lld) ", j, dist[i][j], num_paths[i][j]);
		}
		fprintf(stderr, "\n");
	}
}
#endif //__OUR__
