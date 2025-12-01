import subprocess
import matplotlib.pyplot as plt
import csv
import io
import sys


CPP_EXECUTABLE = "./stein_program"  
FILE_TO_TEST = "n50_needle.txt"                
CORRECT_MIN_CUT = 2                   
NUM_TRIALS = 100                       
OUTPUT_FILENAME = "stein_success_vs_iterations.png"


def run_success_rate_experiment():
    """
    Runs the C++ benchmark program for experiment 2 and returns its CSV output.
    """
    

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
            timeout=2000
        )
    except FileNotFoundError:
        print(f"Error: The program '{CPP_EXECUTABLE}' was not found.")
        print(f"Please compile benchmark.cpp first with: g++ -std=c++11 benchmark.cpp -o {CPP_EXECUTABLE.lstrip('./')}")
        sys.exit(1)
    except subprocess.CalledProcessError as e:
        print(f"Error running C++ program:")
        print(e.stderr)
        sys.exit(1)

    csv_data = []
    in_csv_section = False
    for line in result.stdout.splitlines():
        if "Iterations,SuccessRate" in line: 
            in_csv_section = True
        if in_csv_section:
            csv_data.append(line)
            
    print("...C++ program finished.")
    return "\n".join(csv_data)

def plot_success_rate_data(csv_data):
    """
    Takes a string of CSV data and generates a plot, saving it to a file.
    """
    
    f = io.StringIO(csv_data)
    reader = csv.reader(f)
    
    try:
        header = next(reader) 
        iteration_values = []
        success_values = []
        
        for row in reader:
            if row: 
                iteration_values.append(int(row[0]))
                success_values.append(float(row[1]))
    except (StopIteration, IndexError, ValueError):
        print("\nError: Could not parse any data from the C++ program.")
        print("Please make sure the C++ program is printing the CSV header 'Iterations,SuccessRate' followed by data.")
        return

    print(f"Plotting data: Iterations={iteration_values}, Rates={success_values}")

 
    plt.figure(figsize=(10, 6))
    plt.plot(iteration_values, success_values, marker='o', linestyle='-')
    
    
    plt.title(f"Success Rate vs. Iterations (T) on {FILE_TO_TEST}")
    plt.xlabel("Number of Iterations (T) per Trial")
    plt.ylabel(f"Success Rate (out of {NUM_TRIALS} trials)")
    plt.ylim(0, 1.05)
    plt.grid(True)
    
    plt.savefig(OUTPUT_FILENAME)
    print(f"\nSuccess! Graph saved to {OUTPUT_FILENAME}")

if __name__ == "__main__":
    
    
    csv_output = run_success_rate_experiment()
    
    if csv_output:
        plot_success_rate_data(csv_output)
    else:
        print("Error: No CSV data was captured.")