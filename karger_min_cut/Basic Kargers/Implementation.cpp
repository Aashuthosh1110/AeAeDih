#include <iostream>
#include <vector>
#include <cstdlib>  
#include <ctime>    
#include <algorithm> 

struct Edge {
    int u, v;
};


struct Graph {
    int V;
    std::vector<Edge> edges;
};


int kargerSingleRun(Graph g) {
    int current_vertices = g.V; 

    while (current_vertices > 2) {
        
        
        int edge_index;
        do {
            edge_index = rand() % g.edges.size();
        } while (g.edges[edge_index].u == g.edges[edge_index].v);

        int u = g.edges[edge_index].u;
        int v = g.edges[edge_index].v;

        current_vertices--;

        for (int i = 0; i < g.edges.size(); ++i) {
            if (g.edges[i].u == v) {
                g.edges[i].u = u;
            }
            if (g.edges[i].v == v) {
                g.edges[i].v = u;
            }
        }
    }

 
    int cut_count = 0;
    for (int i = 0; i < g.edges.size(); ++i) {
        if (g.edges[i].u != g.edges[i].v) {
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

int main() {
   
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
        
   
        if (u < 0 || u >= V || v < 0 || v >= V) {
            std::cout << "Invalid vertex number. Vertices must be between 0 and " << V-1 << ".\n";
            i--; 
        } else {
            g.edges.push_back({u, v});
        }
    }


    int iterations = V * V; 
    
    std::cout << "\nGraph has " << g.V << " vertices and " << g.edges.size() << " edges." << std::endl;
    std::cout << "Running Karger's algorithm " << iterations << " times..." << std::endl;
    
    int min_cut = kargerMinCut(g, iterations);
    
    std::cout << "The minimum cut found is: " << min_cut << std::endl;

    return 0;
}