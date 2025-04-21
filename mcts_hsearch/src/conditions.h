#ifndef CONDITIONS_H
#define CONDITIONS_H

#include "cfg/config.h"

// All implement this rule
bool check_0_cols(Config *config, uint8_t *state);
// All implement this rule
bool check_no_2_cycles(Config *config, uint8_t *state);
// Dutta & Jun implement this rule
bool check_no_3_cycles(Config *config, uint8_t *state);
// All implement this rule
bool check_no_forbidden_4_cycles(Config *config, uint8_t *state);

// Li implements
bool check_no_adj_single_syndromes(Config *config, uint8_t *state);

// ~~~~~ Statistics

// We use this one to run tests for the older partiy check matrices that are not AED or AEC
int get_forbidden_4_cycles(Config *config, uint8_t *state);
// We are trying to reduce the number of bad cycles
int get_bad_4_cycles(Config *config, uint8_t *state);
// To calculate miscorrection probability
int get_total_4_cycles(Config *config, uint8_t *state);

// To calculate benchmarks
int get_total_1s(Config *config, uint8_t *state);
bool check_max_weight_col_count(Config *config, uint8_t *state, int max_count, int weight);
int get_max_weight_row(Config *config, uint8_t *state);

#endif