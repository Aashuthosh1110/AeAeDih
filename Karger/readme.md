# Karger's Min Cut & Karger-Stein Algorithm Project

This repository contains implementations and empirical analysis of Karger's randomized algorithm and its optimized variant, the Karger-Stein algorithm.

## 🛠️ Dependencies & Prerequisites

To run this project, you need the following installed on your system:

1.  **C++ Compiler:** `g++` (Must support C++11 or higher).
2.  **Python 3:** Used for test automation and plotting.
3.  **Matplotlib:** Python library for generating the graphs.
    * **Install command:**
        ```bash
        pip install matplotlib
        ```

---

## 📂 Project Structure & Logic

### Why do we have both C++ and Python files?
* **C++ (`.cpp`):** Karger's algorithm is computationally intensive (requiring millions of graph contractions). C++ is used for the core algorithm logic to ensure the experiments finish in a reasonable amount of time (high performance).
* **Python (`.py`) & Matplotlib:** Python is used as a "wrapper" or "test harness." It automates the process of running the C++ executable hundreds of times, captures the output data, and uses the **Matplotlib** library to draw professional graphs (Runtime vs. Size, Success Rate vs. Iterations) for the report.

---

## 🚀 Compilation & Execution Guide

**CRITICAL SETUP NOTE:**
The Python scripts (`graph.py`, `graph2.py`, etc.) assume that the **datasets** (`.txt` files) and the **compiled C++ executables** are in the *same directory* as the script. Before running Python scripts in specific folders, you may need to copy the `.txt` files from `Datasets/` and the compiled executables into that folder.

### 1. Dataset Generation
**Location:** `Datasets/`
* **File:** `worstcasegen.py` (or `generate_all_datasets.py`)
* **Purpose:** Generates the graph text files (cycle graphs, dense graphs) used for testing.
* **How to Run:**
    ```bash
    cd Datasets
    python worstcasegen.py
    ```

### 2. Basic Karger's Algorithm 
**Location:** `Karger/Basic Kargers/`

* **`benchmark.cpp`**: The main test harness for Basic Karger. It supports both runtime and success rate modes. Utilized to generate the different graphs
* **`Implementation.cpp`**: A standalone version 

**How to Compile & Run:**
```bash
cd "Karger/Basic Kargers"
g++ -std=c++11 benchmark.cpp -o benchmark_program
# To run manually:
./benchmark_program

3. Karger-Stein Algorithm (Bonus):
benchmark_stein.cpp: Implements Karger-Stein logic but formats output specifically for the Success Rate experiment (compatible with graph2.py).

stein_runtime.cpp: Implements Karger-Stein logic optimized to measure wall-clock time for the Runtime experiment.

plot_stein_runtime.py: A Python script that compiles stein_runtime.cpp and generates the "Runtime vs. Size" graph.

How to Run (Runtime Graph):

    Bash
        cd karger-stein
        # Ensure n10.txt...n50.txt are copied into this folder first from the datasets folder
        python plot_stein_runtime.py

How to Compile (For Success Rate):

    Bash

        cd karger-stein
        g++ -std=c++11 benchmark_stein.cpp -o stein_program


4. Graphing & Analysis Scripts

graph.py: Automates the Runtime vs. Size experiment for Basic Karger.

Requires: benchmark_program (from Basic Kargers) and datasets (n10.txt...) to be present in this folder.

Run: python graph.py

graph2.py: Automates the Success Rate vs. Iterations experiment.

Requires: Either benchmark_program (for Basic) or stein_program (for Stein), plus the specific dataset (e.g., n50_needle.txt) to be in this folder.

Configuration: You must edit the CPP_EXECUTABLE line inside this script to point to the specific algorithm you want to test (Basic or Stein).

Run: python graph2.py

