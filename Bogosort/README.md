# Comparative Analysis of Bogosort

This project implements and analyzes the time complexity of the probabilistic **Bogosort** algorithm compared to deterministic algorithms (**Bubble Sort** and **Merge Sort**). It uses a hybrid approach with **C** for high-performance execution of sorting algorithms and **Python** for data visualization.

---

## File Structure

- **`bogosort_cli.c`**  
  Standalone executable implementing Bogosort using the Fisher–Yates shuffle.

- **`benchmark_suite.c`**  
  Test harness that runs experiments, measures execution time, and generates CSV data.

- **`plot_results.py`**  
  Python script to visualize benchmark results.

- **`benchmark_results.csv`**  
  Benchmark output data.

---

## Prerequisites

- **C Compiler:** GCC or Clang  
- **Python:** 3.8+

### Install Python Dependencies

```bash
pip install pandas matplotlib
```

---

## Compilation Instructions

Important: The benchmark suite requires an executable named **`bogosort_cli`**.  
Compile *exactly* as shown.

### Compile the Bogosort Executable

```bash
gcc bogosort_cli.c -o bogosort_cli
```

### Compile the Benchmark Suite

```bash
gcc benchmark_suite.c -o benchmark_suite
```

---

## How to Run

### Step 1: Generate Data

Run the benchmark suite:

```bash
./benchmark_suite
```

This executes:

- Bubble Sort and Merge Sort up to **N = 20**
- Bogosort up to **N = 13**

The suite prints progress and produces:

```
benchmark_results.csv
```

---

### Step 2: Visualize Results

Run the plotting script:

```bash
python plot_results.py
```

This will:

- Display the performance graph  
- Save it as:

```
sorting_benchmark.png
```

---

## Implementation Details

### Bogosort

The Bogosort implementation (`bogosort_cli.c`):

- Runs as a separate process to isolate memory usage  
- Repeatedly performs:
  1. Fisher–Yates shuffle  
  2. Sortedness check  
- Loops until the array is sorted

Because Bogosort is extremely slow and highly variable, it is executed only up to **N = 13**.

---

### Benchmarking

The benchmark harness (`benchmark_suite.c`):

- Uses `popen` to launch `bogosort_cli`
- Runs:
  - **20 trials** for small N  
  - **5 trials** for larger N  
- Averages execution times
- Saves results to `benchmark_results.csv`

This helps smooth Bogosort’s huge runtime variance.

---

