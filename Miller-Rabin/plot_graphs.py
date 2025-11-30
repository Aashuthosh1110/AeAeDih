import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# For academic-quality graphs, a clean style is recommended.
plt.style.use('seaborn-v0_8-whitegrid')

def plot_runtime_comparison():
    """
    Generates a grouped bar chart comparing the runtime of Trial Division and Miller-Rabin.
    """
    try:
        df = pd.read_csv('runtime_comparison.csv')
    except FileNotFoundError:
        print("Error: runtime_comparison.csv not found. Please generate the data first.")
        return

    # Filter data: only plot rows where BitSize >= 32 (removes system noise)
    df = df[df['BitSize'] >= 32]
    
    # Group by BitSize and calculate the mean time for smoother plotting
    grouped = df.groupby('BitSize').mean().reset_index()
    
    # Replace -1 values (skipped Trial Division) with NaN for proper visualization
    grouped['TrialDivTime'] = grouped['TrialDivTime'].replace(-1, np.nan)

    plt.figure(figsize=(12, 8))
    
    # Set up the bar positions
    x = np.arange(len(grouped['BitSize']))
    bar_width = 0.35
    
    # Create the grouped bar chart
    bars1 = plt.bar(x - bar_width/2, grouped['TrialDivTime'], bar_width, 
                    label='Trial Division', color='#e74c3c', edgecolor='black', linewidth=1.2)
    bars2 = plt.bar(x + bar_width/2, grouped['MillerRabinTime'], bar_width, 
                    label='Miller-Rabin (k=5)', color='#3498db', edgecolor='black', linewidth=1.2)
    
    # Set the Y-axis to a logarithmic scale (CRUCIAL for comparing vastly different scales)
    plt.yscale('log')
    
    # Annotate the "Wall" - point to the last valid Trial Division bar (32-bit)
    valid_td_indices = grouped[grouped['TrialDivTime'].notna()]['BitSize'].index
    if len(valid_td_indices) > 0:
        last_valid_idx = valid_td_indices[-1]
        last_bit = grouped.loc[last_valid_idx, 'BitSize']
        last_time = grouped.loc[last_valid_idx, 'TrialDivTime']
        bar_x_position = last_valid_idx - bar_width/2
        
        plt.annotate(
            f'Hit Wall (~{last_time:.2f}s)',
            xy=(bar_x_position, last_time),
            xytext=(bar_x_position - 1, last_time * 50),
            arrowprops=dict(facecolor='red', edgecolor='darkred', shrink=0.05, width=2, headwidth=10),
            fontsize=12,
            fontweight='bold',
            bbox=dict(boxstyle="round,pad=0.5", fc="orange", ec="darkred", lw=2, alpha=0.9)
        )

    # --- Formatting ---
    plt.title('Runtime Comparison: Exponential vs. Polynomial', fontsize=16, fontweight='bold')
    plt.xlabel('Input Size (Bits)', fontsize=13, fontweight='bold')
    plt.ylabel('Time in Seconds (Log Scale)', fontsize=13, fontweight='bold')
    plt.xticks(x, grouped['BitSize'], rotation=0)
    plt.legend(fontsize=12, loc='upper left')
    plt.grid(True, which="both", ls="--", alpha=0.5, axis='y')
    plt.tight_layout()
    plt.savefig('graph_runtime_comparison.png', dpi=300, bbox_inches='tight')
    print("Saved graph_runtime_comparison.png")
    plt.show()


def plot_error_analysis():
    """
    Generates a bar chart of the empirical false positive rate vs. theoretical bounds.
    """
    try:
        df = pd.read_csv('error_analysis.csv')
    except FileNotFoundError:
        print("Error: error_analysis.csv not found. Please generate the data first.")
        return

    k_values = df['K_Iterations']
    fpr = df['FalsePositiveRate']
    
    # Calculate the theoretical error bound: 1/4^k or 4^(-k)
    theoretical_bound = [0.25**k for k in k_values]  # Same as 1/4^k

    plt.figure(figsize=(12, 8))
    
    # Plot the empirical results as a bar chart (even if all zeros)
    bar_width = 0.4
    plt.bar(k_values, fpr, width=bar_width, color='skyblue', edgecolor='black', 
            label='Empirical False Positive Rate', alpha=0.7)
    
    # Overlay the theoretical bound as a RED line plot
    plt.plot(k_values, theoretical_bound, 'r-o', linewidth=2, markersize=8,
             label='Theoretical Max Error Bound ($4^{-k}$)')
    
    # Add text annotation noting the observed error is zero
    plt.text(0.65, 0.85, 'Observed Error = 0.00%\n(100k trials on N=1729)', 
             transform=plt.gca().transAxes, fontsize=12, 
             bbox=dict(boxstyle='round', facecolor='lightyellow', edgecolor='black', linewidth=1.5),
             verticalalignment='top')
    
    # --- Formatting ---
    plt.title('Miller-Rabin Error Analysis: Theory vs. Practice', fontsize=16, fontweight='bold')
    plt.xlabel('Number of Iterations (k)', fontsize=12)
    plt.ylabel('False Positive Rate', fontsize=12)
    plt.xticks(k_values)
    plt.yscale('log') # Use log scale to see small rates clearly
    plt.legend(fontsize=12, loc='upper right')
    plt.grid(True, which="both", ls="--", alpha=0.7)
    plt.tight_layout()
    plt.savefig('graph_error_analysis.png', dpi=300)
    print("Saved graph_error_analysis.png")
    plt.show()


def plot_parameter_sensitivity():
    """
    Generates a graph showing the relationship between k-iterations and execution time.
    """
    try:
        df = pd.read_csv('parameter_sensitivity.csv')
    except FileNotFoundError:
        print("Error: parameter_sensitivity.csv not found. Please generate the data first.")
        return

    plt.figure(figsize=(12, 8))
    
    plt.plot(df['K_Iterations'], df['ExecutionTime'], 'g-o', label='Execution Time per k')
    
    # --- Formatting ---
    plt.title('Miller-Rabin: Execution Time vs. Number of Iterations (k)', fontsize=16)
    plt.xlabel('Number of Iterations (k)', fontsize=12)
    plt.ylabel('Average Execution Time (seconds)', fontsize=12)
    plt.legend(fontsize=12)
    plt.grid(True, which="both", ls="--")
    plt.tight_layout()
    plt.savefig('graph_parameter_sensitivity.png')
    print("Saved graph_parameter_sensitivity.png")
    plt.show()


if __name__ == '__main__':
    """
    Main entry point of the script.
    Generates all three graphs.
    
    Requires: pandas, matplotlib
    Install with:
    pip install pandas matplotlib
    """
    print("Generating graphs from CSV data...")
    plot_runtime_comparison()
    plot_error_analysis()
    plot_parameter_sensitivity()
    print("All graphs generated.")
