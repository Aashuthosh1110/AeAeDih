#include <iostream>
#include <vector>
#include <cstdlib>  
#include <ctime>    
#include <algorithm> 

struct Edge {
    int u, v;
};


struct Graph {
    int V; // Number of vertices
    std::vector<Edge> edges;
};

// Performs a single run of Karger's contraction algorithm.
// We pass the graph 'g' by value to get a fresh copy for this run.
int kargerSingleRun(Graph g) {
    int current_vertices = g.V; // Start with the original number of vertices

    // Loop until only 2 "supernodes" remain
    while (current_vertices > 2) {
        
        // 1. Pick a random edge to contract.
        //I need to loop until we find an edge that is NOT a self-loop.
        int edge_index;
        do {
            // rand() % g.edges.size() picks a random index from 0 to (num_edges - 1)
            edge_index = rand() % g.edges.size();
        } while (g.edges[edge_index].u == g.edges[edge_index].v);

        // Get the two vertices of the chosen edge
        int u = g.edges[edge_index].u;
        int v = g.edges[edge_index].v;

        // 2. Contract edge (u, v): merge vertex 'v' into vertex 'u'.
        current_vertices--;

        // Iterate through all edges...
        for (int i = 0; i < g.edges.size(); ++i) {
            // ...and "re-wire" any edge connected to 'v' so it now connects to 'u'.
            if (g.edges[i].u == v) {
                g.edges[i].u = u;
            }
            if (g.edges[i].v == v) {
                g.edges[i].v = u;
            }
        }
    }

    // 3. Count the final cut
    int cut_count = 0;
    // After the loop, only 2 supernodes are left.
    // The edges remaining between them form the cut.
    for (int i = 0; i < g.edges.size(); ++i) {
        if (g.edges[i].u != g.edges[i].v) {
            cut_count++;
        }
    }
    return cut_count;
}

// Runs the Monte Carlo algorithm 'iterations' times to find the min cut
int kargerMinCut(const Graph& original_graph, int iterations) {
    // A simple, large number to represent "infinity"
    int min_cut = 999999; 

    // Run the algorithm many times
    for (int i = 0; i < iterations; ++i) {
        // Pass the original graph by value, which creates a copy
        // for kargerSingleRun to modify.
        int current_cut = kargerSingleRun(original_graph);
        
        // Keep track of the smallest cut found so far
        min_cut = std::min(min_cut, current_cut);
    }
    return min_cut;
}

// Main function to get user input and run the algorithm
int main() {
    // Seed the random number generator ONCE at the start of the program
    srand(time(NULL));

    int V, E;
    
    std::cout << "Enter the number of vertices: ";
    std::cin >> V;
    
    Graph g;
    g.V = V;

    std::cout << "Enter the number of edges: ";
    std::cin >> E;

    std::cout << "Enter " << E << " edges\n";
    for (int i = 0; i < E; ++i) {
        int u, v;
        std::cout << "Edge " << i+1 << ": ";
        std::cin >> u >> v;
        
        // Basic validation
        if (u < 0 || u >= V || v < 0 || v >= V) {
            std::cout << "Invalid vertex number. Vertices must be between 0 and " << V-1 << ".\n";
            i--; // Retry this edge
        } else {
            g.edges.push_back({u, v});
        }
    }

    // A simple choice for the number of repetitions.
    // V*V or V*V*log(V) are common theoretical suggestions.
    int iterations = V * V; 
    
    std::cout << "\nGraph has " << g.V << " vertices and " << g.edges.size() << " edges." << std::endl;
    std::cout << "Running Karger's algorithm " << iterations << " times..." << std::endl;
    
    int min_cut = kargerMinCut(g, iterations);
    
    std::cout << "The minimum cut found is: " << min_cut << std::endl;

    return 0;
}