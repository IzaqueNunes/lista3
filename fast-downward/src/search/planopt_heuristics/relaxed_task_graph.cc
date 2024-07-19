#include "relaxed_task_graph.h"

#include <iostream>
#include <vector>

using namespace std;

namespace planopt_heuristics {
RelaxedTaskGraph::RelaxedTaskGraph(const TaskProxy &task_proxy)
    : relaxed_task(task_proxy),
      variable_node_ids(relaxed_task.propositions.size()) {
    
    // Adicionando nó inicial e o nó objetivo no grafo, como tipo AND
    initial_node_id = graph.add_node(NodeType::AND);
    goal_node_id = graph.add_node(NodeType::AND);
    
    // Para cada proposição é criado um nó no grafo e seu ID é armazenado
    for (size_t i = 0; i < relaxed_task.propositions.size(); ++i) {
        variable_node_ids[i] = graph.add_node(NodeType::OR);
    }
    
    // Adicionando arestas de todos os operadores da tarefa relaxada
    for (const auto &op : relaxed_task.operators) {
        // Criando nó para o operador
        int operator_node_id = graph.add_node(NodeType::AND);
        
        // Adicionando arestas das proposições de pré-condição para o nó do operador
        for (PropositionID precond_id : op.preconditions) {
            graph.add_edge(variable_node_ids[precond_id], operator_node_id);
        }
        
        // Adicionando arestas do nó do operador para as proposições de efeito
        for (PropositionID effect_id : op.effects) {
            graph.add_edge(operator_node_id, variable_node_ids[effect_id]);
        }
    }
}

void RelaxedTaskGraph::change_initial_state(const GlobalState &global_state) {
    // Remove all initial edges that were introduced for relaxed_task.initial_state.
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
    // return true iff the goal is reachable in the relaxed task.

    graph.most_conservative_valuation();

    // Obtendo o nó de meta no gráfico
    const AndOrGraphNode &goal_node = graph.get_node(goal_node_id); 

    return goal_node.forced_true;
}

int RelaxedTaskGraph::additive_cost_of_goal() {

    // Computa a avaliação ponderada mais conservadora do grafo
    graph.weighted_most_conservative_valuation();

    // Retorna o valor h^add do nó objetivo
    const AndOrGraphNode &goal_node = graph.get_node(goal_node_id);
    return goal_node.additive_cost;
}

int RelaxedTaskGraph::ff_cost_of_goal() {
    // TODO: add your code for exercise 2 (e) here.
    return -1;
}

}
