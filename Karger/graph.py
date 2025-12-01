import subprocess
import matplotlib.pyplot as plt
import csv
import io

def run_runtime_experiment(cpp_executable, file_list):
    """
    Runs the C++ benchmark program for experiment 1 and returns its CSV output.
    
    Args:
        cpp_executable (str): The path to the compiled program (e.g., "./benchmark_program")
        file_list (list): A list of .txt filenames to feed into the program.
    """
    
  
    input_text = "1\n" + "\n".join(file_list) + "\ndone\n"
    
    print(f"Running C++ program: {cpp_executable}")
    
   
    result = subprocess.run(
        [cpp_executable],
        input=input_text,
        text=True,
        capture_output=True,
        check=True
    )
    

    csv_data = []
    in_csv_section = False
    for line in result.stdout.splitlines():
        if "n,time_ms" in line: 
            in_csv_section = True
        if in_csv_section:
            csv_data.append(line)
            
    print("...C++ program finished.")
    return "\n".join(csv_data)

def plot_runtime_data(csv_data):
    """
    Takes a string of CSV data and generates a plot, saving it to a file.
    """
    
    f = io.StringIO(csv_data)
    reader = csv.reader(f)
    
    header = next(reader) 
    n_values = []
    time_values = []
    
    for row in reader:
        if row: 
            n_values.append(int(row[0]))
            time_values.append(float(row[1]))

    print(f"Plotting data: n={n_values}, time={time_values}")

    plt.figure(figsize=(10, 6))
    plt.plot(n_values, time_values, marker='o', linestyle='-')
    
    plt.title("Runtime vs. Graph Size (n) for Karger-Stein's Algorithm")
    plt.xlabel("Graph Size (n vertices)")
    plt.ylabel("Average Runtime (ms) for n^2 Iterations")
    plt.grid(True)
    
    output_filename = "runtime_vs_size.png"
    plt.savefig(output_filename)
    print(f"Success! Graph saved to {output_filename}")


if __name__ == "__main__":

    executable_path = "./stein_program"
    files_to_test = [
        "n10.txt",
        "n20.txt",
        "n30.txt",
        "n40.txt",
        "n50.txt"
    ]

    csv_output = run_runtime_experiment(executable_path, files_to_test)
    

    if csv_output:
        plot_runtime_data(csv_output)
    else:
        print("Error: No CSV data was captured from the C++ program.")