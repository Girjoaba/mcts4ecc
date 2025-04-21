/**
 * @file mcts.c
 * @author Andrei Valentin Girjoaba (andrei.girjoaba@gmail.com)
 * @brief 
 * @version 1.1
 * @date 14-09-2024
 * 
 * @copyright Copyright (C) 2024  Andrei Valentin Girjoaba

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
 * 
 */

#include "matrix_game.h"
#include "shared_memory.h"
#include "mcts.h"
#include "cfg/parser.tab.h"

Config config;
void parseConfig();

/**
 * @brief The complete parity matrix search.
 * It uses the Monte-Carlo-Tree-Search to predict the best next column, until
 * the whole matrix is constructed.
 */
void serial_matrix_search(Config *config, SharedMemory *shared_memory, ActionSpace *action_space) {
    int action;
    float value;
    bool is_terminal;
    uint8_t *state;
    print_action_space(config, action_space);

    for (int game_count = 0; game_count < config->num_games; game_count++) {
        state = get_start_state(config);

        for (int i = 0; i < config->K; i++) {
            // Run MCTS (we drop the return)
            action = mcts_search(config, shared_memory, action_space, state);
            
            // Get the next action from the best saved matrix
            int action_pos = 0;
            while (state[action_pos] != 0) action_pos++;
            action = shared_memory->best_matrix[action_pos];

            // Play Action
            get_next_state(state, action);
            get_value_and_terminal(config, shared_memory, state, &value, &is_terminal);

            if (config->enable_verbatim && shared_memory->iteration % 3 == 0) {
                fprintf(config->output_file, "\n\n================\n");
                fprintf(config->output_file, "Intermediate Matrix %d:\n", shared_memory->iteration);
                pretty_print_state(config, state);
            }

            if (is_terminal) {
                printf("Game %d ended!\n", game_count);
                break;
            }
            printf("Iteration %d...\n", shared_memory->iteration);
            shared_memory->iteration++;

            // Hearting the Search Count
            if (i < config->heating_column_limit) config->num_searches *= config->heating_rate; 
        }
        shared_memory->iteration = 0;
        shared_memory->game_count++;
        free_state(state);
    }
}

/**
 * @brief It executes multiple games at the same on different threads. 
 * Works the same as the serial implementation.
 * @deprecated 
 */
void parallel_matrix_search(Config *config, SharedMemory *shared_memory, ActionSpace *action_space) {
    int game_count, iteration;
    print_action_space(config, action_space);
    // 
#pragma omp parallel default(none) firstprivate(config, shared_memory, action_space, game_count, iteration)
    {
        int action;
        float value;
        bool is_terminal;
        uint8_t *state;
#pragma omp for nowait
        for (game_count = 0; game_count < config->num_games; game_count++) {
            iteration = 0;
            state = get_start_state(config);

            for (int i = 0; i < config->K; i++) {
                // Execute MCTS (we drop return)
                action = mcts_search(config, shared_memory, action_space, state);
                
                // Get the next action from the best saved matrix
                int action_pos = 0;
                while (state[action_pos] != 0) action_pos++;
                action = shared_memory->best_matrix[action_pos];

                // Play action
                get_next_state(state, action);
                get_value_and_terminal(config, shared_memory, state, &value, &is_terminal);

                if (is_terminal) {
                    printf("Game %d ended!\n", game_count);
                    break;
                }
                shared_memory->iteration++;
                iteration++;
                
                // Hearting the Search Count
                if (i < config->heating_column_limit) config->num_searches *= config->heating_rate; 
        

                // Only 1 thread prints to stdout
                if (omp_get_thread_num() == 0) {
                    if (config->enable_verbatim && shared_memory->iteration % 3 == 0) {
                        fprintf(config->output_file, "\n\n================\n");
                        fprintf(config->output_file, "Intermediate Matrix %d:\n", shared_memory->iteration);
                        pretty_print_state(config, state);
                    }
                    printf("Game %d Iteration %d...\n", game_count, iteration);
                }
                fflush(config->output_file);
            }
            shared_memory->game_count++;
            free_state(state);
        }
    }
}

/**
 * @brief Performs a search w/ stopwatch.
 */
void search_performance(Config *config, SharedMemory *shared_memory, ActionSpace *action_space) {
    
    serial_matrix_search(config, shared_memory, action_space);

    double end = omp_get_wtime();

    double exec_time = (double) (end - config->start_time);
    fprintf(config->output_file, "\n\n=====================\nExecution time: %.3f seconds\n", exec_time);
}

/**
 * @brief Entry point for our program.
 */
int main(int argc, char *argv[]) {
    // Initialize Config
    srand(time(NULL));
    init_config(&config, argc, argv);
    char *input = readEntireFile(config.input_file);
    parseConfig(input);
    omp_set_num_threads(config.num_threads); // Set threads

    print_config(&config);

    // Create Game
    SharedMemory shared_memory;
    ActionSpace action_space;

    int og_num_searches = config.num_searches; 

    for (int d = 0; d < config.experiment_runs; d++) {
        config.num_searches = og_num_searches;
        init_action_space(&config, &action_space);
        init_shared_memory(&config, &shared_memory);
        config.start_time = omp_get_wtime();

        // Perform Search W. Performance Analysis
        search_performance(&config, &shared_memory, &action_space);
    }

    // I WANT TO BREAK FREE!!!
    free_config(&config);
    free_action_space(&action_space);
    free_shared_memory(&shared_memory);
    return 0;
}