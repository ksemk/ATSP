# Configuration File Documentation

This document provides detailed information about the configuration settings in the `config.json` file.

## Configuration Fields

### configurations
- **isMatrixRandom**: (boolean) Indicates whether to generate a random matrix (`true`) or read from a file (`false`).
- **inputFilePath**: (string) The path to the input file containing the distance matrix.
- **outputFilePath**: (string) The path to the output file where results will be saved.

### matrixGeneration
- **initial_size**: (integer) The initial size of the matrix for random generation.
- **final_size**: (integer) The final size of the matrix for random generation.
- **step**: (integer) The step size for incrementing the matrix size.
- **minValue**: (integer) The minimum value for the random distances.
- **maxValue**: (integer) The maximum value for the random distances.
- **symmetricity**: (integer) The percentage of symmetric edges in the random matrix.
- **asymRangeMin**: (integer) The minimum value for the asymmetry range.
- **asymRangeMax**: (integer) The maximum value for the asymmetry range.

### tabuSearch
- **maxIterations**: (integer) The maximum number of iterations for the Tabu Search algorithm.
- **tabuSize**: (integer) The size of the tabu list for the Tabu Search algorithm.

### Other Fields
- **numSamples**: (integer) The number of samples to run for each algorithm.
- **doBNB**: (boolean) Indicates whether to run the Branch and Bound algorithm.
- **doBF**: (boolean) Indicates whether to run the Brute Force algorithm.
- **doTabu**: (boolean) Indicates whether to run the Tabu Search algorithm.

## Example Configuration

```json
{
    "configurations": {
        "isMatrixRandom": false,
        "inputFilePath": "data/TSPLib_ATSP/ftv70.atsp",
        "outputFilePath": "results/results.csv",
        "matrixGeneration": {
            "initial_size": 5,
            "final_size": 15,
            "step": 1,
            "minValue": 10,
            "maxValue": 100,
            "symmetricity": 50,
            "asymRangeMin": -5,
            "asymRangeMax": 5
        },
        "tabuSearch": {
            "maxIterations": 100000,
            "tabuSize": 53
        },
        "numSamples": 50,
        "doBNB": false,
        "doBF": false,
        "doTabu": true
    }
}