#include <vector>
#include <cassert>
#include <iostream>
#include <chrono>
#include <stack>
#include "ear-decomposition-dfs.hpp"
#include "graph.hpp"

#ifdef __USE_DFS_TREE__

two_connected_prop::two_connected_prop(const undirected_graph_t &G)
	:G(G), visited(G.N), parent(G.N), back_edges(G.N), entry_time(G.N),
	 exit_time(G.N), vert(G.N + 1), visited_ear_decomposition(G.N),
	 ears_of(G.N)
{}

void
two_connected_prop::dfs(vertex_t root, int &current_time)
{
	std::stack<dfs_stack_elem_t> dfs_stack;
	#ifdef __DFS_LESS_SPACE__
	dfs_stack.push({root, G.adj_list[root.id].begin()});
	visited[root.id] = 1;
	entry_time[root.id] = ++current_time;
	vert[current_time] = root;
	#else // can use O(E) space
	dfs_stack.push({root, true, -1});
	#endif // __DFS_LESS_SPACE__


	while(!dfs_stack.empty()) {
		#ifdef __DFS_LESS_SPACE__

		vertex_t u = dfs_stack.top().u;
		std::vector<vertex_t>::iterator &it = dfs_stack.top().it;
		if (it == G.adj_list[u.id].end()) {
			dfs_stack.pop();
			exit_time[u.id] = current_time;
		} else {
			vertex_t v = *it;
			if (!visited[v.id]) {
				visited[v.id] = 1;
				parent[v.id] = u;
				entry_time[v.id] = ++current_time;
				vert[current_time] = v;
				dfs_stack.push({v, G.adj_list[v.id].begin()});
			} else if (v != parent[u.id]) {
				if (entry_time[v.id] >= entry_time[u.id]) {
					back_edges[u.id].push_back(v);
				}
			}
			++it;
		}

		#else // more space

		vertex_t u = dfs_stack.top().u;
		bool in = dfs_stack.top().in;
		int pid = dfs_stack.top().pid;
		dfs_stack.pop();

		if (in && !visited[u.id]) {
			visited[u.id] = 1;
			entry_time[u.id] = ++current_time;
			parent[u.id] = {pid};
			vert[current_time] = u;
			dfs_stack.push({u, false, -1});

			//for (auto &v : G.adj_list[u.id]) {
			/*
			 * Iterating the edges in reverse order gives the same
			 * DFS-tree as the __LESS_SPACE__ variant, so it makes it
			 * easier to test
			 */
			for (auto it = G.adj_list[u.id].rbegin(); it != G.adj_list[u.id].rend(); ++it) {
				auto v = *it;
				dfs_stack.push({v, true, u.id});
			}

		} else if (!in) {
			exit_time[u.id] = current_time;
		} else {
			if (entry_time[pid] < entry_time[u.id]) {
				//fprintf(stderr, "%d(%d) -> %d(%d)\n", pid, entry_time[pid], u.id, entry_time[u.id]);
				back_edges[pid].push_back(u);
			}
		}

		#endif //__DFS_LESS_SPACE__
	}
}

int
two_connected_prop::find_ears(vertex_t u, int parent_ears)
{
	int curr_ears;
	if (!visited_ear_decomposition[u.id]) {
		ear_decomposition.push_back({});
		curr_ears = ear_decomposition.end() - 1 - ear_decomposition.begin();
	} else {
		curr_ears = parent_ears;
	}

	for (vertex_t &v : back_edges[u.id]) {
//		std::cerr << u.id << ' ' << v.id << std::endl;
		visited_ear_decomposition[u.id] = 1;
		path_t ear;
		vertex_t curr = v, prev = u;
		while(!visited_ear_decomposition[curr.id]) { //break on first vertex already visited.
			ear.push_back({prev, curr});
			visited_ear_decomposition[curr.id] = 1;
			prev = curr;
			curr = parent[curr.id];
		}
		//still have to push edge we used to get to first already
		//visited vertex to ear.
		ear.push_back({prev, curr});
		ear_decomposition[curr_ears].push_back(ear);
	}

	return curr_ears;
}

void
two_connected_prop::ear_decompose()
{
	int total_back_edges = 0;
	int current_time = 0;
	for (int i = 0; i < G.N; ++i) {
		if (!visited[i]) {
			dfs({i}, current_time);
		}
		total_back_edges += back_edges[i].size();
	}
	//std::cerr << G.N << ' ' << current_time << std::endl;
	assert(current_time == G.N);

	ear_decomposition.push_back({});
	ears_of[vert[1].id] = find_ears(vert[1], 0);
	for (int i = 2; i <= current_time; ++i) {
		int u = vert[i].id;
		int p = parent[u].id;
		ears_of[u] = find_ears({u}, ears_of[p]);
	}

	ear_decomposition.erase(ear_decomposition.begin());
}

#endif // __USE_DFS_TREE__
