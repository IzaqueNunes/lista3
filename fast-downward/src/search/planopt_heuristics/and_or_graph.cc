#include "and_or_graph.h"

namespace planopt_heuristics {

NodeID AndOrGraph::add_node(NodeType type, int weight) {
    NodeID id = nodes.size();
    nodes.emplace_back(id, type, weight);
    return id;
}

void AndOrGraph::add_edge(NodeID from, NodeID to) {
    nodes[from].successor_ids.push_back(to);
    nodes[to].predecessor_ids.push_back(from);
}

void AndOrGraph::remove_edge(NodeID from, NodeID to) {
    auto &from_successors = nodes[from].successor_ids;
    from_successors.erase(std::remove(from_successors.begin(), from_successors.end(), to), from_successors.end());
    
    auto &to_predecessors = nodes[to].predecessor_ids;
    to_predecessors.erase(std::remove(to_predecessors.begin(), to_predecessors.end(), from), to_predecessors.end());
}

const AndOrGraphNode &AndOrGraph::get_node(NodeID id) const {
    return nodes[id];
}

AndOrGraphNode &AndOrGraph::get_node(NodeID id) {
    return nodes[id];
}

void AndOrGraph::set_direct_cost(NodeID id, int cost) {
    nodes[id].direct_cost = cost;
}

// Implement other methods like most_conservative_valuation, weighted_most_conservative_valuation, etc.

}
