# Miller-Rabin Primality Test

## Project Structure

```
.
├── README.md                    # This file
├── report.md                    # Comprehensive analysis report
├── run_pipeline.sh             # One-command automation script
│
├── src/                        # C++ source code
│   ├── main.cpp               # Interactive program
│   ├── main_batch.cpp         # Batch processing program
│   ├── miller_rabin.cpp/hpp   # Miller-Rabin implementation
│   └── trial_division.cpp/hpp # Trial Division implementation
│
├── scripts/                    # Python automation scripts
│   ├── generate_datasets.py   # Dataset generation
│   └── plot_results.py        # Visualization
│
├── data/                       # Generated datasets (CSV)
├── results/                    # Benchmark results (CSV)
├── plots/                      # Generated visualizations (PNG)
└── bin/                        # Compiled executables
```

## Dependencies

### C++ Compilation
- **Compiler**: GCC with C++11 support (or compatible compiler supporting `__int128`)
- **Standard Libraries**: `<cstdint>`, `<random>`, `<iostream>`, `<fstream>`, `<chrono>`

### Python Environment (for benchmarking and plotting)
- **Python**: 3.8 or higher
- **Required Packages**:
  - `sympy` - For generating mathematically verified prime numbers
  - `matplotlib` - For plotting results
  - `pandas` - For data manipulation
  - `numpy` - For numerical operations
  - `scipy` - For statistical regression

**Install Python Dependencies:**

```bash
# Create virtual environment (recommended)
python3 -m venv .venv
source .venv/bin/activate  # On Windows: .venv\Scripts\activate

# Install required packages
pip install sympy matplotlib pandas numpy scipy
```

## Compilation

**Compile the interactive program:**

```bash
g++ -std=c++11 -O3 src/main.cpp src/miller_rabin.cpp src/trial_division.cpp -o bin/primality_test
```

**Compile the batch processing program:**

```bash
g++ -std=c++11 -O3 src/main_batch.cpp src/miller_rabin.cpp src/trial_division.cpp -o bin/main_batch
```

## Running the Project

**Run the interactive program:**

```bash
./bin/primality_test
```

**Run the complete benchmarking pipeline:**

```bash
# Make executable (first time only)
chmod +x run_pipeline.sh

# Run pipeline
./run_pipeline.sh
```

This generates datasets, compiles programs, runs benchmarks, and creates plots.
