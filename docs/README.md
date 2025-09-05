# ATSP Solver — Branch and Bound, Tabu Search, and Genetic Algorithm

Robust C++ implementation of multiple algorithms for the Asymmetric Traveling Salesman Problem (ATSP), with experiment automation, configuration via JSON, and Python tooling for results analysis and plots. Built as a compact, portfolio-quality project showcasing algorithmic engineering, performance measurement, and clean code structure.

## Highlights

- Multiple approaches: Brute Force (baseline), Branch and Bound (exact), Tabu Search (metaheuristic), Genetic Algorithm (metaheuristic)
- TSPLIB-ATSP support and random instance generation
- JSON-driven experiments and reproducibility
- Performance plots and tables already generated in `raport/` and `results/`
- Clean CMake setup, header-only JSON (nlohmann), simple to run on Windows

## Tech stack

- C++17, CMake
- Algorithms: Brute Force, Branch and Bound, Tabu Search, Genetic Algorithm
- Data/config: TSPLIB-style files, JSON config (`config/config.json`, `config/ga_config.json`)
- Analysis: Python (matplotlib, numpy) with scripts in `python/`

## Repository structure

- `src/` — entry point and wiring (`main.cpp`)
- `include/Algorithms/` — implementations: `BruteForce.h`, `BranchAndBound.h`, `TabuSearch.h`, `GeneticAlgo.h`
- `include/` — core types (`matrix.h`, `node.h`, `util.h`)
- `config/` — config files and docs (`config.json`, `ga_config.json`, `config_documentation.md`)
- `data/` — input instances (TSPLIB and test sets)
- `results/`, `raport/` — experiment outputs, plots, and PDF reports
- `python/` — plotting and GA exploration scripts

## Algorithms implemented

- Brute Force — exact baseline for small N
- Branch and Bound — exact solver with lower-bound pruning
- Tabu Search — local search with tabu list, swap moves, iteration and tabu size controls
- Genetic Algorithm — population-based search with selection, crossover, mutation, and diversity control (configurable via JSON)

Key headers: `include/Algorithms/BranchAndBound.h`, `BruteForce.h`, `TabuSearch.h`, `GeneticAlgo.h`.

## Build and run

Prerequisites: CMake and a C++17 compiler (e.g., MinGW-w64 on Windows). Python is optional for plots.

Quick start (out-of-source build):

1) Configure and build
- Create a `build/` directory and run CMake generate + build.

2) Run the executable
- The app reads `config/config.json` by default from the working directory path assumptions in `main.cpp`.

If you prefer VS Code tasks, a default C/C++ build task is present, or use your own CMake presets.

## Configuration

Main run-time options live in `config/config.json` (see `config/config_documentation.md` for full reference). The most relevant toggles:

- `isMatrixRandom`: true to generate random matrices, false to load from file
- `inputFilePath`: e.g., `data/TSPLib_ATSP/ftv70.atsp`
- `doBNB`, `doBF`, `doTabu`, `doGA`: enable algorithms
- `tabuSearch.maxIterations`, `tabuSearch.tabuSize`: Tabu Search parameters
- `GA.GAPath`: path to GA parameters JSON (e.g., `config/ga_config.json`)

Example (excerpt):

```
{
  "configurations": {
    "isMatrixRandom": false,
    "inputFilePath": "data/TSPLib_ATSP/ftv70.atsp",
    "outputFilePath": "results/results.csv",
    "tabuSearch": { "maxIterations": 100000, "tabuSize": 55 },
    "doBNB": false,
    "doBF": false,
    "doTabu": false,
    "doGA": true,
    "GA": { "GAPath": "../config/ga_config.json" }
  }
}
```

Genetic Algorithm parameters (population size, mutation type/rates, crossover segment size, iteration count, etc.) are configured in `config/ga_config.json` and parsed by `GeneticAlgorithm`.

## Reproducing plots and analysis (optional)

Python scripts in `python/` generate performance plots and GA parameter sweeps. Install deps via `python/requirements.txt` and run individual scripts such as:

- `ga_plot_population_size.py`, `ga_mutation_type.py`, `ga_mutation_rate.py`, `ga_offspring_rate.py`
- `plot1.py`, `plot2.py`, `resultTable2.py`

Output images are in `python/` and `raport/`.

## Results (high level)

Selected artifacts (see `raport/` and `results/` for more):

- Branch and Bound performance: `raport/branch_and_bound_performance_log.png`
- Brute Force performance: `raport/brute_force_performance_log.png`
- Combined performance: `raport/combined_performance.png`
- Tabu/GA result tables and additional plots in `raport/` and `results/`

These demonstrate the expected scaling: BF grows factorially and is only feasible for small N; BnB improves exact solving via pruning; Tabu and GA scale to larger N with near-optimal solutions.

## Why this project (for recruiters)

- End-to-end delivery: modeling, algorithms, instrumentation, analysis, and documentation
- Clean separation of concerns and reproducible experiments via JSON
- Practical exposure to exact vs. metaheuristic trade-offs on ATSP

## License

See `docs/LICENSE`.

## Acknowledgements

- TSPLIB datasets; nlohmann/json
