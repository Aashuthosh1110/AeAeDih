import pandas as pd
import matplotlib.pyplot as plt

# Read CSV file containing results
df = pd.read_csv("runtime_results.csv")

# Extract dataset names for x-axis
x = df['dataset_name']

# Plot average runtimes
plt.figure(figsize=(12, 6))
plt.plot(x, df['det_avg'], marker='o', label='Deterministic Quicksort - Avg')
plt.plot(x, df['rand_avg'], marker='o', label='Randomized Quicksort - Avg')

plt.title("Average Runtime Comparison")
plt.xlabel("Dataset")
plt.ylabel("Runtime (ms)")
plt.grid(True)
plt.xticks(rotation=45)
plt.legend()
plt.tight_layout()
plt.savefig("average_runtime_comparison.png")
plt.show()

# Plot best cases
plt.figure(figsize=(12, 6))
plt.plot(x, df['det_best'], marker='o', label='Deterministic - Best')
plt.plot(x, df['rand_best'], marker='o', label='Randomized - Best')

plt.title("Best Case Runtime Comparison")
plt.xlabel("Dataset")
plt.ylabel("Runtime (ms)")
plt.grid(True)
plt.xticks(rotation=45)
plt.legend()
plt.tight_layout()
plt.savefig("best_runtime_comparison.png")
plt.show()

# Plot worst cases
plt.figure(figsize=(12, 6))
plt.plot(x, df['det_worst'], marker='o', label='Deterministic - Worst')
plt.plot(x, df['rand_worst'], marker='o', label='Randomized - Worst')

plt.title("Worst Case Runtime Comparison")
plt.xlabel("Dataset")
plt.ylabel("Runtime (ms)")
plt.grid(True)
plt.xticks(rotation=45)
plt.legend()
plt.tight_layout()
plt.savefig("worst_runtime_comparison.png")
plt.show()

print("Graphs generated successfully: ")
print(" - average_runtime_comparison.png")
print(" - best_runtime_comparison.png")
print(" - worst_runtime_comparison.png")
