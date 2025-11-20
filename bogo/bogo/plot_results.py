import pandas as pd
import matplotlib.pyplot as plt
import math

def factorial(n):
    return math.factorial(n)

def plot_benchmark():
    try:
        df = pd.read_csv('benchmark_results.csv')
    except FileNotFoundError:
        print("Error: benchmark_results.csv not found. Run ./benchmark_suite first.")
        return

    # Filter N < 4 to remove startup noise
    df_plot = df[df['N'] >= 4].copy()
    
    plt.figure(figsize=(12, 8))

    # --- 1. BUBBLE SORT (Real Data Only) ---
    plt.plot(df_plot['N'], df_plot['BubbleSort_Time'], 
             label='Bubble Sort (Real)', color='blue', marker='o', linestyle='-', linewidth=2, alpha=0.8)

    # --- 2. MERGE SORT (Real Data Only) ---
    plt.plot(df_plot['N'], df_plot['MergeSort_Time'], 
             label='Merge Sort (Real)', color='green', marker='x', linestyle='-', linewidth=2, alpha=0.8)

    # --- 3. BOGOSORT (Real + Theoretical Extension) ---
    # Filter valid Bogosort times (remove 0.0s)
    real_bogo = df_plot[df_plot['Bogosort_Time'] > 0].copy()
    
    if not real_bogo.empty:
        # Plot Real Data Line
        plt.plot(real_bogo['N'], real_bogo['Bogosort_Time'], 
                 label='Bogosort (Real)', color='red', marker='s', linestyle='-', linewidth=2)

        # Calculate Theoretical Extension
        anchor_n = int(real_bogo.iloc[-1]['N'])
        anchor_time = float(real_bogo.iloc[-1]['Bogosort_Time'])
        
        # Constant k = Time / (n * n!)
        k_bogo = anchor_time / (anchor_n * factorial(anchor_n))
        
        # Generate theoretical points for N=2 to 20
        theo_n = list(range(4, 21))
        theo_time = [k_bogo * n * factorial(n) for n in theo_n]
        
        # Plot Theoretical Line
        plt.plot(theo_n, theo_time, 
                 label='Bogosort (Theoretical O(n·n!))', color='red', linestyle='--', alpha=0.5)
        
        # Annotate the split point
        plt.annotate(f'Benchmark Limit\n(N={anchor_n})', 
                     xy=(anchor_n, anchor_time),
                     xytext=(anchor_n-3, anchor_time),
                     arrowprops=dict(facecolor='black', shrink=0.05))

    # --- Formatting ---
    plt.yscale('log')
    plt.title('Analysis of Bogosort: Empirical vs Theoretical Complexity', fontsize=16)
    plt.xlabel('Input Size (N)', fontsize=12)
    plt.ylabel('Time (Seconds) - Log Scale', fontsize=12)
    plt.grid(True, which="both", ls="-", alpha=0.2)
    plt.legend(fontsize=12, loc='upper left')

    plt.tight_layout()
    plt.savefig('sorting_benchmark.png')
    print("Graph saved as sorting_benchmark.png")
    plt.show()

if __name__ == "__main__":
    plot_benchmark()