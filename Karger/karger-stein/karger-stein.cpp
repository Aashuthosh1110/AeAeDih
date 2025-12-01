#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <cmath>     

struct Edge {
    int u, v;
};

struct Graph {
    int V;
    std::vector<Edge> edges;
};

// Helper: Contracts the graph 'g' until only 'k' vertices remain.
// Returns the contracted graph.
Graph contractTo(Graph g, int k) {
    int current_vertices = g.V;
    
    while (current_vertices > k) {
        int edge_index;
        // Pick random edge (not self-loop)
        do {
            edge_index = rand() % g.edges.size();
        } while (g.edges[edge_index].u == g.edges[edge_index].v);

        int u = g.edges[edge_index].u;
        int v = g.edges[edge_index].v;

        // Contract v into u
        current_vertices--;
        for (auto& edge : g.edges) {
            if (edge.u == v) edge.u = u;
            if (edge.v == v) edge.v = u;
        }
    }
    
    // Clean up self-loops before returning (optional optimization)
    // We create a new edge list without self-loops to keep the graph smaller
    std::vector<Edge> clean_edges;
    for (const auto& edge : g.edges) {
        if (edge.u != edge.v) {
            clean_edges.push_back(edge);
        }
    }
    g.edges = clean_edges;
    g.V = current_vertices; // Update vertex count logically
    return g;
}

// The Karger-Stein Recursive Algorithm
int recursiveMinCut(Graph g) {
    int n = g.V;

    // Base Case: If the graph is small (<= 6 vertices), 
    // just run the basic contraction down to 2.
    if (n <= 6) {
        Graph final_g = contractTo(g, 2);
        return final_g.edges.size();
    }

    // Recursive Step:
    // 1. Calculate the target size t = ceil(1 + n / sqrt(2))
    int t = std::ceil(1.0 + n / 1.41421356);

    // 2. Branch 1: Contract down to t, then recurse
    Graph g1 = contractTo(g, t); 
    int cut1 = recursiveMinCut(g1);

    // 3. Branch 2: Contract down to t (independent random choices), then recurse
    // Note: We pass 'g' again, which is the ORIGINAL graph at this level
    Graph g2 = contractTo(g, t);
    int cut2 = recursiveMinCut(g2);

    // 4. Return the minimum of the two branches
    return std::min(cut1, cut2);
}

// Wrapper to run Karger-Stein multiple times
int runKargerStein(const Graph& g, int iterations) {
    int min_cut = 999999;
    for(int i = 0; i < iterations; ++i) {
        int res = recursiveMinCut(g);
        if (res < min_cut) min_cut = res;
    }
    return min_cut;
}

int main() {
    srand(time(NULL));

    int V, E;
    std::cout << "Enter vertices and edges (or redirect from file): ";
    if (!(std::cin >> V >> E)) return 0;

    Graph g;
    g.V = V;
    for (int i = 0; i < E; ++i) {
        int u, v;
        std::cin >> u >> v;
        g.edges.push_back({u, v});
    }

    // Karger-Stein has a higher success probability per run.
    // We generally don't need n^2 runs. Let's try log^2(n) or just a fixed small number.
    // For comparison fairness, you can run it the same number of times as Basic,
    // OR run it significantly fewer times to show it achieves the same accuracy faster.
    int iterations = 100; 

    std::cout << "Graph loaded. V=" << V << ", E=" << E << std::endl;
    std::cout << "Running Karger-Stein " << iterations << " times..." << std::endl;

    int result = runKargerStein(g, iterations);
    std::cout << "Min cut found: " << result << std::endl;

    return 0;
}