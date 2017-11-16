#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <cmath>
#include <climits>

#include <sstream>
#include <iostream>
#include <iomanip>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <queue>
#include <stack>
#include <utility>
#include <vector>
#include <functional>
#include <algorithm>

using namespace std;

#define fast_cin() ios_base::sync_with_stdio(false)

typedef long double ld;
typedef long long ll;
typedef pair <int, int> pii;
typedef pair <ll, ll> pll;

map<int, int> coord;

int main()
{
	int N, M;
	scanf("%d%d", &N, &M);

	set<pii> edges; /* don't want multi-edges */
	for (int i = 1; i <= M; ++i) {
		pii e;
		scanf("%d%d", &e.first, &e.second);
		coord[e.first];
		coord[e.second];
		if (e.first > e.second) {
			swap(e.first, e.second);
		}
		if (e.first < e.second) {
			edges.insert(e);
		}
	}

	printf("%d %lu\n", N, edges.size());

	int j = 1;
	for (auto &x : coord) {
		x.second = j++;
	}
	for (auto e : edges) {
		e.first  = coord[e.first];
		e.second = coord[e.second];
		printf("%d %d\n", e.first, e.second);
	}
}
