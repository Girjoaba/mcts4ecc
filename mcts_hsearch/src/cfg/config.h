#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include <math.h>
#include <time.h>

// Parallel
#include <omp.h>

#define INITIAL_CONFIG_SIZE 1024

typedef struct {
    int N;
    int K;
    int rows;
    int max_weight;
    
    float explore_coeff;
    float depth_coeff;
    int num_searches;
    int num_games;
    int experiment_runs;
    bool enable_shared_memory;
    bool enable_verbatim;

    FILE *input_file;
    FILE *output_file;
    double start_time;
    
    int num_threads;

    // Optimizations
    bool enable_action_space_splitting;
    int splitting_col;
    int left_weight;
    int right_weight;

    bool enable_highscore;

    float heating_rate;
    int heating_column_limit;
} Config;

char *readEntireFile(FILE *file);

/**
 * @brief Initializes the configuration object with the default values.
 * 
 * @param config the configuration object to be initialized.
 * @param argc the number of program arguments.
 * @param argv the program arguments.
 */
void init_config(Config *config, int argc, char *argv[]);

void print_config(Config *config);

/**
 * @brief Frees the configuration object.
 * 
 * @param config the configuration object to be freed.
 */
void free_config(Config *config);

int get_random_action(int n);
int kernighan_bit_count(int number);

#endif