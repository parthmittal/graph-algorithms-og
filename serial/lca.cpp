#include <vector>
#include "lca.hpp"
#include "union-find.hpp"

#ifdef __TARJAN_LCA__

lca_wrapper::lca_wrapper(int N)
	: ancestor(N), color(N), S(N) {}

std::vector< std::vector<int> >
lca_wrapper::lca(int u,
		const std::vector< std::vector<int> > &P,
		const std::vector< std::vector<int> > &T)
{
	static std::vector< std::vector<int> > ans = P;
	ancestor[u] = u;
	for (const int &v : T[u]) {
		lca(v, P, T);
		S.join(u, v);
		ancestor[S.find_set(u)] = u;
	}
	color[u] = 1;
	for (const int &v : P[u]) {
		if (color[v] == 1) {
			ans[u][&v - &P[u][0]] = S.find_set(v);
		}
	}
	return ans;
}

#endif // __TARJAN_LCA__
