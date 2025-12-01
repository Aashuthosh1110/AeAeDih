#!/usr/bin/env python3
"""
plot_results.py
Generates high-quality plots from the C++ benchmark results.

Requires: pandas, matplotlib, numpy, scipy
Install with: pip install pandas matplotlib numpy scipy
"""

import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from scipy import stats

# Use a clean, academic style
plt.style.use('seaborn-v0_8-whitegrid')

def plot_error_rate():
    """
    Plot 1: Error Rate vs. Theoretical Bound
    Shows empirical false positive rate vs. k with theoretical 4^-k overlay.
    """
    try:
        df = pd.read_csv('results/results_error_test.csv')
    except FileNotFoundError:
        print("⚠ results/results_error_test.csv not found. Skipping error rate plot.")
        return
    
    # Get unique Carmichael numbers
    carmichael_numbers = df['Number'].unique()
    
    # Get k values (should be 1-10)
    k_values = sorted(df['k'].unique())
    
    # Calculate theoretical bound
    theoretical_fpr = [0.25 ** k for k in k_values]  # 4^-k = 0.25^k
    
    plt.figure(figsize=(14, 9))
    
    # Define colors and markers for each Carmichael number
    colors = ['#1f77b4', '#ff7f0e', '#2ca02c', '#d62728', '#9467bd']
    markers = ['o', 's', '^', 'D', 'v']
    
    # Plot each Carmichael number separately
    for idx, num in enumerate(carmichael_numbers):
        if pd.isna(num):
            continue
        num_data = df[df['Number'] == num].sort_values('k')
        plt.plot(num_data['k'], num_data['FalsePositiveRate'], 
                marker=markers[idx % len(markers)], color=colors[idx % len(colors)], 
                linewidth=2, markersize=8, label=f'Carmichael {int(num)}', alpha=0.8)
    
    # Plot theoretical bound as a thick red dashed line
    plt.plot(k_values, theoretical_fpr, 'r--', linewidth=3, markersize=10,
             label='Theoretical Max Bound ($4^{-k}$)', marker='x', markeredgewidth=2)
    
    plt.yscale('log')
    plt.xlabel('Number of Iterations (k)', fontsize=14, fontweight='bold')
    plt.ylabel('False Positive Rate (Log Scale)', fontsize=14, fontweight='bold')
    plt.title('Miller-Rabin Error Rate: Individual Carmichael Numbers vs. Theoretical Bound', 
              fontsize=15, fontweight='bold')
    plt.legend(fontsize=11, loc='upper right', framealpha=0.95)
    plt.grid(True, which="both", ls="--", alpha=0.5)
    plt.xticks(k_values)
    plt.tight_layout()
    plt.savefig('plots/plot_error_rate.png', dpi=300, bbox_inches='tight')
    print("✓ Saved plots/plot_error_rate.png")
    plt.close()


