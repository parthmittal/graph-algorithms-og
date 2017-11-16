#include <vector>
#include <cassert>
#include <iostream>
#include <chrono>
#include <stack>
#include "ear-decomposition-dfs.hpp"
#include "ear-decomposition-bfs.hpp"
#include "graph.hpp"
#include "bwc_brandes.hpp"
#include "bwc_our.hpp"

int main()
{
	using namespace std;
	//	freopen("ear.in", "r", stdin);
	//	freopen("ear.out", "w", stdout);

	int N, M;
	scanf("%d%d", &N, &M);
	undirected_graph_t g(N);
	for (int i = 0; i < M; ++i) {
		int u, v;
		scanf("%d%d", &u, &v);
		g.add_edge({u - 1}, {v - 1});
	}

	int deg_three = 0;
	for (int i = 0; i < N; ++i) {
		deg_three += (g.adj_list[i].size() > 2);
	}
	fprintf(stderr, "read graph with (%d) vertices with degree > 2\n", deg_three);

	#ifdef __BRANDES__
	/* no ear decomposition etc. required, we're just testing
	 * brandes algorithm
	 */
	auto start = chrono::steady_clock::now();
	auto bwc = compute_bwc(g);
	auto finish = chrono::steady_clock::now();
	auto int_ms = chrono::duration_cast<chrono::milliseconds>(finish - start);

	cerr << "computed betweenness centrality in"
		<< int_ms.count() << " milliseconds" << endl;
	for (int i = 0; i < N; ++i) {
		printf("%F\n", bwc[i]);
	}

	#else
		#ifdef __USE_DFS_TREE__
		auto start = chrono::steady_clock::now();

		bwc_our bwc(g);
		bwc.sim_brandes_all();

		for (int i = 0; i < N; ++i) {
			printf("%F\n", bwc.bwc[i]);
		}

		auto finish = chrono::steady_clock::now();
		auto int_ms = chrono::duration_cast<chrono::milliseconds>(finish - start);

		cerr << "computed bwc in " << int_ms.count() << " milliseconds" << endl;

		#else // __USE_DFS_TREE__ is not defined.
		auto start = chrono::steady_clock::now();
		two_connected_prop T(g);
		T.ear_decompose();

		auto finish = chrono::steady_clock::now();
		auto int_ms = chrono::duration_cast<chrono::milliseconds>(finish - start);

		cerr << "computed ear decomposition in "
			<< int_ms.count() << " milliseconds" << endl;

		#endif // __USE_DFS_TREE__

	#endif // __BRANDES__
}
