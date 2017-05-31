#include <vector>
#include "graph.hpp"

#ifndef __EAR_DECOMPOSITION_DFS_hpp__
#define __EAR_DECOMPOSITION_DFS_hpp__

class two_connected_prop {
public:
	undirected_graph_t G;
	std::vector<int> visited;
	std::vector<vertex_t> parent;
	std::vector< std::vector<vertex_t> > back_edges;
	std::vector<int> entry_time, exit_time;
	std::vector<vertex_t> vert;

	std::vector<int> visited_ear_decomposition;
	std::vector< std::vector<path_t> > ear_decomposition; //each element is at least 2-edge-connected.
	std::vector<int> ears_of;

	two_connected_prop(const undirected_graph_t &G);

	#ifdef __DFS_LESS_SPACE__
	struct dfs_stack_elem_t {
		vertex_t u;
		std::vector<vertex_t>::iterator it;
	};
	#else // can use O(E) space
	struct dfs_stack_elem_t {
		vertex_t u;
		bool in;
		int pid;
	};
	#endif // __DFS_LESS_SPACE__

	void
	dfs(vertex_t root, int &current_time);

	int
	find_ears(vertex_t u, int parent_ears);

	void
	ear_decompose();
};

#endif // __EAR_DECOMPOSITION_DFS_hpp__