def plot_naive_vs_mr():
    """
    Plot 2: Naive (Trial Division) vs. Miller-Rabin Time
    Shows the exponential "wall" hit by Trial Division vs. MR's flat performance.
    """
    try:
        df_naive = pd.read_csv('results/results_bench_naive.csv')
        df_mr = pd.read_csv('results/results_bench_mr.csv')
    except FileNotFoundError as e:
        print(f"⚠ Missing file: {e}. Skipping naive vs. MR plot.")
        return
    
    # Group by bit length and average the times
    naive_grouped = df_naive.groupby('BitLength')['TimeSec'].mean().reset_index()
    mr_grouped = df_mr.groupby('BitLength')['TimeSec'].mean().reset_index()
    
    plt.figure(figsize=(12, 8))
    
    # Plot both algorithms
    plt.plot(naive_grouped['BitLength'], naive_grouped['TimeSec'], 
             'o-', color='#e74c3c', linewidth=2.5, markersize=8, 
             label='Trial Division (Exponential)', markeredgecolor='black')
    
    plt.plot(mr_grouped['BitLength'], mr_grouped['TimeSec'], 
             's-', color='#3498db', linewidth=2.5, markersize=8,
             label='Miller-Rabin k=5 (Polylog)', markeredgecolor='black')
    
    # Annotate the "wall" - find where Trial Division becomes slow
    wall_threshold = 0.1  # 100ms
    wall_data = naive_grouped[naive_grouped['TimeSec'] > wall_threshold]
    if not wall_data.empty:
        wall_bit = wall_data.iloc[0]['BitLength']
        wall_time = wall_data.iloc[0]['TimeSec']
        plt.annotate(
            f'Trial Division "Wall"\n~{wall_time:.3f}s at {wall_bit} bits',
            xy=(wall_bit, wall_time),
            xytext=(wall_bit - 5, wall_time * 10),
            arrowprops=dict(facecolor='red', shrink=0.05, width=2, headwidth=8),
            fontsize=11,
            bbox=dict(boxstyle="round,pad=0.5", fc="orange", ec="darkred", lw=2)
        )
    
    plt.yscale('log')
    plt.xlabel('Input Size (Bits)', fontsize=14, fontweight='bold')
    plt.ylabel('Time (Seconds, Log Scale)', fontsize=14, fontweight='bold')
    plt.title('Runtime Comparison: Trial Division vs. Miller-Rabin', 
              fontsize=16, fontweight='bold')
    plt.legend(fontsize=12, loc='upper left')
    plt.grid(True, which="both", ls="--", alpha=0.6)
    plt.tight_layout()
    plt.savefig('plots/plot_naive_vs_mr.png', dpi=300, bbox_inches='tight')
    print("✓ Saved plots/plot_naive_vs_mr.png")
    plt.close()


def plot_k_scaling():
    """
    Plot 3: Miller-Rabin Scalability with k
    Shows linear relationship between k and execution time.
    """
    try:
        df = pd.read_csv('results/results_k_test.csv')
    except FileNotFoundError:
        print("⚠ results/results_k_test.csv not found. Skipping k-scaling plot.")
        return
    
    # Group by k (should be one entry per k, but average just in case)
    grouped = df.groupby('k')['TimeSec'].mean().reset_index()
    
    k_values = grouped['k'].values
    times = grouped['TimeSec'].values
    
    # Perform linear regression
    slope, intercept, r_value, p_value, std_err = stats.linregress(k_values, times)
    trendline = slope * k_values + intercept
    
    plt.figure(figsize=(12, 8))
    
    # Scatter plot with trendline
    plt.plot(k_values, times, 'go', markersize=8, label='Measured Time', 
             markeredgecolor='black', markeredgewidth=1.2)
    plt.plot(k_values, trendline, 'r--', linewidth=2, 
             label=f'Linear Fit: y = {slope:.2e}x + {intercept:.2e}\n$R^2$ = {r_value**2:.4f}')
    
    plt.xlabel('Number of Iterations (k)', fontsize=14, fontweight='bold')
    plt.ylabel('Average Execution Time (Seconds)', fontsize=14, fontweight='bold')
    plt.title('Miller-Rabin: Execution Time vs. k (Linear Scaling)', 
              fontsize=16, fontweight='bold')
    plt.legend(fontsize=12, loc='upper left')
    plt.grid(True, which="both", ls="--", alpha=0.6)
    plt.tight_layout()
    plt.savefig('plots/plot_k_scaling.png', dpi=300, bbox_inches='tight')
    print("✓ Saved plots/plot_k_scaling.png")
    plt.close()


if __name__ == '__main__':
    """
    Main entry point.
    Expects CSV files from the C++ batch program:
    - results/results_error_test.csv
    - results/results_bench_naive.csv
    - results/results_bench_mr.csv
    - results/results_k_test.csv
    """
    print("Generating plots from benchmark results...\n")
    plot_error_rate()
    plot_naive_vs_mr()
    plot_k_scaling()
    print("\n✓ All plots generated successfully!")
