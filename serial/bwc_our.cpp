#include "bwc_our.hpp"
#include "ear-decomposition-dfs.hpp"
#include "graph.hpp"
#include "sssp.hpp"
#include "count-sort.hpp"
#include "bwc_brandes.hpp"

#include <queue>
#include <stack>
#include <iostream>
#include <algorithm>
#include <memory>

#include <cassert>

#ifdef __OUR__

void
bwc_our::add_vertex_reduced_graph(int v)
{
	if (id[v] == -1) {
		rid.push_back(v);
		id[v] = Gr.N;
		Gr.add_vertex(); /* adds a vertex with label id[u] */
	} else {
		/* vertex is already in reduced graph, nothing to do here */
	}
}

void
bwc_our::compute_reduced_graph()
{
	using namespace std;
	two.ear_decompose();

	/* NB: currently we expect that G is two-connected */
	assert(two.ear_decomposition.size() == 1);

	auto &ED = two.ear_decomposition[0];
	int eid = 1;
	for (path_t &ear : ED) {
		assert(ear.size() >= 1);

		/* increase the degree of the beginning and end of ear,
		 * so even if their degree was only 2, they still become end-points
		 * of edges in Gr */
		int ear_start = ear[0].u.id, ear_end = ear.back().v.id;
		G.adj_list[ear_start].push_back({-1});
		G.adj_list[ear_end].push_back({-1});

		int weight = 1, u = ear[0].u.id;
		vector<int> active;
		for (auto &e : ear) {
			if (weight > 0 && G.adj_list[e.v.id].size() >= 3) {
				/* add an edge between u and e.v */
				int v = e.v.id;
				
				/* NB: this does nothing if the vertices are already
				 * in the reduced graph */
				add_vertex_reduced_graph(u);
				add_vertex_reduced_graph(v);

				Gr.add_edge(id[u], id[v], active, weight);

				int temp = 1;
				for (auto &w : active) {
					/* these are all the vertices between u and v
					 * on this ear */
					leftV[w]      = u;
					rightV[w]     = v;
					Gr.sig[id[u]] = 1;
					Gr.sig[id[v]] = 1;
					distL[w]      = temp;
					distR[w]      = weight - temp;
					joint_id[w]   = eid;
					//fprintf(stderr, "%d %d %d\n", w, distL[w], distR[w]);
					++temp;
				}
				active.clear();
				++eid;

				u = v;
				weight = 1;
			} else {
				active.push_back(e.v.id);
				++weight;
			}
		}
		/* make sure we did collapse all the edges */
		assert(u == ear.back().v.id);

		/* cleaning up:
		 * remember to remove the fake edges you added to G */
		G.adj_list[ear_start].pop_back();
		G.adj_list[ear_end].pop_back();
	}

	int mxd = 0;
	int nedges = 0;
	int cut = 0;
	for (int i = 0; i < Gr.N; ++i) {
		int actual_degree = 0;
		for (auto &fe : Gr.adj_list[i]) {
			int x = min(i, fe.v.id), y = max(i, fe.v.id);
			if (fe.w == Gr.best_cost[{x, y}]) {
				fe.relevant = true;
				++actual_degree;
			} else {
				++cut;
			}
		}
		mxd = max(mxd, actual_degree);
		nedges += actual_degree;
	}

	nedges /= 2;
	cut /= 2;

	fprintf(stderr, "cut (%d) edges\n", cut);

	fprintf(stderr, "constructed reduced graph, with %d vertices, "
			"%d edges, and max degree (%d) \n", Gr.N, nedges, mxd);
}

	void
