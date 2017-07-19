#include <vector>
#include <cassert>
#include <iostream>
#include <chrono>
#include <stack>
#include "graph.hpp"
#include "bwc_brandes.hpp"

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

	#ifdef __BRANDES__
	/* no ear decomposition etc. required, we're just testing
	 * brandes algorithm
	 */
	auto start = chrono::steady_clock::now();

	cerr << "I/O done, computing things now" << endl;

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
		two_connected_prop T(g);
		T.ear_decompose();

		/*
		 * at this point, we are done with computing the ear-decomposition,
		 * we will just spend the rest of the time pretty printing
		 */

		auto finish = chrono::steady_clock::now();
		auto int_ms = chrono::duration_cast<chrono::milliseconds>(finish - start);

		cerr << "computed ear decomposition in "
			<< int_ms.count() << " milliseconds" << endl;

		auto vec = T.ear_decomposition;
		/* code used to print output in format for asc24b
		 * problem B over here(http://codeforces.com/gym/100340)
		 * this is useful to test any changes we make to algorithm,
		 * so I'm leaving it on here.
		 */
		//	if (vec.size() > 2) {
		//		cout << -1 << endl;
		//		return 0;
		//	}
		//	cout << vec[1].size() << endl;
		//	for (auto it = vec[1].rbegin(); it != vec[1].rend(); ++it) {
		//		auto p = *it;
		//		cout << p.size() << ' ';
		//		for (auto &e : p) {
		//			cout << e.u.id + 1 << ' ';
		//		}
		//		cout << p.rbegin()->v.id + 1 << '\n';
		//	}

		int conn_id = 1;
		for (auto &two_conn : vec) {
			printf("component %d\n", conn_id);
			int ear_id = 1;
			for (auto &ear : two_conn) {
				printf("ear %d\n", ear_id);
				for (auto &e : ear) {
					printf("\t%d -> %d\n", e.u.id, e.v.id);
				}
				++ear_id;
			}
			printf("\n");
			++conn_id;
		}

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
