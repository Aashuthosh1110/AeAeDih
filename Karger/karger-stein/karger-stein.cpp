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


Graph contractTo(Graph g, int k) {
    int current_vertices = g.V;
    
    while (current_vertices > k) {
        int edge_index;
        do {
            edge_index = rand() % g.edges.size();
        } while (g.edges[edge_index].u == g.edges[edge_index].v);

        int u = g.edges[edge_index].u;
        int v = g.edges[edge_index].v;

        current_vertices--;
        for (auto& edge : g.edges) {
            if (edge.u == v) edge.u = u;
            if (edge.v == v) edge.v = u;
        }
    }
    

    std::vector<Edge> clean_edges;
    for (const auto& edge : g.edges) {
        if (edge.u != edge.v) {
            clean_edges.push_back(edge);
        }
    }
    g.edges = clean_edges;
    g.V = current_vertices; 
    return g;
}

int recursiveMinCut(Graph g) {
    int n = g.V;

  
    if (n <= 6) {
        Graph final_g = contractTo(g, 2);
        return final_g.edges.size();
    }

 
    int t = std::ceil(1.0 + n / 1.41421356);

   
    Graph g1 = contractTo(g, t); 
    int cut1 = recursiveMinCut(g1);


    Graph g2 = contractTo(g, t);
    int cut2 = recursiveMinCut(g2);


    return std::min(cut1, cut2);
}


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

   
    int iterations = 100; 

    std::cout << "Graph loaded. V=" << V << ", E=" << E << std::endl;
    std::cout << "Running Karger-Stein " << iterations << " times..." << std::endl;

    int result = runKargerStein(g, iterations);
    std::cout << "Min cut found: " << result << std::endl;

    return 0;
}