bwc_our::sim_brandes1(int u, const rgraph_vinfo &Lrv, const rgraph_vinfo &Rrv)
{
	using namespace std;

	vector<int> dist(G.N, -1);
	vector<long long> num_paths(G.N, 0);

	/* here, v is in V(G) */
#define SET_DISTANCE(v, distance, paths)        \
	do {                                            \
		if (dist[v] == -1 || distance < dist[v]) {  \
			dist[v]      = distance;                \
			num_paths[v] = 0;                       \
		}                                           \
		if (distance == dist[v]) {                  \
			num_paths[v] += paths;                  \
		}                                           \
	} while (0)

	if (G.adj_list[u].size() < 2) {
		/* not biconnected, something is wrong */
		assert(0);
	} else if (id[u] != -1) {
		/* we already know the distance of every vertex in Gr
		 * from u : Lrv.dist, Lrv.num_paths, etc. */
		dist[u] = 0;
		num_paths[u] = 1;

		for (int i = 0; i < G.N; ++i) {
			if (id[i] == -1) {
				/* is not in reduced graph */
				/* check path length from left and right joint of i */
				int L = id[leftV[i]], R = id[rightV[i]];
				const int dL = distL[i] + Lrv.dist[L];
				const int dR = distR[i] + Lrv.dist[R];
				dist[i] = min(dL, dR);

				if (dL == dist[i]) {
					num_paths[i] += Lrv.num_paths[L];
				}
				if (dR == dist[i]) {
					num_paths[i] += Lrv.num_paths[R];
				}
			} else {
				/* simply "import" values from Lrv */
				dist[i]      = Lrv.dist[id[i]];
				num_paths[i] = Lrv.num_paths[id[i]];
			}
		}
	} else {
		//fprintf(stderr, "(%d) <(%d)-%d-(%d)> (%d)\n",
		//		leftV[u], distL[u], u, distR[u], rightV[u]);
		const vector<int> &L = Lrv.inorder, &R = Rrv.inorder;

		num_paths[u] = 1;
		dist[u] = 0;
		if (leftV[u] != rightV[u]) {
			dist[leftV[u]]  = distL[u];
			dist[rightV[u]] = distR[u];

			num_paths[leftV[u]]  = 1;
			num_paths[rightV[u]] = 1;

			size_t i = 1, j = 1;
			while(i < L.size() && j < R.size()) {
				int v = L[i], w = R[j];
				int d1 = distL[u] + Lrv.dist[v];
				int d2 = distR[u] + Rrv.dist[w];
				if (d1 <= d2) {
					SET_DISTANCE(rid[v], d1, Lrv.num_paths[v]);
					++i;
				} else {
					SET_DISTANCE(rid[w], d2, Rrv.num_paths[w]);
					++j;
				}
			}
			while(i < L.size()) {
				int v = L[i];
				int d1 = distL[u] + Lrv.dist[v];
				SET_DISTANCE(rid[v], d1, Lrv.num_paths[v]);
				++i;
			}
			while(j < R.size()) {
				int w = R[j];
				int d2 = distR[u] + Rrv.dist[w];
				SET_DISTANCE(rid[w], d2, Rrv.num_paths[w]);
				++j;
			}
		} else if (leftV[u] == rightV[u]) {
			int v = leftV[u];
			int d1 = distL[u], d2 = distR[u];
			dist[v] = min(d1, d2);

			if (d1 == dist[v]) {
				num_paths[v]++;
			} 
			if (d2 == dist[v]) {
				num_paths[v]++;
			}

			for (int i = 0; i < Gr.N; ++i) {
				if (i != id[v]) {
					dist[rid[i]]      = dist[v] + Lrv.dist[i];
					num_paths[rid[i]] = num_paths[v] * Lrv.num_paths[i];
				}
			}
		}

		for (int i = 0; i < G.N; ++i) {
			if (dist[i] != -1) {
				continue;
			}
			if (id[i] == -1) {
				assert(G.adj_list[i].size() == 2);

				/* first check if is in same ear-joint as u */
				if (joint_id[u] == joint_id[i]) {
					/* two possibilities for best distance, 
					 * going through vertex w/ deg >= 3 or not */

					const int d1 = abs(distL[u] - distL[i]);
					const int d2 = dist[leftV[i]] + distL[i];
					const int d3 = dist[rightV[i]] + distR[i];
					dist[i] = min(d1, min(d2, d3));

					/* once we know the best distance, add
					 * to num_paths every way we can achieve it */

					if (d1 == dist[i]) {
						num_paths[i] += 1;
					}
					if (d2 == dist[i]) {
						num_paths[i] += num_paths[leftV[i]];
					}
					if (d3 == dist[i]) {
						num_paths[i] += num_paths[rightV[i]];
					}
				} else {
					/* the main idea is the same as above, except
					 * since the two vertices are on different ears,
					 * we don't have one possibility */

					const int d1 = dist[leftV[i]] + distL[i];
					const int d2 = dist[rightV[i]] + distR[i];
					dist[i] = min(d1, d2);

					if (d1 == dist[i]) {
						num_paths[i] += num_paths[leftV[i]];
					}
					if (d2 == dist[i]) {
						num_paths[i] += num_paths[rightV[i]];
					}
				}
			} else {
				assert(0);
			}
		}
	}

	/* Now we know the distance for every vertex v of G:
	 * - the distance of v from u
	 * - the number of shortest paths from u to v
	 * For the reverse phase of Brandes Algorithm, we need
	 * to order the vertices in decreasing order of distance
	 * from u. Since the distances are all <= |G.V|, we can
	 * use counting sort.
	 */

	typedef pair<int, int> pii;
	vector<pii> S(G.N);
	for (int i = 0; i < G.N; ++i) {
		S[i] = {i, dist[i]};
		assert(dist[i] < G.N);
	}
	S = counting_sort(S);
	reverse(S.begin(), S.end());

	/* We also need for each vertex v the "parents" of v,
	 * which is a list of all the vertices that are adjacent
	 * to v on a shortest path from u to v.
	 * We don't store this explicitly, instead note that
	 * u is a parent of v iff d[u] + 1 == d[v] && (u, v) is an edge of G.
	 */

	vector<double> delta(G.N);
	for (auto &x : S) {
		int v = x.first;
		ll par_sum = 0;
		for (auto &e : G.adj_list[v]) {
			int w = e.id;
			if (dist[w] + 1 == dist[v]) {
				/* w is a parent of v */
				delta[w] += ((1 + delta[v]) * num_paths[w]) / num_paths[v];
				par_sum += num_paths[w];
			}
		}

		if (v != u) {
			bwc[v] += delta[v];
		} else {
			assert(par_sum == 0);
		}
	}

#undef SET_DISTANCE
}

