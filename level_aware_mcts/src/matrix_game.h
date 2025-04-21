#ifndef MATRIXGAME_H
#define MATRIXGAME_H

#include "cfg/config.h"
#include "conditions.h"

typedef struct {
    int action_count;
    uint8_t *avail_cols;
} ActionSpace;

/**
 * @brief Get the start state of the matrix game. The start state is a (N-K, K) zero matrix
 * with a (N-K, N-K) identity matrix appended to the right.
 *
 * Since it is a bit matrix, it is represented as a 1D integer array, where each number corresponds
 * to a bit column.
 * 
 * @param config contains the arguments for the size of the matrix.
 * @return uint8_t* the matrix representation as a 1D integer array.
 */
uint8_t *get_start_state(Config *config);
void free_state(uint8_t *state);

/**
 * @brief Initializes the complete action space required by the code.
 * Each action (column) is represented by an integer number. We compute all 
 * the numbers up until our possible max and then counting their bits when 
 * represented in binary we decide what to keep in our action space. 
 * 
 * @param config used for accessing the required matrix size & space requirements.
 * @param action_space stores the create space in this object.
 */
void init_action_space(Config *config, ActionSpace *action_space);

/**
 * ! behavior is off for both in the matrices are full
 */
void get_next_state(uint8_t *state, int column);
int *get_legal_actions(Config *config, ActionSpace *action_space, uint8_t *state, int *legal_count);
uint8_t *copy_state(Config *config, uint8_t *state);
int get_depth(Config *config, uint8_t *state);

bool is_different_state(Config *config, uint8_t *state1, uint8_t *state2);

/**
 * @brief Prints the state under the bits format.
 * 
 * @param config contains the length of the matrix and the output file.
 * @param state the matrix being printed.
 */
void pretty_print_state(Config *config, uint8_t *state);

/**
 * @brief Prints all vectors in the action space to the specified output.
 * 
 * @param config contains the output file.
 * @param action_space the action space.
 */
void print_action_space(Config *config, ActionSpace *action_space);

/**
 * @brief Frees the vector containing all the available columns.
 * 
 * @param action_space the action space that contains the vector of available columns.
 */
void free_action_space(ActionSpace *action_space);

#endif