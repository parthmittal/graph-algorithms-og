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
	assert(u < N  && v < N);
	adj_list[u].push_back({{u}, {v}, vid, weight});
	reverse(vid.begin(), vid.end());
	adj_list[v].push_back({{v}, {u}, vid, weight});
}

reduced_graph_t::reduced_graph_t()
	:N(0), adj_list(0)
{}

int *crs_row_t::begin()
{
	return start;
}

int *crs_row_t::end()
{
	return finish;
}

int crs_row_t::size()
{
	return std::distance(start, finish);
}

crs_t::crs_t (const std::vector< std::vector<vertex_t> > &adj)
	: start_idx(adj.size()), end_idx(adj.size())
{
	int N = adj.size();
	for (int u = 0; u < N; ++u) {
		int si = adj_collapse.size();
		for (auto &v : adj[u]) {
			adj_collapse.push_back(v.id);
		}
		int ei = adj_collapse.size();

		start_idx[u] = si;
		end_idx[u] = ei;
	}
}

crs_row_t crs_t::operator[](int u)
{
	int si = start_idx[u];
	int ei = end_idx[u];

	return crs_row_t{ &adj_collapse[0] + si, &adj_collapse[0] + ei };
}
