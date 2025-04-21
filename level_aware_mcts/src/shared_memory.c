#include "shared_memory.h"

/**
 * @brief Initializes the shared memory with the default starting values.
 * 
 * @param config used for finding the expected dimensions of the best matrix.
 * @param shared_memory object storing the shared_memory values.
 */
void init_shared_memory(Config *config, SharedMemory *shared_memory) {
    shared_memory->best_matrix = calloc(config->N, sizeof(uint8_t));
    shared_memory->bcs_count = INT_MAX;
    shared_memory->set_bits_count = INT_MAX;
    shared_memory->matrix_depth = 0;
    shared_memory->max_row_weight = INT_MAX;

    shared_memory->iteration = 1;
    shared_memory->game_count = 1;
}

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
bool improves_metric(Config *config, SharedMemory *shared_memory, uint8_t *state, bool is_minimization) {
    int baseline = shared_memory->max_row_weight;
    // // Baseline: Dutta
    // int new_value = get_bad_4_cycles(config, state);
    
    // Baseline: Li
    int new_value = get_max_weight_row(config, state);

    if (is_minimization) {
        if (new_value < baseline) {
            shared_memory->bcs_count = get_bad_4_cycles(config, state);
            shared_memory->max_row_weight = new_value;
            shared_memory->set_bits_count = get_total_1s(config, state);
            return true;
        }
    } else {
        if (new_value > baseline) {
            shared_memory->bcs_count = get_bad_4_cycles(config, state);
            shared_memory->max_row_weight = new_value;
            shared_memory->set_bits_count = get_total_1s(config, state);
            return true;
        }
    }
    return false;
}

/**
 * @brief Function used for the highscore optimization implementation.
 */
bool is_highscore(Config *config, SharedMemory *shared_memory, uint8_t *state) {
    // // Highscore: Dutta
    // return shared_memory->bcs_count > get_bad_4_cycles(config, state);
    
    // Highscore: Li
    return shared_memory->max_row_weight > get_max_weight_row(config, state);
}

void update_best_matrix(Config *config, SharedMemory *shared_memory, uint8_t *new_state) {
    for (int i = 0; i < config->N; i++) {
        shared_memory->best_matrix[i] = new_state[i];
    }
}

/**
 * @brief Logs found matrices.
 */
void log_matrix(Config *config, SharedMemory *shared_memory) {
    // fprintf(config->output_file, "\n\nN%dK%d\n", config->N, config->K);
    // fprintf(config->output_file, "-----------------------------------\n");
    // fprintf(config->output_file, "Found @ iteration: %d ; game = %d \n", shared_memory->iteration, shared_memory->game_count + omp_get_thread_num());
    // fprintf(config->output_file, "After %.3f seconds\n", (double) (omp_get_wtime() - config->start_time));

    if (shared_memory->matrix_depth < config->N) {
        // fprintf(config->output_file, "~~~Longest Matrix Yet %d~~~\n", shared_memory->matrix_depth);
        // pretty_print_state(config, shared_memory->best_matrix);
        // fprintf(config->output_file, "\n");
    } else if (!(shared_memory->max_row_weight == INT_MAX)) {
        fprintf(config->output_file, "\n\nN%dK%d\n", config->N, config->K);
        fprintf(config->output_file, "-----------------------------------\n");
        fprintf(config->output_file, "Found @ iteration: %d ; game = %d \n", shared_memory->iteration, shared_memory->game_count + omp_get_thread_num());
        fprintf(config->output_file, "After %.3f seconds\n", (double) (omp_get_wtime() - config->start_time));
        fprintf(config->output_file, "# Bad Cycles: %d\n", shared_memory->bcs_count);
        fprintf(config->output_file, "# Set Bits:   %d\n", shared_memory->set_bits_count);
        fprintf(config->output_file, "# Max Row Weight: %d\n", get_max_weight_row(config, shared_memory->best_matrix));
        pretty_print_state(config, shared_memory->best_matrix);
        fprintf(config->output_file, "\n");
    } else {
        fprintf(config->output_file, "No matrix found yet... :(\n");
    }
}

void free_shared_memory(SharedMemory *shared_memory) {
    free(shared_memory->best_matrix);
}