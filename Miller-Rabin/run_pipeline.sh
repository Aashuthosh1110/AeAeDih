#!/bin/bash
# run_pipeline.sh
# Automated benchmark pipeline for Miller-Rabin analysis
# Usage: ./run_pipeline.sh

set -e  # Exit on any error

echo "================================================"
echo "Miller-Rabin Automated Benchmark Pipeline"
echo "================================================"
echo ""

# Activate virtual environment if it exists
if [ -d "../.venv" ]; then
    echo "Activating virtual environment..."
    source ../.venv/bin/activate
    echo ""
fi

# Step 1: Generate datasets
echo "[Step 1/4] Generating test datasets..."
python3 scripts/generate_datasets.py
echo ""

# Step 2: Compile the batch processing program
echo "[Step 2/4] Compiling main_batch.cpp..."
g++ -std=c++11 -O3 src/main_batch.cpp src/miller_rabin.cpp src/trial_division.cpp -o bin/main_batch
echo "✓ Compiled bin/main_batch"
echo ""

# Step 3: Run all benchmarks
echo "[Step 3/4] Running benchmarks..."
echo "  → Benchmarking Trial Division (naive)..."
./bin/main_batch --bench-naive data/dataset_naive.csv > results/results_bench_naive.csv
echo "  ✓ results/results_bench_naive.csv"

echo "  → Benchmarking Miller-Rabin (k=5)..."
./bin/main_batch --bench-mr data/dataset_mr.csv > results/results_bench_mr.csv
echo "  ✓ results/results_bench_mr.csv"

echo "  → Running error analysis on Carmichael numbers..."
./bin/main_batch --error-test data/dataset_carmichael.csv > results/results_error_test.csv
echo "  ✓ results/results_error_test.csv"

echo "  → Running k-scaling analysis..."
./bin/main_batch --k-test data/dataset_k_scaling.csv > results/results_k_test.csv
echo "  ✓ results/results_k_test.csv"
echo ""

# Step 4: Generate plots
echo "[Step 4/4] Generating visualization plots..."
python3 scripts/plot_results.py
echo ""

echo "================================================"
echo "✓ Pipeline completed successfully!"
echo "================================================"
echo ""
echo "Generated files:"
echo "  Datasets:"
echo "    - data/dataset_carmichael.csv"
echo "    - data/dataset_naive.csv"
echo "    - data/dataset_mr.csv"
echo "    - data/dataset_k_scaling.csv"
echo ""
echo "  Results:"
echo "    - results/results_bench_naive.csv"
echo "    - results/results_bench_mr.csv"
echo "    - results/results_error_test.csv"
echo "    - results/results_k_test.csv"
echo ""
echo "  Plots:"
echo "    - plots/plot_error_rate.png"
echo "    - plots/plot_naive_vs_mr.png"
echo "    - plots/plot_k_scaling.png"
echo ""
