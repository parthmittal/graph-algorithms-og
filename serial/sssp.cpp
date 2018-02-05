#include "graph.hpp"
#include "sssp.hpp"

#include <stack>
#include <queue>
#include <functional>
#include <iostream>
#include <chrono>

struct queue_element_t {
	int dist;
	int parent, node;

	bool operator>(const queue_element_t &other) const
	{
		return (dist > other.dist);
	}
};

long long sssp_profile = 0;

void
sssp(int source, const reduced_graph_t &G, std::vector<int> &S,
		std::vector<int> &dist, std::vector<ll> &num_paths)
{
	using namespace std;
	using namespace std::chrono;

	high_resolution_clock::time_point stime = high_resolution_clock::now();

	dist.resize(G.N, -1);
	num_paths.resize(G.N, 0);
	vector<int> popped(G.N, 0);

	priority_queue< queue_element_t, vector<queue_element_t>,
		greater<queue_element_t> > dfq;

	dist[source]      = 0;
	num_paths[source] = 1;
	dfq.push({0, -1, source});

	while(!dfq.empty()) {
		auto curr = dfq.top();
		dfq.pop();

		if (curr.dist == dist[curr.node]) {
			/* this is a shortest path to curr.node */
			if (curr.parent != -1) {
				num_paths[curr.node] += num_paths[curr.parent];
			}

			/* have we relaxed edges leaving curr.node? */
			if (!popped[curr.node]) {
				popped[curr.node] = 1;
				S.push_back(curr.node);
				for (auto &e : G.adj_list[curr.node]) {
					int targ   = e.v.id;
					int weight = dist[curr.node] + e.w;
					if (dist[targ] == -1 || weight <= dist[targ]) {
						dist[targ] = weight;
						dfq.push({weight, curr.node, targ});
					}
				}
			}
		}
	}

	high_resolution_clock::time_point etime = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>( etime - stime ).count();
	sssp_profile += duration;
}

void
sssp_khops(int source, int u, std::vector<int> &dist, reduced_graph_t &G,
		std::stack<int> &reset, int k)
{
	using namespace std;

	if (k <= 0) {
		return;
	}
	for (auto &e : G.adj_list[u]) {
		int w = dist[u] + e.w;
		int v = e.v.id;
		if (dist[v] == -1) {
			reset.push(v);
			dist[v] = w;
			sssp_khops(source, v, dist, G, reset, k - 1);
		} else if (dist[v] > w) {
			dist[v] = w;
			sssp_khops(source, v, dist, G, reset, k - 1);
		}
	}
}
