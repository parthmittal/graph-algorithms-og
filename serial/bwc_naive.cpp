#include <iostream>
#include <vector>
#include <iomanip>
#include <queue>

#include <cstring>
#include <cassert>

const int max_N = 1e5 + 5;
using namespace std;

struct vprop {
	int dist;
	long long num_paths;
};

vector<int> E[max_N];
int vis[max_N];
double BC[max_N];

vector<vprop> dist_from_source(int source, int N);

int main()
{
	int N, M;
	scanf("%d%d", &N, &M);

	assert(N < max_N);

	for (int i = 1; i <= M; ++i) {
		int u, v;
		scanf("%d%d", &u, &v);
		E[u].push_back(v);
		E[v].push_back(u);
	}

	for (int s = 1; s <= N; ++s) {
		for (int t = 1; t <= N; ++t) {
			if (t != s) {
				vector<vprop> ds = dist_from_source(s, N);
				vector<vprop> dt = dist_from_source(t, N);
				for (int v = 1; v <= N; ++v) {
					if (v != s && v != t) {
						if (ds[v].dist + dt[v].dist == ds[t].dist) {
							BC[v] += (ds[v].num_paths * dt[v].num_paths) / double(ds[t].num_paths);
						}
					}
				}
			}
		}
	}

	cout << fixed << setprecision(6);
	for (int i = 1; i <= N; ++i) {
		cout << BC[i] << '\n';
	}
}

vector<vprop> dist_from_source(int source, int N)
{
	memset(vis, 0, sizeof(vis));
	queue<int> bfq;
	vector<vprop> prop(N + 1);
	bfq.push(source);
	vis[source] = 1;
	prop[source].num_paths = 1;

	while(!bfq.empty()) {
		int u = bfq.front();
		bfq.pop();
		for (auto &v : E[u]) {
			if (!vis[v]) {
				vis[v] = 1;
				prop[v].dist = prop[u].dist + 1;
				bfq.push(v);
			}
			if (prop[v].dist == prop[u].dist + 1) {
				prop[v].num_paths += prop[u].num_paths;
			}
		}
	}

	return prop;
}
