#ifndef PLANOPT_HEURISTICS_AND_OR_GRAPH_H
#define PLANOPT_HEURISTICS_AND_OR_GRAPH_H

#include "../utils/hash.h"

#include <vector>
#include <unordered_map>

namespace planopt_heuristics {

using NodeID = int;
enum class NodeType { AND, OR };

struct AndOrGraphNode {
    NodeID id;
    NodeType type;
    bool forced_true;
    int num_forced_successors;
    std::vector<NodeID> successor_ids;
    std::vector<NodeID> predecessor_ids;

    // Only needed for exercises (d) and (f)
    int direct_cost;
    int additive_cost;
    NodeID achiever;

    AndOrGraphNode(NodeID id, NodeType type, int direct_cost = 0)
        : id(id), type(type), forced_true(false), num_forced_successors(0),
          direct_cost(direct_cost), additive_cost(0), achiever(-1) {
    }
};

class AndOrGraph {
    std::vector<AndOrGraphNode> nodes;
    utils::HashMap<std::vector<NodeID>, NodeID> and_node_ids;
    utils::HashMap<std::vector<NodeID>, NodeID> or_node_ids;
public:
    NodeID add_node(NodeType type, int weight = 0);
    void add_edge(NodeID from, NodeID to);
    void remove_edge(NodeID from, NodeID to);

    const AndOrGraphNode &get_node(NodeID id) const;
    AndOrGraphNode &get_node(NodeID id);  // Added non-const version for modification

    void set_direct_cost(NodeID id, int cost); // Added method to set direct cost

    void most_conservative_valuation();
    void weighted_most_conservative_valuation();
};

extern void test_and_or_graphs();
}
#endif
