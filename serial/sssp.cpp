#include "graph.hpp"
#include "sssp.hpp"

#include <stack>
#include <queue>
#include <functional>
#include <iostream>

struct queue_element_t {
	int dist;
	int u, v;

	bool operator>(const queue_element_t &other) const
	{
		return (dist > other.dist);
	}
};

void
sssp(int source, const w_undirected_graph_t &G, std::vector<int> &S,
		std::vector<int> &dist, std::vector<ll> &num_paths,
		std::vector< std::vector<int> > &P)
{
	using namespace std;

	dist.resize(G.N, -1);
	num_paths.resize(G.N, 0);
	P.resize(G.N);
	vector<int> popped(G.N, 0);

	priority_queue< queue_element_t, vector<queue_element_t>,
		greater<queue_element_t> > dfq;

	dist[source] = 0;
	num_paths[source] = 1;
	dfq.push({0, -1, source});

	while(!dfq.empty()) {
		auto curr = dfq.top();
		dfq.pop();

		if (curr.dist == dist[curr.v]) {
			/* this is a shortest path to v */
			if (curr.u != -1) {
				cerr << curr.u << ' ' << curr.v << ' ' << curr.dist << endl;
				P[curr.v].push_back(curr.u);
				num_paths[curr.v] += num_paths[curr.u];
			}

			/* have we relaxed edges leaving curr.v? */
			if (!popped[curr.v]) {
				popped[curr.v] = 1;
				S.push_back(curr.v);
				for (auto &e : G.adj_list[curr.v]) {
					int v = e.v.id;
					int w = dist[curr.v] + e.w;
					if (dist[v] == -1 || w <= dist[v]) {
						dist[v] = w;
						dfq.push({w, curr.v, v});
					}
				}
			}
		}
	}
}
