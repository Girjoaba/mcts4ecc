#include "mcts.h"

// ---------------------------------------------------------- |
// ---------------------- Node Functions -------------------- |
// ---------------------------------------------------------- |

/**
 * @brief Initialize the root node of the MCTS tree.
 * 
 * @param action_space the space of all possible actions.
 * @param init_state the state added as the root state.
 * @return MCTSNode* the root node.
 */
MCTSNode *init_MCTSNode_root(ActionSpace *action_space, uint8_t *init_state) {
    MCTSNode* root = (MCTSNode*)malloc(sizeof(MCTSNode));
    // Default Values
    root->visit_count = 0;
    root->total_action_value = 0;
    root->is_dead = false;
    root->action_space = action_space;

    // Updated Values
    root->state = init_state;
    root->action_taken = 0;

    root->children_count = 0;
    root->children = (MCTSNode**)malloc((action_space->action_count)*sizeof(MCTSNode*));
    root->parent = NULL;

    return root;
}

/**
 * @brief Creates a child. The edge is the action taken.
 * 
 * @param config the configuration structure used for the matrix size.
 * @param parent the parent node.
 * @param action_taken the added edge.
 * @return MCTSNode* the child node.
 */
MCTSNode *init_MCTSNode_child(Config *config, MCTSNode *parent, int action_taken) {
    MCTSNode* child = (MCTSNode*)malloc(sizeof(MCTSNode));
    
    uint8_t *child_state = copy_state(config, parent->state);
    get_next_state(child_state, action_taken);
    
    // Default Values
    child->visit_count = 0;
    child->total_action_value = 0;
    child->is_dead = false;
    child->action_space = parent->action_space;

    // Updated Values
    child->state = child_state;
    child->action_taken = action_taken;

    child->children_count = 0;
    child->children = (MCTSNode**)malloc((parent->action_space->action_count)*sizeof(MCTSNode*));
    child->parent = parent;

    return child;
}

/**
 * @brief Frees the MCTS.
 * 
 * @param node it frees the MCTS from the node.
 */
void freeMCTS(MCTSNode *node) {
    if (node == NULL) {
        return;
    }
    
    // Free all children
    for (int i = 0; i < node->children_count; i++) {
        freeMCTS(node->children[i]);
    }
    free(node->children);
    
    // Keep the state of the root node.
    if (node->parent != NULL) {
        free(node->state);
    }
    free(node);
}

/**
 * @brief Appends a child to a node. Increments the children count.
 * 
 * @param parent the parent to which the child is added.
 * @param child the added child.
 */
void append_child(MCTSNode *parent, MCTSNode *child) {
    parent->children[parent->children_count++] = child;
}

/**
 * @brief Gets the Upper Confidence Bound function. It is used during the
 * selection phase of the MCTS.
 * 
 * @param config used for the explore_coeff.
 * @param parent parent node used for calculations.
 * @param child child node used for calculations.
 * @return float the UCB value.
 */
float get_UCB(Config *config, MCTSNode *parent, MCTSNode *child) {
    float value_score;
    if (child->visit_count == 0) {
        value_score = 0.5;
    } else {
        // value_score = ((child->total_action_value / child->visit_count) + 1) / 3;
        value_score = ((child->total_action_value / child->visit_count) + 1) / 2;
    }
    float pb_c = sqrt( log(parent->visit_count)/(child->visit_count) );
    return value_score + config->explore_coeff * pb_c;
}

/**
 * @brief Checks if it has children.
 * 
 * @param node the node.
 * @return true 
 * @return false 
 */
bool has_children(MCTSNode *node) {
    return node->children_count != 0;
}

/**
 * @brief Using conditions that are code specific (Li codes use
 * different conditions than Dutta), we define if the state is valid or not.
 * This resolved the "is_game_terminated" statement.
 * 
 * @param config used for various algorithm specific settings.
 * @param shared_memory used for the highscore optimization implementation.
 * @param state the state being checked.
 * 
 * @return the state is valid or not.
 */
