#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <string>
#include <fstream>   // For reading files
#include <chrono>    // For measuring time
#include <map>       // For the simple DSU (easier than the previous one)

// A simple structure to represent an edge (u, v)
struct Edge {
    int u, v;
};

// Represents the graph
struct Graph {
    int V; // Number of vertices
    std::vector<Edge> edges; // List of all edges

    // Function to load a graph from a filename
    bool loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file " << filename << std::endl;
            return false;
        }

        int E;
        file >> V >> E; // Read vertex and edge counts
        edges.clear();
        for (int i = 0; i < E; ++i) {
            int u, v;
            file >> u >> v;
            edges.push_back({u, v});
        }
        file.close();
        return true;
    }
};

// --- This is the Karger's Algorithm code from before ---

// This simple DSU uses a map, which is less efficient but very easy
// to understand. It maps a vertex to its "supernode" representative.
struct DSU {
    std::map<int, int> parent;
    DSU(int n) {
        for(int i = 0; i < n; ++i) {
            parent[i] = i;
        }
    }
    int find(int i) {
        if (parent[i] == i) return i;
        return parent[i] = find(parent[i]);
    }
    void unite(int i, int j) {
        int root_i = find(i);
        int root_j = find(j);
        if (root_i != root_j) {
            parent[root_i] = root_j;
        }
    }
};

int kargerSingleRun(const Graph& g) {
    int V = g.V;
    DSU dsu(V);
    int supernodes = V;
    std::vector<Edge> current_edges = g.edges;

    while (supernodes > 2) {
        int edge_index = rand() % current_edges.size();
        Edge& randomEdge = current_edges[edge_index];

        int set1 = dsu.find(randomEdge.u);
        int set2 = dsu.find(randomEdge.v);

        if (set1 != set2) {
            supernodes--;
            dsu.unite(set1, set2);
        }
        // Remove this edge to simulate contraction & avoid re-picking
        // This is a simple (though not most efficient) way to handle it
        current_edges.erase(current_edges.begin() + edge_index);
        
        // Stop if we run out of edges (for disconnected graphs)
        if (current_edges.empty()) break;
    }

    int cut_count = 0;
    for (const auto& edge : g.edges) {
        if (dsu.find(edge.u) != dsu.find(edge.v)) {
            cut_count++;
        }
    }
    return cut_count;
}

int kargerMinCut(const Graph& original_graph, int iterations) {
    int min_cut = 999999; 
    for (int i = 0; i < iterations; ++i) {
        int current_cut = kargerSingleRun(original_graph);
        min_cut = std::min(min_cut, current_cut);
    }
    return min_cut;
}

// --- End of Karger's Algorithm code ---


// --- Experiment 1: Runtime vs. Graph Size (n) ---
void runTimeExperiment() {
    std::cout << "--- Experiment 1: Runtime vs. Graph Size ---\n";
    std::cout << "Enter filenames to test (e.g., n10.txt n20.txt n50.txt), end with 'done':\n";
    
    std::string filename;
    std::cout << "n,time_ms\n"; // Header for CSV data

    while (std::cin >> filename && filename != "done") {
        Graph g;
        if (!g.loadFromFile(filename)) {
            continue; // Skip if file not found
        }
        
        int n = g.V;
        int iterations = n * n; // Use n^2 iterations as discussed

        // Measure time
        auto start = std::chrono::high_resolution_clock::now();
        
        kargerMinCut(g, iterations); // Run the algorithm
        
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed_ms = end - start;

        // Print data in CSV format
        std::cout << n << "," << elapsed_ms.count() << std::endl;
    }
}

// --- Experiment 2: Success Rate vs. Iterations (T) ---
void runSuccessRateExperiment() {
    std::cout << "--- Experiment 2: Success Rate vs. Iterations ---\n";
    std::string filename;
    std::cout << "Enter one filename to test (e.g., n20.txt): ";
    std::cin >> filename;

    Graph g;
    if (!g.loadFromFile(filename)) {
        return;
    }

    int correct_answer;
    std::cout << "Enter the correct min cut for this graph (e.g., 2): ";
    std::cin >> correct_answer;

    int num_trials;
    std::cout << "Enter number of trials to run (e.g., 200): ";
    std::cin >> num_trials;
    
    std::cout << "\nIterations,SuccessRate\n"; // Header for CSV data

    // Test for different numbers of iterations (T)
    std::vector<int> T_values = {1, 5, 10, 20, 50, 100, 150};
    // For n^2, we'll use g.V * g.V
    T_values.push_back(g.V * g.V);

    for (int T : T_values) {
        int success_count = 0;
        for (int i = 0; i < num_trials; ++i) {
            if (kargerMinCut(g, T) == correct_answer) {
                success_count++;
            }
        }
        double success_rate = static_cast<double>(success_count) / num_trials;
        std::cout << T << "," << success_rate << std::endl;
    }
}

int main() {
    // Seed the random number generator ONCE
    srand(time(NULL));

    int choice;
    std::cout << "Select your experiment:\n";
    std::cout << "  1: Runtime vs. Graph Size (n)\n";
    std::cout << "  2: Success Rate vs. Iterations (T)\n";
    std::cout << "Enter choice (1 or 2): ";
    std::cin >> choice;

    if (choice == 1) {
        runTimeExperiment();
    } else if (choice == 2) {
        runSuccessRateExperiment();
    } else {
        std::cout << "Invalid choice." << std::endl;
    }

    return 0;
}