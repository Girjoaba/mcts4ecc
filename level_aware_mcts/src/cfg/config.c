#include "config.h"

char *readEntireFile(FILE *file) {

    // Allocate Memory
    int config_input_size = INITIAL_CONFIG_SIZE;
    char* input = malloc(sizeof(char)*config_input_size);
    if (input == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // Read input
    int index = 0;
    char c;
    while ((c = fgetc(file)) != EOF) {
        if (index == config_input_size) {
            config_input_size *= 2;
            input = realloc(input, config_input_size*sizeof(char));
            if (input == NULL) {
                fprintf(stderr, "Memory allocation failed\n");
                exit(EXIT_FAILURE);
            }
        }
        input[index++] = c;
    }

    // Null-terminate the string
    if (index == config_input_size) {
        input = realloc(input, (config_input_size + 1) * sizeof(char));
        if (input == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
    }
    input[index] = '\0';

    return input;
}


void parse_arguments(Config *config, int argc, char *argv[]) {

    // Get output file
    if (argc != 3) {
        fprintf(stderr, "Must specify first an input config file and an output file.\n");
        exit(EXIT_FAILURE);
    }

    const char *config_path = argv[1];
    FILE *cfg_file = fopen(config_path, "r");
    if (cfg_file == NULL) {
        perror("Could not open the configuration file!\n");
        exit(EXIT_FAILURE);
    }
    config->input_file = cfg_file;

    const char *file_path = argv[2];
    FILE *output_file = fopen(file_path, "w");
    if (output_file == NULL) {
        perror("Could not open the output file!\n");
        exit(EXIT_FAILURE);
    }
    config->output_file = output_file;

}

void init_config(Config *config, int argc, char *argv[]) {
    parse_arguments(config, argc, argv);

    config->N = 39;
    config->K = 32;
    config->max_weight = 3;
    config->rows = config->N - config->K;
    config->explore_coeff = 2.0;
    config->depth_coeff = (double)(1) / config->K;

    config->num_searches = 1000; 
    config->num_games = 10;

    config->experiment_runs = 1;
    config->enable_shared_memory = true;
    config->enable_verbatim = false;

    config->num_threads = 1;
    config->start_time = omp_get_wtime();

    config->enable_action_space_splitting = false;
    config->splitting_col = 37;
    config->left_weight = 2;
    config->right_weight = 3;

    config->enable_highscore = false;

    config->heating_rate = 1.0;
    config->heating_column_limit = 0;
}

void print_config(Config *config) {
    fprintf(config->output_file, "\n~~~~~~~~~~~ Configuration Settings ~~~~~~~~~\n\n");

    fprintf(config->output_file, "N: %d\n", config->N);
    fprintf(config->output_file, "K: %d\n", config->K);
    fprintf(config->output_file, "rows: %d\n", config->rows);
    fprintf(config->output_file, "max_weight: %d\n", config->max_weight);
    fprintf(config->output_file, "explore_coeff: %f\n", config->explore_coeff);
    fprintf(config->output_file, "depth_coeff: %f\n", config->depth_coeff);
    fprintf(config->output_file, "num_searches: %d\n", config->num_searches);
    fprintf(config->output_file, "num_games: %d\n", config->num_games);
    fprintf(config->output_file, "experiment_runs: %d\n", config->experiment_runs);
    fprintf(config->output_file, "enable_shared_memory: %d\n", config->enable_shared_memory);
    fprintf(config->output_file, "enable_verbatim: %d\n", config->enable_verbatim);
    fprintf(config->output_file, "num_threads: %d\n", config->num_threads);
    fprintf(config->output_file, "enable_action_space_splitting: %d\n", config->enable_action_space_splitting);
    fprintf(config->output_file, "splitting_col: %d\n", config->splitting_col);
    fprintf(config->output_file, "left_weight: %d\n", config->left_weight);
    fprintf(config->output_file, "right_weight: %d\n", config->right_weight);
    fprintf(config->output_file, "enable_highscore: %d\n", config->enable_highscore);
    fprintf(config->output_file, "heating_rate: %f\n", config->heating_rate);
    fprintf(config->output_file, "heating_column_limit: %d\n", config->heating_column_limit);

    fprintf(config->output_file, "\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
}

void free_config(Config *config) {
    fclose(config->input_file);
    fclose(config->output_file);
}

int get_random_action(int n) {
    return rand() % (n + 1);
}

int kernighan_bit_count(int number) {
    unsigned int bit_count;
    for (bit_count = 0; number; bit_count++) {
        number &= number - 1; // clear the least significant bit
    }
    return bit_count;
}


