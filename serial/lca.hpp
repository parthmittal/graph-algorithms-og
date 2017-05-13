#include <vector>
#include "union-find.hpp"

#ifndef __LCA_hpp__
#define __LCA_hpp__

#ifdef __TARJAN_LCA__

struct lca_wrapper {
	std::vector<int> ancestor;
	std::vector<int> color;
	union_find_t S;

	lca_wrapper(int N);

	std::vector< std::vector<int> >
	lca(int root,
		const std::vector< std::vector<int> > &P,
		const std::vector< std::vector<int> > &T);
};

#endif // __TARJAN_LCA__

#endif // __LCA_hpp__
