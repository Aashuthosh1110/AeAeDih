#!/bin/bash
# run_pipeline.sh
# Automated benchmark pipeline for Miller-Rabin analysis
# Usage: ./run_pipeline.sh

set -e  # Exit on any error

echo "================================================"
echo "Miller-Rabin Automated Benchmark Pipeline"
echo "================================================"
echo ""

# Step 1: Generate datasets
echo "[Step 1/4] Generating test datasets..."
python3 generate_datasets.py
echo ""

# Step 2: Compile the batch processing program
echo "[Step 2/4] Compiling main_batch.cpp..."
g++ -std=c++11 -O3 main_batch.cpp miller_rabin.cpp trial_division.cpp -o main_batch
echo "✓ Compiled main_batch"
echo ""

# Step 3: Run all benchmarks
echo "[Step 3/4] Running benchmarks..."
echo "  → Benchmarking Trial Division (naive)..."
./main_batch --bench-naive dataset_naive.csv > results_bench_naive.csv
echo "  ✓ results_bench_naive.csv"

echo "  → Benchmarking Miller-Rabin (k=5)..."
./main_batch --bench-mr dataset_mr.csv > results_bench_mr.csv
echo "  ✓ results_bench_mr.csv"

echo "  → Running error analysis on Carmichael numbers..."
./main_batch --error-test dataset_carmichael.csv > results_error_test.csv
echo "  ✓ results_error_test.csv"

echo "  → Running k-scaling analysis..."
./main_batch --k-test dataset_k_scaling.csv > results_k_test.csv
echo "  ✓ results_k_test.csv"
echo ""

# Step 4: Generate plots
echo "[Step 4/4] Generating visualization plots..."
python3 plot_results.py
echo ""

echo "================================================"
echo "✓ Pipeline completed successfully!"
echo "================================================"
echo ""
echo "Generated files:"
echo "  Datasets:"
echo "    - dataset_carmichael.csv"
echo "    - dataset_naive.csv"
echo "    - dataset_mr.csv"
echo "    - dataset_k_scaling.csv"
echo ""
echo "  Results:"
echo "    - results_bench_naive.csv"
echo "    - results_bench_mr.csv"
echo "    - results_error_test.csv"
echo "    - results_k_test.csv"
echo ""
echo "  Plots:"
echo "    - plot_error_rate.png"
echo "    - plot_naive_vs_mr.png"
echo "    - plot_k_scaling.png"
echo ""
