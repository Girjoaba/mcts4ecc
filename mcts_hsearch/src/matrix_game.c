#include "matrix_game.h"

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
uint8_t *get_start_state(Config *config) {
    uint8_t *state = calloc(config->N, sizeof(uint8_t));
    // Build Identity Part: Dutta & Jun
    for (int i = 0; i < config->rows; i++) {
        state[config->K + i] = 1 << i;
    }

    // // Build Identity Part: Li
    // for (int i = 0; i < config->rows; i++) {
    //     state[2 * i + 1] = 1 << i;
    // }

    return state;
}

void get_next_state(uint8_t *state, int column) {
    int i = 0;
    while (state[i] != 0) i++;
    state[i] = column;
}

int *get_legal_actions(Config *config, ActionSpace *action_space, uint8_t *state, int *legal_count) {
    *legal_count = 0;
    int *legal_actions = calloc(action_space->action_count, sizeof(int));
    int col_nr;
    bool unique_flag;
    for (int i = 0; i < action_space->action_count; i++) {
        unique_flag = true;
        col_nr = 0;
        while (state[col_nr] != 0) {
            if (action_space->avail_cols[i] == state[col_nr]) {
                unique_flag = false;
            }
            col_nr++;
        }
        if (unique_flag) {
            if (config->enable_action_space_splitting) { // Optimization
                if (col_nr < config->splitting_col) {
                    if (kernighan_bit_count(action_space->avail_cols[i]) == config->left_weight) {
                        legal_actions[(*legal_count)++] = action_space->avail_cols[i];
                    }
                } else if (kernighan_bit_count(action_space->avail_cols[i]) == config->right_weight) {
                    legal_actions[(*legal_count)++] = action_space->avail_cols[i];
                }
            } else {
                legal_actions[(*legal_count)++] = action_space->avail_cols[i];
            }
        }
    }
    return legal_actions;
}

uint8_t *copy_state(Config *config, uint8_t *state) {
    uint8_t *new_state = malloc(config->N*sizeof(uint8_t));
    for (int i = 0; i < config->N; i++) {
        new_state[i] = state[i];
    }
    return new_state;
}

void pretty_print_state(Config *config, uint8_t *state) {
    int current_col;
    for (int i = 0; i < config->N; i++) {
        current_col = state[i];
        i < 9 ? fprintf(config->output_file, "col %d : (", i+1) : fprintf(config->output_file, "col %d: (", i+1);
        for (int j = 0; j < config->rows; j++) {
            fprintf(config->output_file, "%d ", current_col & 1);
            current_col >>= 1;
        }
        fprintf(config->output_file, ")\n");
    }
    for (int i = 0; i < config->N; i++) {
        fprintf(config->output_file, "%d ", state[i]);
    }
    fprintf(config->output_file, "\n");

}

void free_state(uint8_t *state) {
    free(state);
}


bool is_different_state(Config *config, uint8_t *state1, uint8_t *state2) {
    for (int i = 0; i < config->N; i++) {
        if (state1[i] != state2[i]) return true;
    }
    return false;
}

int get_depth(Config *config, uint8_t *state) {
    int count = 0;
    for (int i = 0; i < config->N; i++) {
        if (state[i]) count++;
    } 
    return count;
}

/**
 * @brief Initializes the complete action space required by the code.
 * Each action (column) is represented by an integer number. We compute all 
 * the numbers up until our possible max and then counting their bits when 
 * represented in binary we decide what to keep in our action space. 
 * 
 * @param config used for accessing the required matrix size & space requirements.
 * @param action_space stores the create space in this object.
 */
void init_action_space(Config *config, ActionSpace *action_space) {
    action_space->avail_cols = malloc((1 << config->rows)*sizeof(uint8_t));
    action_space->action_count = 0;
    
    long complete_vector_space_bound = 1 << config->rows;
    unsigned int bit_count;
    int action_col;

    for (int i = 0; i < complete_vector_space_bound; i++) {
        action_col = i;
        bit_count = kernighan_bit_count(action_col);

        // Action Space: Dutta & Jun
        if (bit_count >= 3 && bit_count % 2 == 1 && bit_count <= config->max_weight) {
            action_space->avail_cols[action_space->action_count++] = i;
        }

        // // Action Space: Li
        // if (bit_count >= 2 && bit_count <= config->max_weight) {
        //     action_space->avail_cols[action_space->action_count++] = i;
        // }
    }

    action_space->avail_cols = realloc(action_space->avail_cols, action_space->action_count*sizeof(uint8_t));
}

void print_action_space(Config *config, ActionSpace *action_space) {
    for (int i = 0; i < action_space->action_count; i++) {
        printf("%d ", action_space->avail_cols[i]);
    }
    printf("\n");
}

void free_action_space(ActionSpace *action_space) {
    free(action_space->avail_cols);
}