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

#include "graph.hpp"
#include "ear-decomposition-dfs.hpp"

int ed_size(std::vector<path_t> &ed);

int main()
{
	using namespace std;
	int N, M;
	scanf("%d%d", &N, &M);

	undirected_graph_t G(N);
	for (int i = 1; i <= M; ++i) {
		int u, v;
		scanf("%d%d", &u, &v);
		G.add_edge({u - 1}, {v - 1});
	}

	two_connected_prop T(G);
	T.ear_decompose();

	vector<path_t> best; int best_size = 0;
	for (auto &ed : T.ear_decomposition) {
		int temp = ed_size(ed);
		if (temp > best_size) {
			best = ed;
			best_size = temp;
		}
	}

	vector<edge_t> all_edges;
	for (auto &ear : best) {
		all_edges.insert(all_edges.end(), ear.begin(), ear.end());
	}

	/* don't make it too easy for the bwc code :) */
	auto rng = default_random_engine {};
	shuffle(begin(all_edges), end(all_edges), rng);

	/* count number of vertices */
	map<int, int> vert;
	for (auto &e : all_edges) {
		vert[e.u.id];
		vert[e.v.id];
	}

	printf("%lu %lu\n", vert.size(), all_edges.size());
	for (auto &e : all_edges) {
		printf("%d %d\n", e.u.id, e.v.id);
	}
}

int ed_size(std::vector<path_t> &ed)
{
	int tot = 0;
	for (auto &ear : ed) {
		tot += ear.size();
	}
	return tot;
}
