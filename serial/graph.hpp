#include <vector>

#ifndef __GRAPH_hpp__
#define __GRAPH_hpp__

struct vertex_t {
	int id;

	bool operator==(const vertex_t &other) const;
	bool operator!=(const vertex_t &other) const;
};

struct edge_t {
	vertex_t u, v;
};

struct w_edge_t {
	vertex_t u, v;
	int w;
};

typedef std::vector<edge_t> path_t;

struct undirected_graph_t {
	undirected_graph_t (int N);
	void add_edge (vertex_t u, vertex_t v);
		
	int N;
	std::vector< std::vector< vertex_t > > adj_list;
};

struct w_undirected_graph_t {
	w_undirected_graph_t ();
	void add_vertex();
	void add_edge(int u, int v, int weight);

	int N;
	std::vector< std::vector< w_edge_t > > adj_list;
};

#endif // __GRAPH_hpp__
