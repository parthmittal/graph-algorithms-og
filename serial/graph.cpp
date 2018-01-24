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
	sig.push_back(0);
}

void
reduced_graph_t::add_edge(int u, int v, std::vector<int> vid, int weight)
{
	int x = std::min(u, v), y = std::max(u, v);
	auto it = best_cost.find({x, y});
	if (it == best_cost.end()) {
		best_cost[{x, y}] = weight;
	} else {
		it -> second = std::min(it -> second, weight);
	}

	assert(u < N  && v < N);
	adj_list[u].push_back({{u}, {v}, vid, weight});
	reverse(vid.begin(), vid.end());
	adj_list[v].push_back({{v}, {u}, vid, weight});
}

reduced_graph_t::reduced_graph_t()
	:N(0), adj_list(0)
{}
