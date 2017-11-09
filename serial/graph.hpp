#include <vector>
#include <algorithm>

#ifndef __GRAPH_hpp__
#define __GRAPH_hpp__

struct vertex_t {
	int id;

	bool operator==(const vertex_t &other) const;
	bool operator!=(const vertex_t &other) const;

	vertex_t(int id):
		id(id) {}

	vertex_t():
		id(0) {}
};

struct edge_t {
	vertex_t u, v;

	edge_t(vertex_t u, vertex_t v)
		: u(u), v(v) {}

	edge_t()
		: u(0), v(0) {}
};

struct w_edge_t {
	vertex_t u, v;
	int w;

	w_edge_t(vertex_t u, vertex_t v, int w)
		: u(u), v(v), w(w) {}

	w_edge_t()
		: u(0), v(0), w(0) {}
};

struct w_expanded_edge_t : w_edge_t {
	int p, q;
	std::vector<int> vids;
	w_expanded_edge_t(vertex_t u, vertex_t v, int p, int q,
			std::vector<int> vids, int w)
		: w_edge_t(u, v, w), p(p), q(q), vids(vids) {}
	w_expanded_edge_t()
		: w_edge_t(), p(0), q(0), vids(0) {}
};

typedef std::vector<edge_t> path_t;

struct undirected_graph_t {
	undirected_graph_t (int N);
	void add_edge (vertex_t u, vertex_t v);
		
	int N;
	std::vector< std::vector< vertex_t > > adj_list;
};

struct reduced_graph_t {
	reduced_graph_t ();
	void add_vertex();
	void add_edge(int u, int v, int p, int q, std::vector<int> vid, int weight);

	int N;
	std::vector< std::vector< w_expanded_edge_t > > adj_list;
};

struct rgraph_vinfo {
	int p, q;
	std::vector<int> dist; /* dist[i] <- distance of i from this vertex in Gr */
	std::vector<long long> num_paths; /* number of shortest paths */
	std::vector< std::vector<int> > parents;
	std::vector<int> inorder; /* vertices of Gr ordered by distance from this */
};

#endif // __GRAPH_hpp__
