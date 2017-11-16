/* Largest 2-Connected Component:
 *
 * uses ear decomposition to compute the largest edge-biconnected component
 * of the input graph G 
 * Runs in O(V + E)
 */

#include <vector>
#include <algorithm>
#include <random>
#include <map>
#include <stack>
#include <cassert>
#include <iostream>
#include <cstdio>

using namespace std;

typedef pair<int, int> pii;
const int max_N = 5e6 + 10;

int vis[max_N], ent[max_N], low[max_N], p[max_N];
vector<int> E[max_N];
stack<pii> S;
vector< vector<pii> > bcc;

void dfs(int node, int &timer);
void store_bcc(int u, int v);

int main()
{
	using namespace std;
	int N, M;
	scanf("%d%d", &N, &M);

	for (int i = 1; i <= M; ++i) {
		int u, v;
		scanf("%d%d", &u, &v);
		E[u].push_back(v);
		E[v].push_back(u);
	}

	int timer = 1;
	for (int i = 1; i <= N; ++i) {
		if (!vis[i]) {
			dfs(i, timer);
		}
	}
	assert(timer == N + 1);

	vector<pii> all_edges;
	for (auto &B : bcc) {
		if (B.size() > all_edges.size()) {
			all_edges = B;
		}
	}

	/* count number of vertices */
	map<int, int> vert;
	for (auto &e : all_edges) {
		vert[e.first];
		vert[e.second];
	}

	printf("%lu %lu\n", vert.size(), all_edges.size());
	for (auto &e : all_edges) {
		printf("%d %d\n", e.first, e.second);
	}
}

void dfs(int node, int &timer)
{
	vis[node] = 1;
	low[node] = ent[node] = timer++;
	
	for (auto &v : E[node]) {
		if (!vis[v]) { /* tree edge */
			S.push({node, v});
			p[v] = node;
			dfs(v, timer);
			low[node] = min(low[node], low[v]);
			if (low[v] >= ent[node]) {
				store_bcc(node, v);
			}
		} else if (v != p[node] && ent[v] <= ent[node]) { /* back edge */
			low[node] = min(low[node], ent[v]);
			S.push({node, v});
		}
	}
}

void store_bcc(int u, int v)
{
	vector<pii> temp;
	while(S.top() != make_pair(u, v)) {
		temp.push_back(S.top());
		S.pop();
	}
	temp.push_back(S.top());
	S.pop();
	bcc.push_back(temp);
}
