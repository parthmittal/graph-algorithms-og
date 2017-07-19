#include "bwc_brandes.hpp"
#include "graph.hpp"
#include <queue>
#include <stack>
#include <omp.h>
#include <cstdio>

std::vector<double>
compute_bwc(const undirected_graph_t &G)
{
	using namespace std;
	vector<double> BC(G.N);
	vector<omp_lock_t> locks(G.N);

	for (auto &x : locks) {
		omp_init_lock(&x);
	}

	omp_set_num_threads(20);

#pragma omp parallel
	{
		long long tid = omp_get_thread_num(), threads = omp_get_num_threads();
		long long lb = tid, ub = lb + 1;
		vector<double> BC_local(G.N);

		for (int source = (lb * G.N) / threads; source < (ub * G.N) / threads; ++source) {
			stack<int> S;
			vector< vector<int> > P(G.N);
			//sig[v] counts the number of shortest paths
			//which pass through v.
			vector<long long> sig(G.N); sig[source] = 1;
			vector<int> d(G.N, -1); d[source] = 0;

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
					omp_set_lock(&(locks[w]));
					BC[w] += delta[w];
					omp_unset_lock(&(locks[w]));
				}
			}
		}

		for (int i = 0; i < G.N; ++i) {
			omp_set_lock(&locks[i]);
			BC[i] += BC_local[i];
			omp_unset_lock(&(locks[i]));
		}
	}

	for (auto &x : locks) {
		omp_destroy_lock(&x);
	}

	return BC;
}
