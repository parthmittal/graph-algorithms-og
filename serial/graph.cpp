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

void reduced_graph_t::add_vertex()
{
	++N;
	adj_list.push_back(std::vector<w_expanded_edge_t>(0));
}

void
reduced_graph_t::add_edge(int u, int v, int p, int q, std::vector<int> vid,
		int weight)
{
	assert(u < N  && v < N);
	adj_list[u].push_back({{u}, {v}, p, q, vid, weight});
	reverse(vid.begin(), vid.end());
	adj_list[v].push_back({{v}, {u}, q, p, vid, weight});
}

reduced_graph_t::reduced_graph_t()
	:N(0), adj_list(0)
{}
