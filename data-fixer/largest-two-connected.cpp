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

#include <sys/resource.h>

using namespace std;

#define fast_cin() ios_base::sync_with_stdio(false)

typedef long double ld;
typedef long long ll;
typedef pair <int, int> pii;
typedef pair <ll, ll> pll;

const int max_N = 5e6 + 10;

vector<int> E[max_N];
int low_point[max_N], vis[max_N];
int depth[max_N], parent[max_N];
int root = 1;
stack<pii> E2; 
vector< vector<pii> > bccs;

void dfs_visit(int node);
void store_bcc(int u, int v);

int main()
{
	const rlim_t kStackSize = 1024 * 1024 * 1024; // 1024 MB.
	struct rlimit rl;
	int result;

	result = getrlimit(RLIMIT_STACK, &rl);
	if (result == 0)
	{
		if (rl.rlim_cur < kStackSize)
		{
			rl.rlim_cur = kStackSize;
			result = setrlimit(RLIMIT_STACK, &rl);
			if (result != 0)
			{
				fprintf(stderr, "setrlimit returned result = %d\n", result);
			}
		}
	}


	int N, M;
	scanf("%d%d", &N, &M);
	assert(N < max_N);

	for (int i = 1; i <= M; ++i) {
		int u, v;
		scanf("%d%d", &u, &v);
		E[u].push_back(v);
		E[v].push_back(u);
	}

	for (int i = 1; i <= N; ++i) {
		if (!vis[i]) {
			dfs_visit(i);
		}
	}

	vector<pii> ans;
	for (auto &v : bccs) {
		if (v.size() > ans.size()) {
			ans = v;
		}
	}

	map<int, int> V;
	for (auto &e : ans) {
		V[e.first];
		V[e.second];
	}
	printf("%lu %lu\n", V.size(), ans.size());
	for (auto &e : ans) {
		printf("%d %d\n", e.first, e.second);
	}
}

void dfs_visit(int node)
{
	vis[node] = 1;

	for (auto &v : E[node]) {
		if (!vis[v]) {
			E2.push({node, v});
			parent[v] = node;
			depth[v] = depth[node] + 1;
			low_point[v] = depth[v];

			dfs_visit(v);
			if (low_point[v] > depth[node]) {
				store_bcc(node, v);
			}
			low_point[node] = min(low_point[node], low_point[v]);
		} else if (v != parent[node] && depth[v] < depth[node]) {
			E2.push({node, v});
			low_point[node] = min(low_point[node], depth[v]);
		}
	}
}

void store_bcc(int u, int v)
{
	pii e;
	bccs.push_back(vector<pii>(0));
	auto &vec = bccs.back();
	do {
		e = E2.top();
		E2.pop();
		vec.push_back(e);
	} while(e != make_pair(u, v));
}
