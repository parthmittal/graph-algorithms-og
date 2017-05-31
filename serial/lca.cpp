#include <vector>
#include "lca.hpp"
#include "union-find.hpp"

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

#endif // __TARJAN_LCA__
