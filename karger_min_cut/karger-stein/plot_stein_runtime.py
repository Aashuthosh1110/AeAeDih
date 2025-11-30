import subprocess
import matplotlib.pyplot as plt
import csv
import io
import os

# --- Configuration ---
CPP_SOURCE = "stein_runtime.cpp"
EXECUTABLE = "./stein_runtime"
DATA_FILES = ["n10.txt", "n20.txt", "n30.txt", "n40.txt", "n50.txt"]
OUTPUT_IMAGE = "stein_runtime_vs_size.png"
# ---------------------

def compile_cpp():
    print(f"Compiling {CPP_SOURCE}...")
    cmd = ["g++", "-std=c++11", CPP_SOURCE, "-o", EXECUTABLE.replace("./", "")]
    subprocess.run(cmd, check=True)

def run_experiment():
    # Create the input string: "n10.txt\nn20.txt...\ndone\n"
    input_str = "\n".join(DATA_FILES) + "\ndone\n"
    
    print(f"Running {EXECUTABLE} on {len(DATA_FILES)} files...")
    result = subprocess.run(
        [EXECUTABLE],
        input=input_str,
        text=True,
        capture_output=True,
        check=True
    )
    
    # Filter output for CSV lines
    csv_lines = []
    is_data = False
    for line in result.stdout.splitlines():
        if "n,time_ms" in line:
            is_data = True
        if is_data:
            csv_lines.append(line)
            
    return "\n".join(csv_lines)

def plot_data(csv_data):
    f = io.StringIO(csv_data)
    reader = csv.DictReader(f)
    
    ns = []
    times = []
    
    for row in reader:
        ns.append(int(row['n']))
        times.append(float(row['time_ms']))

    print(f"Data points: {list(zip(ns, times))}")

    plt.figure(figsize=(10, 6))
    plt.plot(ns, times, marker='o', linestyle='-', color='orange', label='Karger-Stein (100 runs)')
    
    plt.title("Karger-Stein Runtime vs. Graph Size")
    plt.xlabel("Number of Vertices (n)")
    plt.ylabel("Total Runtime (ms)")
    plt.grid(True)
    plt.legend()
    
    plt.savefig(OUTPUT_IMAGE)
    print(f"Graph saved to {OUTPUT_IMAGE}")

if __name__ == "__main__":
    try:
        compile_cpp()
        data = run_experiment()
        plot_data(data)
    except Exception as e:
        print(f"An error occurred: {e}")