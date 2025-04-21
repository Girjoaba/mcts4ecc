# Monte Carlo Tree Search for Rapid Discovery of Parity-Check Matrices in Linear Error Correction Codes

### Authors 
1. Andrei V. Girjoaba (agirjoaba@ethz.ch)
2. Arnold Meijster (a.meijster@rug.nl)
3. Dilek Dustegor (d.dustegor@rug.nl)

This repository is meant to be accompanied by the paper *Monte Carlo Tree Search for Rapid Discovery of Parity-Check Matrices in Linear Error Correction Codes*. 

# Paper Reproducibility

All results attributed to the MCTS-based parity check matrix (H-matrix) search can be found in `mcts_hsearch/`.

## Requirements

1. We use GNU Bison [1] for the configuration file parser we use to enable/disable/setup various optimizations for our algorithm.
2. A C compiler. The gcc compiler is set by default in the `mcts_hsearch/makefile` file. The compile settings and flags can be changed there.

## Running the Search Algorithm
``` bash
cd mcts_hsearch
make
./search input/{your_config_file}.cfg output/{your_output_file}.txt
```

The search takes 2 parameters:
1. The optimization configuration file. A few examples are provided in `mcts_hsearch/input/`.
2. An output file that logs the H-matrix discovered with a few additional statistics such as time, iterations and the count of target metrics.

Currently the **search is configured for finding the Li H-matrices.**
1. `./search input/liN39K32.cfg output/liN39K32.txt` finds the N39K32 codes.
2. `./search input/liN72K64.cfg output/liN72K64.txt` finds the N72K64 codes.



## Setting Up Optimizations
The file `mcts_hsearch/input/mcts.cfg.example` contains an example of how to write a configuration file with each optimization annotated.

The optimizations are the same as the ones descirbed in the main paper. The *Parallel Settings* are still WIP and should be left to their default value.

## Results

Our results can be found in:
1. `mcts_hsearch/output/final_results/` where we show our found H-matrices for the Dutta [2], Jun [3] and Li [4] codes.
2. `mcts_hsearch/output/splitting/` where we logged the execution time when the *Column Space Splitting* optimization is enabled and disabled.
3. `mcts_hsearch/output/optimizations/` where we logged the information found in the *Figure Comparison of MCTS Optimizations*. The experiment is aggregated across 10 runs and the execution time can be found by searching for "Execution time". The matrix right above contains the bad cycle target metric.

## Testing

We include a testing framework used to verify the correctness of our discovered H-matrices and our algorithm. We check that the strict conditions correctly apply to the orignal parity check matrices. Run the tests by calling:
``` bash
make
./test
```
More testing is also done by our Python Visualizer, outside the scope of the main paper.

## Changing the Search from Li codes to Dutta/Jun Codes

The MCTS-based search is configured for the Dutta/Jun codes on the `dutta_jun_search` branch.

To configure the MCTS-based search for a different code can be done by simply changing the strict conditions and the target metrics.

The Li/Dutta alternatives are commented out. 

### Shared Memory Structure `src/shared_memory.c`
``` C
// // Baseline: Dutta
// int new_value = get_bad_4_cycles(config, state);

// Baseline: Li
int new_value = get_max_weight_row(config, state);
```

### Monte Carlo Tree Search `src/mcts.c`
``` C
bool check_valid(Config *config, SharedMemory *shared_memory, uint8_t *state) {
    if (config->enable_highscore) {
            return (
            check_no_2_cycles(config, state)
            // && check_no_3_cycles(config, state)              // Dutta & Jun: Condition
            && check_no_adj_single_syndromes(config, state)     // Li: Condition
            && check_no_forbidden_4_cycles(config, state)       
            && is_highscore(config, shared_memory, state)
    );
    }

    return (
            check_no_2_cycles(config, state)
            // && check_no_3_cycles(config, state)           // Dutta & Jun: Condition
            && check_no_adj_single_syndromes(config, state)  // Li: Condition 
            && check_no_forbidden_4_cycles(config, state) 
    );
}
```

### Action Space `src/matrix_game.c`
``` C
for (int i = 0; i < complete_vector_space_bound; i++) {
    action_col = i;
    bit_count = kernighan_bit_count(action_col);

    // // Action Space: Dutta & Jun
    // if (bit_count >= 3 && bit_count % 2 == 1 && bit_count <= config->max_weight) {
    //     action_space->avail_cols[action_space->action_count++] = i;
    // }

    // Action Space: Li
    if (bit_count >= 2 && bit_count <= config->max_weight) {
        action_space->avail_cols[action_space->action_count++] = i;
    }
}
```


# Python Visualizer

The visualizer is found in the `/visualizer` directory.

The project includes an **experimental** visualizer for the Encoding/Decoding pipeline of an ECC. We display the three benchmark codes and our Li N39K32 code. You can further test the correctness of the discovered H-matrix either with the visualizer or with the included `tests/test_bit_flips.py`.


## Setup 
``` bash
cd visualizer/
pip install -r requirements.txt 
```

## Visualizer
``` bash
python src/views/main_view.py
```

## Running a Pattern Based Bit Flip Test
``` bash
python tests/test_bit_flips.py
```

# References
[1]: GNU Project. (n.d.). GNU Bison. https://www.gnu.org/software/bison/ <br>
[2]: Dutta, A., & Touba, N. A. (2007, May). Multiple bit upset tolerant memory using a selective cycle avoidance based SEC-DED-DAEC code. In 25th IEEE VLSI Test Symposium (VTS'07) (pp. 349-354). IEEE. <br> 
[3]: Jun, H., & Lee, Y. (2014). SEC—DED—DAEC Codes for On-chip Memory Protection against Multiple Cell Upsets. International Journal of Computer and Information Technology. <br> 
[4]: Li, J., Reviriego, P., Xiao, L., Liu, Z., Li, L., & Ullah, A. (2019). Low delay single error correction and double adjacent error correction (SEC-DAEC) codes. Microelectronics Reliability, 97, 31-37.