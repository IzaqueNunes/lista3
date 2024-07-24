#include "relaxed_task_graph.h"

#include <iostream>
#include <vector>

#include <queue>
#include <unordered_set>

using namespace std;

namespace planopt_heuristics {
RelaxedTaskGraph::RelaxedTaskGraph(const TaskProxy &task_proxy)
    : relaxed_task(task_proxy),
      variable_node_ids(relaxed_task.propositions.size()) {
    /*
      TODO: add your code for exercise 2 (b) here. Afterwards
        - variable_node_ids[i] should contain the node id of the variable node for variable i
        - initial_node_id should contain the node id of the initial node
        - goal_node_id should contain the node id of the goal node
        - the graph should contain precondition and effect nodes for all operators
        - the graph should contain all necessary edges.
    */

    for( Proposition &prop : relaxed_task.propositions){
        NodeID id = graph.add_node(NodeType::OR);
        variable_node_ids[prop.id] = id;
    }

    initial_node_id = graph.add_node(NodeType::AND);

    for( PropositionID &prop : relaxed_task.initial_state){
        graph.add_edge(variable_node_ids[prop], initial_node_id);
    }

    goal_node_id = graph.add_node(NodeType::AND);

    for( PropositionID &prop : relaxed_task.goal){
        graph.add_edge(goal_node_id, variable_node_ids[prop]);
    }

    for( RelaxedOperator &op : relaxed_task.operators){
        NodeID op_node = graph.add_node(NodeType::AND);
        // NodeID op_node = graph.add_node(NodeType::AND, op.cost);

        // NodeID precondition_node = graph.add_node(NodeType::AND);
        // NodeID effect_node = graph.add_node(NodeType::AND);

        for( PropositionID &prop : op.preconditions){
            // graph.add_edge(precondition_node, variable_node_ids[prop]);
            graph.add_edge(op_node, variable_node_ids[prop]);            
        }

        for( PropositionID &prop : op.effects){
            // graph.add_edge(variable_node_ids[prop], effect_node);
            graph.add_edge(variable_node_ids[prop], op_node);
        }

        // graph.add_edge(op_node, precondition_node);
        // graph.add_edge(effect_node, op_node);
    }

}

void RelaxedTaskGraph::change_initial_state(const GlobalState &global_state) {
    // Remove all initial edges that where introduced for relaxed_task.initial_state.
    for (PropositionID id : relaxed_task.initial_state) {
        graph.remove_edge(variable_node_ids[id], initial_node_id);
    }

    // Switch initial state of relaxed_task
    relaxed_task.initial_state = relaxed_task.translate_state(global_state);

    // Add all initial edges for relaxed_task.initial_state.
    for (PropositionID id : relaxed_task.initial_state) {
        graph.add_edge(variable_node_ids[id], initial_node_id);
    }
}

bool RelaxedTaskGraph::is_goal_relaxed_reachable() {
    // Compute the most conservative valuation of the graph and use it to
    // return true iff the goal is reachable in the relaxed task.

    graph.most_conservative_valuation();
    return graph.get_node(goal_node_id).forced_true;
}

int RelaxedTaskGraph::additive_cost_of_goal() {
    // Compute the weighted most conservative valuation of the graph and use it
    // to return the h^add value of the goal node.

    // TODO: add your code for exercise 2 (c) here.
    graph.weighted_most_conservative_valuation();
    return graph.get_node(goal_node_id).additive_cost;
}

int RelaxedTaskGraph::ff_cost_of_goal() {
    // TODO: add your code for exercise 2 (e) here.
    int ff_cost = 0;
    
    graph.weighted_most_conservative_valuation();

    queue<AndOrGraphNode> q;
    unordered_set<int> visited;

    AndOrGraphNode n0 = graph.get_node(goal_node_id);

    q.push(n0);

    while(!q.empty()){
        AndOrGraphNode node = q.front();

        visited.insert(node.id);
        q.pop(); 

        ff_cost += node.direct_cost;

        if(node.type == NodeType::AND){

            for(NodeID &succ_id : node.successor_ids){

                if(visited.find(succ_id) == visited.end()){
                    q.push( graph.get_node(succ_id) );
                }
            }

        } else if(node.type == NodeType::OR){
            NodeID succ_id = node.achiever;
            if(visited.find(succ_id) == visited.end()){
                q.push( graph.get_node(succ_id) );
            }
        }
    }

    return ff_cost;
}

}
