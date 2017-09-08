#include <vector>
#include <cassert>
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
	: N(N), adj_list(N)
{}

void undirected_graph_t::add_edge (vertex_t u, vertex_t v)
{
	adj_list[u.id].push_back(v);
	adj_list[v.id].push_back(u);
}

void w_undirected_graph_t::add_vertex()
{
	++N;
	adj_list.push_back(std::vector<w_edge_t>(0));
}

void w_undirected_graph_t::add_edge(int u, int v, int weight)
{
	assert(u < N  && v < N);
	adj_list[u].push_back({{u}, {v}, weight});
	adj_list[v].push_back({{v}, {u}, weight});
}

w_undirected_graph_t::w_undirected_graph_t()
	:N(0), adj_list(0)
{}
