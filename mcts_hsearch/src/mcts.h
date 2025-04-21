
#ifndef MCTS_H
#define MCTS_H

#include "cfg/config.h"
#include "matrix_game.h"
#include "shared_memory.h"

typedef struct MCTSNode {
    // Exploration Data
    int visit_count;
    int total_action_value;
    bool is_dead;
    // Game Data
    uint8_t *state;
    int action_taken;
    ActionSpace *action_space;
    // Pointers
    struct MCTSNode *parent;
    int children_count;
    struct MCTSNode **children;
} MCTSNode;

/**
 * @brief Check if the game terminated and get its reward value.
 * 
 * @param config used for different algorithm specific settings.
 * @param shared_memory used for highscore optimization implementation.
 * @param state the state being checked.
 * @param value return value for reward.
 * @param terminal return value for "is game finished".
 */
void get_value_and_terminal(Config *config, SharedMemory *shared_memory, uint8_t *state, float *value, bool *terminal);

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
int mcts_search(Config *config, SharedMemory *shared_memory, ActionSpace *action_space, uint8_t *state);


#endif