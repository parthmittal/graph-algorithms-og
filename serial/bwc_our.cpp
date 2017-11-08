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
bwc_our::add_vertex_reduced_graph(int v)
{
	if (id[v] == -1) {
		rid[Gr.N] = v;
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
	for (path_t &ear : ED) {
		assert(ear.size() >= 1);

		/* increase the degree of the beginning and end of ear,
		 * so even if their degree was only 2, they still become end-points
		 * of edges in Gr */
		int ear_start = ear[0].u.id, ear_end = ear.back().v.id;
		G.adj_list[ear_start].push_back({-1});
		G.adj_list[ear_end].push_back({-1});

		int weight = 1, u = ear[0].u.id;
		edge_t joint_start_edge;
		vector<int> active;
		for (auto &e : ear) {
			if (weight == 1) {
				/* the new joint starts at e.u, so we should store e */
				joint_start_edge = e;
			}
			if (weight > 0 && G.adj_list[e.v.id].size() >= 3) {
				/* add an edge between u and e.v */
				int v = e.v.id;
				
				/* NB: this does nothing if the vertices are already
				 * in the reduced graph */
				add_vertex_reduced_graph(u);
				add_vertex_reduced_graph(v);

				int p = joint_start_edge.v.id;
				int q = e.u.id;
				//fprintf(stderr, "%d(%d) <%d--(%d)--%d> %d(%d)\n",
				//		u, id[u], p, weight, q, v, id[v]);
				Gr.add_edge(id[u], id[v], p, q, active, weight);

				int temp = 1;
				for (auto &w : active) {
					/* these are all the vertices between u and v
					 * on this ear */
					leftV[w]  = u;
					rightV[w] = v;
					distL[w]  = temp;
					distR[w]  = weight - temp;
					//fprintf(stderr, "%d %d %d\n", w, distL[w], distR[w]);
					++temp;
				}
				active.clear();

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
	cerr << "constructed reduced graph" << endl;
}

void
bwc_our::sim_brandes1(int u, const rgraph_vinfo &Lrv, const rgraph_vinfo &Rrv)
{
	using namespace std;

	vector<int> dist(G.N, -1);
	vector<long long> num_paths(G.N, 0);
	vector< vector<int> > P(G.N);

	#define SET_DISTANCE(v, distance, paths, parents)                \
	do {                                                             \
		if (v == leftV[u] || v == rightV[u]) {                       \
			continue;                                                \
		}                                                            \
		if (dist[v] == -1 || distance < dist[v]) {                   \
			dist[v]      = distance;                                 \
			num_paths[v] = 0;                                        \
			P[v].clear();                                            \
		}                                                            \
		if (distance == dist[v]) {                                   \
			num_paths[v] += paths;                                   \
			P[v].insert(P[v].end(), parents.begin(), parents.end()); \
		}                                                            \
	} while (0)

	if (G.adj_list[u].size() < 2) {
		/* not biconnected, something is wrong */
		assert(0);
	} else if (G.adj_list[u].size() >= 3) {
		/* should be a part of reduced graph */
		assert(id[u] != -1);

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
				P[i]         = Lrv.parents[id[i]];
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

			P[leftV[u]].push_back(Lrv.p);
			P[rightV[u]].push_back(Lrv.q);
		} else if (leftV[u] == rightV[u]) {
			int v = leftV[u];
			int d1 = distL[u], d2 = distR[u];
			dist[v] = min(d1, d2);

			if (d1 == dist[v]) {
				P[v].push_back(Lrv.p);
				num_paths[v]++;
			} 
			if (d2 == dist[v]) {
				P[v].push_back(Lrv.q);
				num_paths[v]++;
			}
		}

		size_t i = 0, j = 0;
		while(i < L.size() && j < R.size()) {
			int v = L[i], w = R[j];
			int d1 = distL[u] + Lrv.dist[v];
			int d2 = distR[u] + Rrv.dist[w];
			if (d1 <= d2) {
				SET_DISTANCE(rid[v], d1, Lrv.num_paths[v], Lrv.parents[v]);
				++i;
			} else {
				SET_DISTANCE(rid[w], d2, Rrv.num_paths[w], Rrv.parents[w]);
				++j;
			}
		}
		while(i < L.size()) {
			int v = L[i];
			int d1 = distL[u] + Lrv.dist[v];
			SET_DISTANCE(rid[v], d1, Lrv.num_paths[v], Lrv.parents[v]);
			++i;
		}
		while(j < R.size()) {
			int w = R[j];
			int d2 = distR[u] + Rrv.dist[w];
			SET_DISTANCE(rid[w], d2, Rrv.num_paths[w], Rrv.parents[w]);
			++j;
		}

		for (int i = 0; i < G.N; ++i) {
			if (dist[i] != -1) {
				continue;
			}
			if (id[i] == -1) {
				/* not in the reduced graph.
				 * NB: we don't care about this nodes parents, since
				 * it only has two edges on it */
				assert(G.adj_list[i].size() == 2);

				/* first check if is in same ear as u */
				if (leftV[i] == leftV[u] && rightV[i] == rightV[u]) {
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
	 * For every vertex in the reduced graph, we store this
	 * explicitly.
	 * For every vertex v not in the reduced graph, we check every
	 * vertex incident on v (since there are only 2 of them)
	 */

	vector<double> delta(G.N);
	for (auto &x : S) {
		int v = x.first;
		ll par_sum = 0;
		if (id[v] == -1) {
			/* not in the reduced graph */
			for (auto &e : G.adj_list[v]) {
				int w = e.id;
				if (dist[w] + 1 == dist[v]) {
					/* w is a parent of v */
					delta[w] += ((1 + delta[v]) * num_paths[w]) / num_paths[v];
					par_sum += num_paths[w];
				}
			}
		} else {
			/* in reduced graph, parents are already known */
			for (auto &w : P[v]) {
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
}

rgraph_vinfo bwc_our::get_node_info(int u)
{
	rgraph_vinfo info;
	sssp(u, Gr, info.inorder, info.dist, info.num_paths, info.parents);
	return info;
}

void bwc_our::sim_brandes_all()
{
	/* idea:
	 * Do a BFS of the reduced graph (treating edges as unweighted).
	 * Whenever you see edge (u, v), make sure you know rgraph_vinfo
	 * and u and v (use sssp.hpp API) */

	using namespace std;

	#define FREE_MEMORY(v)                           \
	do {                                             \
		vector<int>().swap(info[v].dist);            \
		vector<long long>().swap(info[v].num_paths); \
		vector<vector<int>>().swap(info[v].parents); \
		vector<int>().swap(info[v].inorder);         \
	} while(0)

	int root = 0;
	queue<int> bfq;

	vector<rgraph_vinfo> info(Gr.N);
	vector<int> vis(Gr.N);

	info[root] = get_node_info(root);
	bfq.push(root);

	while(!bfq.empty()) {
		int u = bfq.front();
		bfq.pop();

		for (auto &e : Gr.adj_list[u]) {
			int v = e.v.id;
			if (vis[v] == 0) {
				info[v] = get_node_info(v);
				bfq.push(v);
				vis[v] = 1;
			}
			if (vis[v] == 1) {
				for (auto &w : e.vids) {
					info[u].p = e.p;
					info[u].q = e.q;
					sim_brandes1(w, info[u], info[v]);
				}
			}
		}

		sim_brandes1(rid[u], info[u], info[u]);
		vis[u] = 2;
		FREE_MEMORY(u);
	}
}

#endif //__OUR__