bool check_valid(Config *config, SharedMemory *shared_memory, uint8_t *state) {
    if (config->enable_highscore) {
            return (
            check_no_2_cycles(config, state)
            // && check_no_3_cycles(config, state)              // Dutta & Jun: Condition
            && check_no_adj_single_syndromes(config, state)     // Li: Condition
            && check_no_forbidden_4_cycles(config, state)       
            && is_highscore(config, shared_memory, state)
    );
    }

    return (
            check_no_2_cycles(config, state)
            // && check_no_3_cycles(config, state)           // Dutta & Jun: Condition
            && check_no_adj_single_syndromes(config, state)  // Li: Condition 
            && check_no_forbidden_4_cycles(config, state) 
    );
}

/**
 * @brief Check if the game terminated and get its reward value.
 * 
 * @param config used for different algorithm specific settings.
 * @param shared_memory used for highscore optimization implementation.
 * @param state the state being checked.
 * @param value return value for reward.
 * @param terminal return value for "is game finished".
 */
void get_value_and_terminal(Config *config, SharedMemory *shared_memory, uint8_t *state, float *value, bool *terminal) {
    if (check_valid(config, shared_memory, state)) {
        if (check_0_cols(config, state)) {
            *value = 1;
            *terminal = false;               
        } else {
            *value = 1;
            *terminal = true;
        }
        return;
    }

    *value = -1;
    *terminal = true;
}

/**
 * @brief Selects the best node according to the UCB.
 * 
 * @param config used for getting the UCB.
 * @param node the parent node.
 * @return MCTSNode* the best child.
 */
MCTSNode *select(Config *config, MCTSNode *node) {
    MCTSNode *best_child = NULL;
    float best_ucb = -10000;
    float ucb;

    for (int i = 0; i < node->children_count; i++) {
        if (!node->children[i]->is_dead) {
            ucb = get_UCB(config, node, node->children[i]);
            if (ucb > best_ucb) {
                best_child = node->children[i];
                best_ucb = ucb;
            }
        }
    }
    if (best_child == NULL) {
        node->is_dead = true;
        // for (int i = 0; i < node->children_count; i++) {
        //     freeMCTS(node->children[i]);
        //     free(node->children[i]);
        // }
    }
    return best_child;
}

/**
 * @brief RAVE explansion using all possible actions. Exapnds a node with all possible actions.
 * 
 * @param config used for the matrix size.
 * @param node the expanded node.
 */
void expand(Config *config, MCTSNode *node) {
    int action_count;
    int *legal_actions = get_legal_actions(config, node->action_space, node->state, &action_count);
    for (int i = 0; i < action_count; i++) {
        MCTSNode *child = init_MCTSNode_child(config, node, legal_actions[i]);
        append_child(node, child);
    }
    free(legal_actions);
}

/**
 * @brief Simulates the random rollout after expansion.
 * 
 * @param config used for distinct settings and matrix properties.
 * @param shared_memory saves the best found matrices (won games) to the shared memory.
 * @param node the node from which we simulate.
 * @return float the value of the simulation.
 */
float simulate(Config *config, SharedMemory *shared_memory, MCTSNode *node) {
    float value;
    bool is_terminal;
    get_value_and_terminal(config, shared_memory, node->state, &value, &is_terminal);

    if (is_terminal) {
        return value;
    }

    uint8_t *rollout_state = copy_state(config, node->state);
    int legal_action_count, *legal_actions, action_idx;
    while (true) {
        legal_actions = get_legal_actions(config, node->action_space, rollout_state, &legal_action_count);
        action_idx = get_random_action(legal_action_count);
        action_idx = (action_idx + omp_get_thread_num()) % legal_action_count; // Parallel Different Games
        get_next_state(rollout_state, legal_actions[action_idx]);
        get_value_and_terminal(config, shared_memory, rollout_state, &value, &is_terminal);
        if (is_terminal) {
            if (config->enable_shared_memory && value > 0) {
#pragma omp critical
                {
                    if (improves_metric(config, shared_memory, rollout_state, true)
                            && is_different_state(config, shared_memory->best_matrix, rollout_state)) {
                        update_best_matrix(config, shared_memory, rollout_state);
                        shared_memory->matrix_depth = config->N;
                        log_matrix(config, shared_memory);
                    }
                }
            } else if (get_depth(config, rollout_state) > shared_memory->matrix_depth) {
                update_best_matrix(config, shared_memory, rollout_state);
                shared_memory->matrix_depth = get_depth(config, rollout_state);
                log_matrix(config, shared_memory);
            }
            free(legal_actions);
            free(rollout_state);
            return value;
        }

        free(legal_actions);
    }
}

