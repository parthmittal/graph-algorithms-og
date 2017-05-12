#include <vector>

#ifndef __UNION_FIND_hpp__
#define __UNION_FIND_hpp__

#ifdef __UNION_BY_RANK__
struct union_find_t {
	std::vector<int> parent;
	std::vector<int> rank;

	union_find_t (int N);
	void join(int x, int y);
	void link(int x, int y);
	int find_set(int x);
};
#endif // __UNION_BY_RANK__

#endif // __UNION_FIND_hpp__
