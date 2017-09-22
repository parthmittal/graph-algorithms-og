#include "bwc_our.hpp"
#include "ear-decomposition-dfs.hpp"
#include "graph.hpp"
#include "sssp.hpp"
#include "count-sort.hpp"

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
		vector<int> active;
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

				int temp = 1;
				for (auto &w : active) {
					/* these are all the vertices between u and v
					 * on this ear */
					leftV[w] = u;
					rightV[w] = v;
					distL[w] = temp;
					distR[w] = weight - temp;
					++temp;
				}
				active.clear();

				cerr << id[u] << ' ' << id[v] << ' ' << weight << endl;

				u = v;
				weight = 1;
			} else {
				active.push_back(e.v.id);
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

			int temp = 1;
			for (auto &w : active) {
				/* these are all the vertices between u and v
				 * on this ear */
				leftV[w] = u;
				rightV[w] = v;
				distL[w] = temp;
				distR[w] = weight - temp;
				++temp;
			}
			active.clear();

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
	parents.resize(Gr.N);
	inorder.resize(Gr.N);

	for (int i = 0; i < Gr.N; ++i) {
		sssp(i, Gr, inorder[i], dist[i], num_paths[i], parents[i]);

		fprintf(stderr, "source is (%d)\n", rid[i]);
		for (int j = 0; j < Gr.N; ++j) {
			fprintf(stderr, "%d: (%d, %lld) ", j, dist[i][j], num_paths[i][j]);
		}
		fprintf(stderr, "\n");
	}

	cerr << "reduced graph forward phase complete" << endl;
}

void
bwc_our::sim_forward_phase(int u)
{
	using namespace std;
	if (G.adj_list[u].size() < 2) {
		/* not biconnected, something is wrong */
		assert(0);
	} else if (G.adj_list[u].size() >= 3) {
		/* should be a part of reduced graph */
		assert(id[u] != -1);
	} else {
		cerr << u << ' ' << leftV[u] << ' ' << rightV[u] << ' '
			<< distL[u] << ' ' << distR[u] << endl;
		const vector<int> &L = inorder[leftV[u]], &R = inorder[rightV[u]];

		vector<int> distu(G.N, -1);
		vector<long long> num_pathsu(G.N, 0);
		vector< vector<int> > P(G.N);

		num_pathsu[u] = 1;
		distu[u] = 0;
		num_pathsu[leftV[u]] = 1;
		num_pathsu[rightV[u]] = 1;
		distu[leftV[u]] = distL[u];
		distu[rightV[u]] = distR[u];

		size_t i = 0, j = 0;
		while(i < L.size() && j < R.size()) {
			int v = L[i], w = R[j];
			int d1 = distL[u] + dist[id[leftV[u]]][v];
			int d2 = distR[u] + dist[id[rightV[u]]][w];
			if (d1 <= d2) {
				if (distu[rid[v]] == -1 || d1 < distu[rid[v]]) {
					distu[rid[v]] = d1;
					num_pathsu[rid[v]] = 0;
				}
				if (d1 == distu[rid[v]]) {
					num_pathsu[rid[v]] += num_paths[id[leftV[u]]][v];
				}
				++i;
			} else {
				if (distu[rid[w]] == -1 || d2 < distu[rid[w]]) {
					distu[rid[w]] = d2;
					num_pathsu[rid[w]] = 0;
				}
				if (d2 == distu[rid[w]]) {
					num_pathsu[rid[w]] += num_paths[id[rightV[u]]][w];
				}
				++j;
			}
		}
		while(i < L.size()) {
			int v = L[i];
			int d1 = distL[u] + dist[id[leftV[u]]][v];
			if (distu[rid[v]] == -1 || d1 < distu[rid[v]]) {
				distu[rid[v]] = d1;
				num_pathsu[rid[v]] = 0;
			}
			if (d1 == distu[rid[v]]) {
				num_pathsu[rid[v]] += num_paths[id[leftV[u]]][v];
			}
			++i;
		}
		while(j < R.size()) {
			int w = R[j];
			int d2 = distR[u] + dist[id[rightV[u]]][w];
			if (distu[rid[w]] == -1 || d2 < distu[rid[w]]) {
				distu[rid[w]] = d2;
				num_pathsu[rid[w]] = 0;
			}
			if (d2 == distu[rid[w]]) {
				num_pathsu[rid[w]] += num_paths[id[rightV[u]]][w];
			}
			++j;
		}

		for (int i = 0; i < G.N; ++i) {
			if (distu[i] == -1) {
				/* must not be in the reduced graph */
				assert(id[i] == -1);

				/* first check if is in same ear as u */
				if (leftV[i] == leftV[u] && rightV[i] == rightV[u]) {
					/* two possibilities for best distance, 
					 * going through vertex w/ deg >= 3 or not */
					int D = min(abs(distL[u] - distL[i]),
							min(distu[leftV[i]] + distL[i], distu[rightV[i]] + distR[i]));
					distu[i] = D;
					if (abs(distL[u] - distL[i]) == D) {
						num_pathsu[i] += 1;
					}
					if (distu[leftV[i]] + distL[i] == D) {
						num_pathsu[i] += num_pathsu[leftV[i]];
					}
					if (distu[rightV[i]] + distR[i] == D) {
						num_pathsu[i] += num_pathsu[rightV[i]];
					}
				} else {
					int D = min(distu[leftV[i]] + distL[i], distu[rightV[i]] + distR[i]);
					distu[i] = D;
					if (distu[leftV[i]] + distL[i] == D) {
						num_pathsu[i] += num_pathsu[leftV[i]];
					}
					if (distu[rightV[i]] + distR[i] == D) {
						num_pathsu[i] += num_pathsu[rightV[i]];
					}
				}
			}
		}

		typedef pair<int, int> pii;
		vector<pii> S(G.N);
		for (int i = 0; i < G.N; ++i) {
			S[i] = {i, distu[i]};
		}
		S = counting_sort(S);
		fprintf(stderr, "with source (%d)\n", u);
		for (auto &v : S) {
			fprintf(stderr, "(%d, %d) ", v.first, v.second);
		}
		fprintf(stderr, "\n");
	}
}

#endif //__OUR__