/**
 * @brief Backpropagates the value of the simulation up the tree as part of the MCTS.
 * 
 * @param config used for the depth_coeff.
 * @param node the node from where we propagate.
 * @param value the value we propagate.
 */
void backpropagate(Config *config, MCTSNode *node, float value) {
    node->total_action_value += value;
    node->visit_count += 1;

    if (node->parent != NULL) {
        backpropagate(config, node->parent, value + config->depth_coeff);
    }
}

/**
 * @brief Performs the 4 main MCTS phases:
 * 1. Selection
 * 2. Expansion
 * 3. Simulation
 * 4. Backpropagation
 * 
 * Each iteration through the phases defines a search. The number of performed searches
 * is predefined and saved in config.
 * 
 * @param config used for different algorithm specific settings.
 * @param shared_memory used for saving the best results
 * @param action_space used to perform exisitng moves/actions
 * @param state starting state
 * 
 * @return the action most performed
 */
int mcts_search(Config *config, SharedMemory *shared_memory, ActionSpace *action_space, uint8_t *state) {
    MCTSNode *root = init_MCTSNode_root(action_space, state);
    MCTSNode *node;
    float value;
    bool is_terminal;
    int new_metric_value;
    for (int search_count = 0; search_count < config->num_searches; search_count++) {
        node = root;
    
        // 1. Selection + 2. Expansion
        while (node != NULL && has_children(node)) {
            node = select(config, node);
        }
        if (node == NULL) continue;

        get_value_and_terminal(config, shared_memory, node->state, &value, &is_terminal);

        // 4. Backpropagate if you can't expand anymore
        if(is_terminal) {
            if (value < 0) {
                node->is_dead = true;
                if (get_depth(config, node->state) > shared_memory->matrix_depth) {
                    update_best_matrix(config, shared_memory, node->state);
                    shared_memory->matrix_depth = get_depth(config, node->state);
                    log_matrix(config, shared_memory);
                }
                // freeMCTS(node);   // Gives unaligned tcache chunck detected: Aborted()
            } else if (config->enable_shared_memory) {
#pragma omp critical
                {
                    if (improves_metric(config, shared_memory, node->state, true)
                            && is_different_state(config, shared_memory->best_matrix, node->state)) {
                        update_best_matrix(config, shared_memory, node->state);
                        shared_memory->matrix_depth = config->N;
                        log_matrix(config, shared_memory);
                    }
                }
            }
            backpropagate(config, node, value);
        } else {
            // 3. Expand + 4. Backpropagate
            expand(config, node);
            for (int child_idx = 0; child_idx < node->children_count; child_idx++) {
                value = simulate(config, shared_memory, node->children[child_idx]);
                backpropagate(config, node, value);
            }
        }
    }

    // Get most visited action.
    int best_action = 0;
    int max_visit_count = 0;
    for (int child_idx = 0; child_idx < root->children_count; child_idx++) {
        if (root->children[child_idx]->visit_count > max_visit_count) {
            max_visit_count = root->children[child_idx]->visit_count;
            best_action = root->children[child_idx]->action_taken;
        }
    }
    freeMCTS(root);
    return best_action;
}
