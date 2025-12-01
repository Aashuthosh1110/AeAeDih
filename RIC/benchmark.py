import subprocess
import random
import statistics
import os
import sys
import math
import matplotlib.pyplot as plt


def compile_cpp(filename, output_name):
    if not os.path.exists(filename):
        print(f"Error: {filename} not found.")
        sys.exit(1)
    cmd = ["g++", "-O3", filename, "-o", output_name]
    print(f"Compiling {filename}...")
    try:
        subprocess.run(cmd, check=True)
    except subprocess.CalledProcessError:
        print(f"Compilation failed for {filename}")
        sys.exit(1)


def generate_random_points(n):
    # The kind of stuff I have to do to satisfy the linter smh
    points = [f"{random.uniform(-10000, 10000):.4f}"
              f"{random.uniform(-10000, 10000):.4f}"
              for _ in range(n)]
    return "\n".join(points)


def generate_circle_points(n):
    points = []
    for _ in range(n):
        angle = random.uniform(0, 2 * math.pi)
        r = 10000.0
        x = r * math.cos(angle)
        y = r * math.sin(angle)
        points.append(f"{x:.4f} {y:.4f}")
    return "\n".join(points)


def run_algo(executable, input_data):
    try:
        # Run with input data piped to stdin
        process = subprocess.run(
            [f"./{executable}"],
            input=input_data,
            text=True,
            capture_output=True,
            check=True
        )
        # Parse stderr for the time
        for line in process.stderr.splitlines():
            if line.startswith("Time:"):
                # Format: "Time: 0.1234 s"
                return float(line.split()[1])
    except subprocess.CalledProcessError as e:
        print(f"Error running {executable}: {e}")
        return None
    return 0.0


def run_test_suite(label, generator_func, sizes,
                   num_inputs, runs_per_input, executables):
    print(f"\n--- {label} Analysis ---")
    print(f"{'N':<10} | {'RIC (s)':<15} | {'Graham (s)':<15}")
    print("-" * 45)

    results = {
        'sizes': sizes,
        'ric': [],
        'graham': []
    }

    for n in sizes:
        input_avgs_ric = []
        input_avgs_graham = []
        for _ in range(num_inputs):
            data = generator_func(n)

            curr_in_times_ric = []
            curr_in_times_graham = []
            for _ in range(runs_per_input):
                t_ric = run_algo("ric", data)
                if t_ric is not None:
                    curr_in_times_ric.append(t_ric)
                t_graham = run_algo("graham_scan", data)
                if t_graham is not None:
                    curr_in_times_graham.append(t_graham)

            if curr_in_times_ric:
                input_avgs_ric.append(statistics.mean(curr_in_times_ric))
            if curr_in_times_graham:
                input_avgs_graham.append(statistics.mean(curr_in_times_graham))

        final_ric = (statistics.mean(input_avgs_ric)
                     if input_avgs_ric else 0.0)
        final_graham = (statistics.mean(input_avgs_graham)
                        if input_avgs_graham else 0.0)

        results['ric'].append(final_ric)
        results['graham'].append(final_graham)

        print(f"{n:<10} | {final_ric:<15.6f} | {final_graham:<15.6f}")

    return results


def plot_results(avg_results, worst_results):
    plt.figure(figsize=(12, 6))

    # Subplot 1: Average Case
    plt.subplot(1, 2, 1)
    plt.plot(avg_results['sizes'], avg_results['ric'],
             label='RIC', marker='o')
    plt.plot(avg_results['sizes'], avg_results['graham'],
             label='Graham Scan', marker='x')
    plt.title('Average Case (Random Points)')
    plt.xlabel('Input Size (N)')
    plt.ylabel('Time (s)')
    plt.legend()
    plt.grid(True)

    # Subplot 2: Worst Case
    plt.subplot(1, 2, 2)
    plt.plot(worst_results['sizes'], worst_results['ric'],
             label='RIC', marker='o')
    plt.plot(worst_results['sizes'], worst_results['graham'],
             label='Graham Scan', marker='x')
    plt.title('Worst Case (Points on Circle)')
    plt.xlabel('Input Size (N)')
    plt.ylabel('Time (s)')
    plt.legend()
    plt.grid(True)

    plt.tight_layout()
    plt.savefig('benchmark_plot.png')
    print("\nGraph saved to benchmark_plot.png")


def main():
    sizes = list(range(1000, 31000, 1000))
    num_inputs = 5
    runs_per_input = 3
    executables = [("ric", "ric.cpp"), ("graham_scan", "graham_scan.cpp")]

    for exe, src in executables:
        compile_cpp(src, exe)

    avg_results = run_test_suite(
        "Average Case (Uniform Random)",
        generate_random_points,
        sizes,
        num_inputs,
        runs_per_input,
        executables
    )
    worst_results = run_test_suite(
        "Worst Case (Points on Circle)",
        generate_circle_points,
        sizes,
        num_inputs,
        runs_per_input,
        executables
    )
    plot_results(avg_results, worst_results)


if __name__ == "__main__":
    main()
