import subprocess
import matplotlib.pyplot as plt
import csv
import io
import sys

# --- Configuration ---
CPP_EXECUTABLE = "./stein_program"  # Your compiled C++ program
FILE_TO_TEST = "n50_needle.txt"                # Which graph file to test
CORRECT_MIN_CUT = 2                   # The known correct answer for this file
NUM_TRIALS = 100                        # How many times to test each iteration count
OUTPUT_FILENAME = "stein_success_vs_iterations.png"


def run_success_rate_experiment():
    """
    Runs the C++ benchmark program for experiment 2 and returns its CSV output.
    """
    
    # 1. Prepare the input for the C++ program
    # This simulates you typing "2", "n20.txt", "2", and "200"
    input_text  = f"2\n"
    input_text += f"{FILE_TO_TEST}\n"
    input_text += f"{CORRECT_MIN_CUT}\n"
    input_text += f"{NUM_TRIALS}\n"
    
    print(f"Running C++ program: {CPP_EXECUTABLE}")
    print(f"Testing file: {FILE_TO_TEST} (Min Cut = {CORRECT_MIN_CUT})")
    print(f"Running {NUM_TRIALS} trials for each iteration count...")

    # 2. Run the C++ program
    try:
        result = subprocess.run(
            [CPP_EXECUTABLE],
            input=input_text,
            text=True,
            capture_output=True,
            check=True,
            timeout=2000# 5-minute timeout, just in case
        )
    except FileNotFoundError:
        print(f"Error: The program '{CPP_EXECUTABLE}' was not found.")
        print(f"Please compile benchmark.cpp first with: g++ -std=c++11 benchmark.cpp -o {CPP_EXECUTABLE.lstrip('./')}")
        sys.exit(1)
    except subprocess.CalledProcessError as e:
        print(f"Error running C++ program:")
        print(e.stderr)
        sys.exit(1)

    # 3. The C++ program prints a lot. We only want the CSV data.
    csv_data = []
    in_csv_section = False
    for line in result.stdout.splitlines():
        if "Iterations,SuccessRate" in line: # This is the CSV header
            in_csv_section = True
        if in_csv_section:
            csv_data.append(line)
            
    print("...C++ program finished.")
    return "\n".join(csv_data)

def plot_success_rate_data(csv_data):
    """
    Takes a string of CSV data and generates a plot, saving it to a file.
    """
    
    # Use io.StringIO to treat the CSV string as a file
    f = io.StringIO(csv_data)
    reader = csv.reader(f)
    
    # Read the data into lists
    try:
        header = next(reader) # Skip header
        iteration_values = []
        success_values = []
        
        for row in reader:
            if row: # Avoid empty rows
                iteration_values.append(int(row[0]))
                success_values.append(float(row[1]))
    except (StopIteration, IndexError, ValueError):
        print("\nError: Could not parse any data from the C++ program.")
        print("Please make sure the C++ program is printing the CSV header 'Iterations,SuccessRate' followed by data.")
        return

    print(f"Plotting data: Iterations={iteration_values}, Rates={success_values}")

    # 4. Create the plot with Matplotlib
    plt.figure(figsize=(10, 6))
    plt.plot(iteration_values, success_values, marker='o', linestyle='-')
    
    # 5. Label the plot (required by your guidelines)
    plt.title(f"Success Rate vs. Iterations (T) on {FILE_TO_TEST}")
    plt.xlabel("Number of Iterations (T) per Trial")
    plt.ylabel(f"Success Rate (out of {NUM_TRIALS} trials)")
    plt.ylim(0, 1.05) # Set Y-axis from 0 to 105%
    plt.grid(True)
    
    # 6. Save the plot to a file
    plt.savefig(OUTPUT_FILENAME)
    print(f"\nSuccess! Graph saved to {OUTPUT_FILENAME}")

# --- Main execution ---
if __name__ == "__main__":
    
    # 1. Run the experiment
    csv_output = run_success_rate_experiment()
    
    # 2. Plot the results
    if csv_output:
        plot_success_rate_data(csv_output)
    else:
        print("Error: No CSV data was captured.")