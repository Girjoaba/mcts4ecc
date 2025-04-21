#ifndef SHAREDMEMORY_H
#define SHAREDMEMORY_H

#include "cfg/config.h"
#include "matrix_game.h"

typedef struct {
    uint8_t *best_matrix;
    int bcs_count;
    int set_bits_count;
    int matrix_depth;
    int max_row_weight;

    int iteration;
    int game_count;
} SharedMemory;

/**
 * @brief Initializes the shared memory with the default starting values.
 * 
 * @param config used for finding the expected dimensions of the best matrix.
 * @param shared_memory object storing the shared_memory values.
 */
void init_shared_memory(Config *config, SharedMemory *shared_memory);

/**
 * @brief This function defines the metric we are trying to improve on.
 * When looking at different baselines (e.g. Li codes vs Dutta codes), we
 * only need to change this function.
 * 
 * @param config used for matrix dimensions.
 * @param shared_memory used to access already stored values.
 * @param state the current state we compare against the best state.
 * @param is_minimization if we are looking for a smaller or larger value.
 * 
 * @return bool, improves or not
 */
bool improves_metric(Config *config, SharedMemory *shared_memory, uint8_t *state, bool is_minimization);

/**
 * @brief Function used for the highscore optimization implementation.
 */
bool is_highscore(Config *config, SharedMemory *shared_memory, uint8_t *state);
void update_best_matrix(Config *config, SharedMemory *shared_memory, uint8_t *new_state);
void free_shared_memory(SharedMemory *shared_memory);

/**
 * @brief Logs found matrices.
 */
void log_matrix(Config *config, SharedMemory *shared_memory);


#endif