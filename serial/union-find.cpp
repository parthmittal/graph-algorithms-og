#include "union-find.hpp"

#ifdef __UNION_BY_RANK__
union_find_t::union_find_t (int N)
{
	parent.resize(N);
	rank.resize(N);
}

void union_find_t::join(int x, int y)
{
	link(find_set(x), find_set(y));
}

void union_find_t::link(int x, int y)
{
	if (rank[x] > rank[y]) {
		parent[y] = x;
	} else {
		parent[x] = parent[y];
		if (x != y && rank[x] == rank[y]) {
			++rank[y];
		}
	}
}

int union_find_t::find_set(int x)
{
	#ifdef __PATH_COMPRESSION__
	/*
	 * this part is recursive, but depth of 
	 * tree is bounded by log(N), or even a 
	 * stronger bound, see CLRS
	 */
	if (x != parent[x]) {
		parent[x] = find_set(parent[x]);
	}
	return parent[x];
	#else // __PATH_COMPRESSION__ is not defined.
	while(x != parent[x]) {
		x = parent[x];
	}
	return x;
	#endif // __PATH_COMPRESSION__
}

#endif // __UNION_BY_RANK__
