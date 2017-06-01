#include <vector>
#include <algorithm>
#include <cmath>
#include <utility>
#include <iostream>

#include "lca.hpp"
#include "union-find.hpp"
#include "graph.hpp"

#ifdef __TARJAN_LCA__

lca_wrapper::lca_wrapper(int N)
	: ancestor(N), color(N), S(N) {}

void
lca_wrapper::lca(int u,
		const std::vector< std::vector<int> > &P,
		const std::vector< std::vector<int> > &T,
		const std::vector<int> &dist,
		std::vector<std::vector<std::pair<int, int>>> &edges_by_lca_dist)
{
	ancestor[u] = u;
	for (const int &v : T[u]) {
		lca(v, P, T, dist, edges_by_lca_dist);
		S.join(u, v);
		ancestor[S.find_set(u)] = u;
	}
	color[u] = 1;
	for (const int &v : P[u]) {
		if (color[v] == 1) {
			int lca = S.find_set(v);
			int d = dist[lca];
			edges_by_lca_dist[d].push_back({u, v});
		}
	}
}

#elif defined(__DP_LCA__)
void
lca_wrapper::lca(const std::vector< std::vector<int> > &P,
		const std::vector<vertex_t> &parent,
		const std::vector<int> &dist,
		std::vector<std::vector<std::pair<int, int>>> &edges_by_lca_dist)
{
	int D = *std::max_element(dist.begin(), dist.end());
	int lg = 0;
	while((1 << lg) <= D) {
		++lg;
	}
	--lg;

	pa.resize(lg + 1, std::vector<int>(dist.size(), -1));

	for (int i = 0; i < parent.size(); ++i) {
		pa[0][i] = parent[i].id;
	}

	for (int j = 1; j <= lg; ++j) {
		for (int i = 0; i < parent.size(); ++i) {
			if (pa[j - 1][i] != -1) {
				pa[j][i] = pa[j - 1][pa[j - 1][i]];
			}
		}
	}

	for (auto &U : P) {
		for (const int &V : U) {
			int u = &U - &P[0];
			int v = &V - &U[0];
			std::pair<int, int> q = {u, v};

			if (dist[u] < dist[v]) {
				std::swap(u, v);
			}

			for (int j = lg; j >= 0; --j) {
				if (dist[u] - (1 << j) >= dist[v]) {
					u = pa[j][u];
				}
			}

			if (u != v) {
				for (int j = lg; j >= 0; --j) {
					if (pa[j][u] != pa[j][v]) {
						u = pa[j][u];
						v = pa[j][v];
					}
				}
				u = pa[0][u];
			}

			edges_by_lca_dist[dist[u]].push_back(q);
		}
	}
}

#endif // __TARJAN_LCA__
