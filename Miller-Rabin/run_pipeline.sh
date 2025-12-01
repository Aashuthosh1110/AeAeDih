#!/bin/bash
# run_pipeline.sh
# Automated benchmark pipeline for Miller-Rabin analysis

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
# Move datasets to data/ if they were generated in root
mkdir -p data
mv dataset_*.csv data/ 2>/dev/null || true
echo ""

# Step 2: Compile the batch processing program
echo "[Step 2/4] Compiling main_batch.cpp..."
# Added -I src to ensure headers are found safely
if [ -d "src" ]; then
    g++ -std=c++11 -O3 -I src src/main_batch.cpp src/miller_rabin.cpp src/trial_division.cpp -o main_batch
    echo "✓ Compiled main_batch"
else
    g++ -std=c++11 -O3 main_batch.cpp miller_rabin.cpp trial_division.cpp -o main_batch
    echo "✓ Compiled main_batch"
fi
echo ""

# Setup directories
DATA_DIR="data/"
RESULTS_DIR="results/"
PLOTS_DIR="plots/"
mkdir -p "$RESULTS_DIR" "$PLOTS_DIR"

# Step 3: Run all benchmarks
echo "[Step 3/4] Running benchmarks..."
echo "  → Benchmarking Trial Division (naive)..."
./main_batch --bench-naive ${DATA_DIR}dataset_naive.csv > ${RESULTS_DIR}bench_naive.csv
echo "  ✓ ${RESULTS_DIR}bench_naive.csv"

echo "  → Benchmarking Miller-Rabin (k=5)..."
./main_batch --bench-mr ${DATA_DIR}dataset_mr.csv > ${RESULTS_DIR}bench_mr.csv
echo "  ✓ ${RESULTS_DIR}bench_mr.csv"

echo "  → Running error analysis on Carmichael numbers..."
./main_batch --error-test ${DATA_DIR}dataset_carmichael.csv > ${RESULTS_DIR}error_test.csv
echo "  ✓ ${RESULTS_DIR}error_test.csv"

echo "  → Running k-scaling analysis..."
./main_batch --k-test ${DATA_DIR}dataset_k_scaling.csv > ${RESULTS_DIR}k_test.csv
echo "  ✓ ${RESULTS_DIR}k_test.csv"
echo ""

# Step 4: Generate plots
echo "[Step 4/4] Generating visualization plots..."

# FIX: Link results to root so python script can find them
ln -sf ${RESULTS_DIR}*.csv .

python3 scripts/plot_results.py

# Cleanup links and move plots
rm *.csv 2>/dev/null || true
mv plot_*.png "$PLOTS_DIR" 2>/dev/null || true

echo ""
echo "================================================"
echo "✓ Pipeline completed successfully!"
echo "================================================"
echo ""
echo "Generated files:"
echo "  Datasets:  $DATA_DIR"
echo "  Results:   $RESULTS_DIR"
echo "  Plots:     $PLOTS_DIR"
echo ""