std::unique_ptr<rgraph_vinfo> bwc_our::get_node_info(int u)
{
	rgraph_vinfo info;
	sssp(u, Gr, info.inorder, info.dist, info.num_paths);
	return std::make_unique<rgraph_vinfo>(info);
}

void bwc_our::sim_brandes_all()
{
	/* idea:
	 * Do a BFS of the reduced graph (treating edges as unweighted).
	 * Whenever you see edge (u, v), make sure you know rgraph_vinfo
	 * and u and v (use sssp.hpp API) */

	using namespace std;

	vector<unique_ptr<rgraph_vinfo>> info(Gr.N);
	vector<int> vis(Gr.N);
	vector<int> done(G.N);

#ifdef __DRY_RUN__
	int max_allocated     = 0;
	int current_allocated = 0;
#endif // __DRY_RUN__
	for (int root = 0; root < Gr.N; ++root) {
		if (!Gr.sig[root] || vis[root]) {
			continue;
		}

		queue<int> bfq;
		bfq.push(root);
		vis[root] = 1;

#ifdef __DRY_RUN__
		++current_allocated;
		max_allocated = max(max_allocated, current_allocated);
#else
		info[root] = get_node_info(root);
#endif

		while(!bfq.empty()) {
			int u = bfq.front();
			bfq.pop();

			for (auto &e : Gr.adj_list[u]) {
				int v = e.v.id;
				if (vis[v] == 0 && Gr.sig[v] && !e.vids.empty()) {
					/* the dry run simulates the allocations we'd make */
					bfq.push(v);
					vis[v] = 1;

#ifdef __DRY_RUN__
					++current_allocated;
					max_allocated = max(max_allocated, current_allocated);
#else /* not a dry run, so actually make the allocation */
					info[v] = get_node_info(v);
#endif
				}
				if (vis[v] == 1) {
#ifndef __DRY_RUN__
					for (auto &w : e.vids) {
						if (!done[w]) {
							done[w] = 1;
							sim_brandes1(w, *info[id[leftV[w]]],
									*info[id[rightV[w]]]);
						}
					}
#endif
				}
			}

			vis[u] = 2;
#ifdef __DRY_RUN__
			--current_allocated;
#else
			if (!done[rid[u]]) {
				sim_brandes1(rid[u], *info[u], *info[u]);
				info[u].reset(); /* calls destructor on *(info[u]) */
			}
#endif
		}
	}

	for (int root = 0; root < Gr.N; ++root) {
		if (!Gr.sig[root]) {
#ifndef __DRY_RUN__
			assert(!vis[root]);
			assert(!done[rid[root]]);
			/* instead of simulating for this , just run Brandes Algorithm */
			brandes1(G, rid[root], bwc);
#endif
		}
	}

#ifdef __DRY_RUN__
	fprintf(stderr, "max number of active elements was %d\n", max_allocated);
#endif

	fprintf(stderr, "total time on SSSP in reduced graph was %lld ms\n",
			sssp_profile);
}

#endif //__OUR__
