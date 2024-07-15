#include "relaxed_task_graph.h"

#include <iostream>
#include <vector>

using namespace std;

namespace planopt_heuristics {
RelaxedTaskGraph::RelaxedTaskGraph(const TaskProxy &task_proxy)
    : relaxed_task(task_proxy),
      variable_node_ids(relaxed_task.propositions.size()) {
               
    for (size_t i = 0; i < relaxed_task.propositions.sice(); i++){
	    variable_node_ids[i] = graph.add_node();
    }

    initial_node_id = graph.add_node();
    goal_node_id = graph.add_node();

    for (PropositionID id : relaxed_task.initial_state){
	    graph.add_edge(variable_node_ids[id], initial_node_id);
    }

    // Adicionar nós e arestas para os operadores
    for (const OperatorProxy &op : relaxed_task.operators) {
        int precondition_node_id = graph.add_node();
        for (PropositionID pre : op.get_preconditions()) {
            graph.add_edge(variable_node_ids[pre], precondition_node_id);
        }

        int effect_node_id = graph.add_node();
        for (PropositionID eff : op.get_effects()) {
            graph.add_edge(effect_node_id, variable_node_ids[eff]);
        }

        graph.add_edge(precondition_node_id, effect_node_id);
    }

    // Adicionar arestas de objetivo
    for (PropositionID goal : relaxed_task.goal) {
        graph.add_edge(variable_node_ids[goal], goal_node_id);
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

   // Vetor para armazenar se o nó foi visitado
    vector<bool> visited(graph.size(), false);

    // Função de busca em profundidade (DFS)
    function<bool(int)> dfs = [&](int node_id) -> bool {
        if (visited[node_id]) {
            return false;
        }
        visited[node_id] = true;

        // Se for o nó de objetivo, retornamos true
        if (node_id == goal_node_id) {
            return true;
        }

        // Buscar todos os nós conectados
        for (int neighbor_id : graph.get_neighbors(node_id)) {
            if (dfs(neighbor_id)) {
                return true;
            }
        }

        return false;
    };

    // Iniciar a busca a partir do nó inicial
    return dfs(initial_node_id);
}

int RelaxedTaskGraph::additive_cost_of_goal() {
    // Compute the weighted most conservative valuation of the graph and use it
    // to return the h^add value of the goal node.

    // TODO: add your code for exercise 2 (c) here.
    return -1;
}

int RelaxedTaskGraph::ff_cost_of_goal() {
    // TODO: add your code for exercise 2 (e) here.
    return -1;
}

}
