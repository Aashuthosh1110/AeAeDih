import subprocess
import matplotlib.pyplot as plt
import csv
import io

# --- This is the function that runs your C++ code ---
def run_runtime_experiment(cpp_executable, file_list):
    """
    Runs the C++ benchmark program for experiment 1 and returns its CSV output.
    
    Args:
        cpp_executable (str): The path to the compiled program (e.g., "./benchmark_program")
        file_list (list): A list of .txt filenames to feed into the program.
    """
    
    # 1. Prepare the input for the C++ program
    # This simulates you typing "1" (for experiment 1),
    # then the list of files, then "done".
    input_text = "1\n" + "\n".join(file_list) + "\ndone\n"
    
    print(f"Running C++ program: {cpp_executable}")
    
    # 2. Run the C++ program
    # We pass the input_text to its stdin
    result = subprocess.run(
        [cpp_executable],
        input=input_text,
        text=True,
        capture_output=True,
        check=True
    )
    
    # 3. The C++ program prints a lot of text. We only want the CSV data.
    csv_data = []
    in_csv_section = False
    for line in result.stdout.splitlines():
        if "n,time_ms" in line: # This is the CSV header
            in_csv_section = True
        if in_csv_section:
            csv_data.append(line)
            
    print("...C++ program finished.")
    return "\n".join(csv_data)

# --- This is the Matplotlib plotting function ---
def plot_runtime_data(csv_data):
    """
    Takes a string of CSV data and generates a plot, saving it to a file.
    """
    
    # Use io.StringIO to treat the CSV string as a file
    f = io.StringIO(csv_data)
    reader = csv.reader(f)
    
    # Read the data into lists
    header = next(reader) # Skip header
    n_values = []
    time_values = []
    
    for row in reader:
        if row: # Avoid empty rows
            n_values.append(int(row[0]))
            time_values.append(float(row[1]))

    print(f"Plotting data: n={n_values}, time={time_values}")

    # 4. Create the plot with Matplotlib
    plt.figure(figsize=(10, 6))
    plt.plot(n_values, time_values, marker='o', linestyle='-')
    
    # 5. Label the plot (required by your guidelines)
    plt.title("Runtime vs. Graph Size (n) for Karger-Stein's Algorithm")
    plt.xlabel("Graph Size (n vertices)")
    plt.ylabel("Average Runtime (ms) for n^2 Iterations")
    plt.grid(True)
    
    # 6. Save the plot to a file
    output_filename = "runtime_vs_size.png"
    plt.savefig(output_filename)
    print(f"Success! Graph saved to {output_filename}")

# --- Main execution ---
if __name__ == "__main__":
    
    # 1. Define your C++ program location and test files
    executable_path = "./stein_program"
    files_to_test = [
        "n10.txt",
        "n20.txt",
        "n30.txt",
        "n40.txt",
        "n50.txt"
    ]
    
    # 2. Run the experiment
    csv_output = run_runtime_experiment(executable_path, files_to_test)
    
    # 3. Plot the results
    if csv_output:
        plot_runtime_data(csv_output)
    else:
        print("Error: No CSV data was captured from the C++ program.")