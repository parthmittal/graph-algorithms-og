#include "bwc_brandes.hpp"
#include "graph.hpp"
#include <queue>
#include <stack>
#include <vector>

void
brandes1(const undirected_graph_t &G, int source, std::vector<double> &BC)
{
	using namespace std;
	stack<int> S;
	vector< vector<int> > P(G.N);

	vector<int> sig(G.N, 0); sig[source] = 1;
	vector<int> d(G.N, -1);  d[source]   = 0;

	queue<int> bfq;
	bfq.push(source);
	while(!bfq.empty()) {
		int u = bfq.front();
		bfq.pop();
		S.push(u);

		for (auto &v : G.adj_list[u]) {
			if (d[v.id] < 0) {
				bfq.push(v.id);
				d[v.id] = d[u] + 1;
			}
			if (d[v.id] == d[u] + 1) {
				//TODO: check if this overflows.
				sig[v.id] += sig[u];
				P[v.id].push_back(u);
			}
		}
	}

	vector<double> delta(G.N);
	while(!S.empty()) {
		int w = S.top();
		S.pop();
		for(auto &v : P[w]) {
			delta[v] += sig[v] / double(sig[w]) * (1 + delta[w]);
		}
		if (w != source) {
			BC[w] += delta[w];
		}
	}
}

std::vector<double>
compute_bwc(const undirected_graph_t &G)
{
	using namespace std;
	vector<double> BC(G.N);
	for (int source = 0; source < G.N; ++source) {
		brandes1(G, source, BC);
	}

	return BC;
}
