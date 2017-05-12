#include <vector>
#include "graph.hpp"

bool vertex_t::operator==(const vertex_t &other) const
{
	return id == other.id;
}

bool vertex_t::operator!=(const vertex_t &other) const
{
	return id != other.id;
}

undirected_graph_t::undirected_graph_t (int N)
{
	this -> N = N;
	adj_list.resize(N);
}

void undirected_graph_t::add_edge (vertex_t u, vertex_t v)
{
	adj_list[u.id].push_back(v);
	adj_list[v.id].push_back(u);
}

