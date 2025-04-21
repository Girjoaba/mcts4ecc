#include "conditions.h"

bool check_0_cols(Config *config, uint8_t *state) {
    int i;
    for (i = 0; i < config->N; i++) {
        if (state[i] == 0) {
            return true;
        }
    }
    return false;
}

bool check_no_2_cycles(Config *config, uint8_t *state) {
    int i, j;
    for (i = 0; i < config->N - 1; i++) {
        if (state[i] == 0) continue;
        for (j = i + 1; j < config->N; j++) {
            if (state[j] == 0) continue;
            if (state[j] == state[i]) {
                return false;
            }
        }
    }
    return true;
}

bool check_no_3_cycles(Config *config, uint8_t *state) {
    int i, j, k;
    for (i = 0; i < config->N - 2; i++) {
        if (state[i] == 0) continue;
        for (j = i + 1; j < config->N - 1; j++) {
            if (state[j] == 0) continue;
            for (k = j + 1; k < config->N; k++) {
                if (state[k] == 0) continue;
                if ((state[i] ^ state[j] ^ state[k]) == 0) {
                    return false;
                }
            }
        }
    }
    return true;
}

bool check_no_forbidden_4_cycles(Config *config, uint8_t *state) {
    int i, j, k, m;
    for (i = 0; i < config->N - 3; i++) {
        if (state[i] == 0) continue;
        j = i + 1;
        if (state[j] == 0) continue;

        for (k = j + 1; k < config->N - 1; k++) {
            if (state[k] == 0) continue;
            m = k + 1;
            if (state[m] == 0) continue;
            
            if ((state[i] ^ state[j] ^ state[k] ^ state[m]) == 0) {
                return false;
            }
        }
    }
    return true;
}

int get_forbidden_4_cycles(Config *config, uint8_t *state) {
    int counter = 0;
    int i, j, k, m;
    for (i = 0; i < config->N - 3; i++) {
        if (state[i] == 0) continue;
        j = i + 1;
        if (state[j] == 0) continue;

        for (k = j + 1; k < config->N - 1; k++) {
            if (state[k] == 0) continue;
            m = k + 1;
            if (state[m] == 0) continue;
            
            if ((state[i] ^ state[j] ^ state[k] ^ state[m]) == 0) {
                counter++;
            }
        }
    }
    return counter;
}

bool check_no_adj_single_syndromes(Config *config, uint8_t *state) {
    int i, j, k;
    for (i = 0; i < config->N - 1; i++) {
        if (state[i] == 0) continue;
        j = i + 1;
        if (state[j] == 0) continue;

        for (k = 0; k < config->N; k++) {
            if (state[k] == 0) continue;
            if (i == k || j == k) continue;
            if ((state[i] ^ state[j] ^ state[k]) == 0) {
                return false;
            }
        }
    }
    return true;
}


/**
 * @brief Get the bad 4 cycles object.
 *
 * ! It is important to realize that simply computing the 3 cases:
 *  - i, j adjacent; k, m non-adjacent
 *  - j, k adjacent; i, m non-adjacent
 *  - k, m adjacent; i, j non-adjacent
 * will result in more bad cycles. 
 *
 * We take care of the overlapping sitatuations (e.g. j = i + 1 and k = j + 1)
 * which falls into 2 cases by managing the for loops conditions.
 * 
 * @param config use config->N to iterate over the number of columns.
 * @param state the matrix for which we get the cycles.
 * @return int the number of bad cycles.
 */
int get_bad_4_cycles(Config *config, uint8_t *state) {
    int counter = 0;
    int i, j, k, m;
    // i, j adjacent case: j = i + 1
    for (i = 0; i < config->N - 3; i++) {
        if (state[i] == 0) continue;
        j = i + 1;
        if (state[j] == 0) continue;
        for (k = j + 1; k < config->N - 1; k++) {
            if (state[k] == 0) continue;
            for (m = k + 1; m < config->N; m++) {
                if (state[m] == 0) continue;

                if ((state[i] ^ state[j] ^ state[k] ^ state[m]) == 0) {
                    counter++;
                }
            }
        }
    }

    // j, k adjacent case: k = j + 1
    for (i = 0; i < config->N - 3; i++) {
        if (state[i] == 0) continue;
        for (j = i + 2; j < config->N - 2; j++) { // avoid j = i + 1 && k = j + 1 collision
            if (state[j] == 0) continue;
            k = j + 1;
            if (state[k] == 0) continue;
            for (m = k + 1; m < config->N; m++) {
                if (state[m] == 0) continue;
                
                if ((state[i] ^ state[j] ^ state[k] ^ state[m]) == 0) {
                    counter++;
                }
            }
        }
    }

    // k, m adjacent case: m = k + 1
    for (i = 0; i < config->N - 3; i++) {
        if (state[i] == 0) continue;
        for (j = i + 2; j < config->N - 2; j++) { // avoid j = i + 1 && m = k + 1 adjacent correct syndrome collision
            if (state[j] == 0) continue;
            for (k = j + 2; k < config->N - 1; k++) {   // avoid k = j + 1 && m = k + 1 collision
                if (state[k] == 0) continue;
                m = k + 1;
                if (state[m] == 0) continue;
                
                if ((state[i] ^ state[j] ^ state[k] ^ state[m]) == 0) {
                    counter++;
                }
            }
        }
    }

    return counter;
}

int get_total_4_cycles(Config *config, uint8_t *state) {
    int counter = 0;
    int i, j, k, m;
    for (i = 0; i < config->N - 3; i++)
        for (j = i + 1; j < config->N - 2; j++)
            for (k = j + 1; k < config->N - 1; k++)
                for (m = k + 1; m < config->N; m++) 
                    if ((state[i] ^ state[j] ^ state[k] ^ state[m]) == 0) 
                        counter++;

    return counter;
}

int kernighan_bit_count_again(int number) {
    unsigned int bit_count;
    for (bit_count = 0; number; bit_count++) {
        number &= number - 1; // clear the least significant bit
    }
    return bit_count;
}

int get_total_1s(Config *config, uint8_t *state) {
    int i, total_bits = 0;
    for (i = 0; i < config->N; i++) {
        total_bits += kernighan_bit_count_again(state[i]);
    }
    return total_bits;
}

bool check_max_weight_col_count(Config *config, uint8_t *state, int max_count, int weight) {
    int i, count = 0;
    for (i = 0; i < config->N; i++) {
        count += (weight == kernighan_bit_count_again(state[i])); 
    }
    return count <= max_count; 
}

int get_max_weight_row(Config *config, uint8_t *state) {
    int max_weight = 0;
    int weight;
    for (int row = 0; row < config->K; row++) {
        weight = 0;
        for (int col = 0; col < config->N; col++) {
            if (state[col] & (1 << row)) weight++;
        }
        if (weight > max_weight) {
            max_weight = weight;
        }
    }

    return max_weight;
}