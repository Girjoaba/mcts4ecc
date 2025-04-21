# Monte Carlo Tree Search for Rapid Discovery of Parity-Check Matrices in Linear Error Correction Codes

### Authors 
1. Andrei V. Girjoaba (agirjoaba@ethz.ch)
2. Arnold Meijster (a.meijster@rug.nl)
3. Dilek Dustegor (d.dustegor@rug.nl)

## Paper Reproducibility

## MCTS for H-matrix Discovery Descripion


## Python Visualizer

The visualizer is found in the `/visualizer` directory.

The project includes an **experimental** visualizer for the Encoding/Decoding pipeline of an ECC. We display the three benchmark codes and our Li N39K32 code. You can further test the correctness of the discovered H-matrix either with the visualizer or with the included `tests/test_bit_flips.py`.


### Setup 
``` bash
cd visualizer/
pip install -r requirements.txt 
```

### Visualizer
``` bash
python src/views/main_view.py
```

### Our Li N39K32 Error Test
``` bash
python tests/test_bit_flips.py
```

# References