#include <vector>
#include <cassert>
#include <iostream>
#include <chrono>
#include <stack>
#include "ear-decomposition-dfs.hpp"
#include "graph.hpp"

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
}
