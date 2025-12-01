#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <string>
#include <fstream>
#include <cmath>
#include <chrono>

struct Edge { int u, v; };

struct Graph {
    int V;
    std::vector<Edge> edges;

    bool loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) return false;
        int E;
        file >> V >> E;
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


Graph contractTo(Graph g, int k) {
    int current_vertices = g.V;
    while (current_vertices > k && !g.edges.empty()) {
        int idx = rand() % g.edges.size();
        int u = g.edges[idx].u;
        int v = g.edges[idx].v;
        
      
        if (u == v) {
            g.edges.erase(g.edges.begin() + idx);
            continue;
        }
        
        current_vertices--;
        for (auto& edge : g.edges) {
            if (edge.u == v) edge.u = u;
            if (edge.v == v) edge.v = u;
        }
        g.edges.erase(g.edges.begin() + idx);
    }
    
   
    g.V = current_vertices; 
    return g;
}

int recursiveMinCut(Graph g) {
   
    if (g.edges.empty()) return 0;

    if (g.V <= 6) {
        return contractTo(g, 2).edges.size();
    }
    
    int t = std::ceil(1.0 + g.V / 1.41421356);

    Graph g1 = contractTo(g, t);
    int res1 = recursiveMinCut(g1);
    
  
    Graph g2 = contractTo(g, t);
    int res2 = recursiveMinCut(g2);

    return std::min(res1, res2);
}

int main() {
    srand(time(NULL));
    std::string filename;
    
    std::cout << "n,time_ms" << std::endl;

    while (std::cin >> filename && filename != "done") {
        Graph g;
        if (!g.loadFromFile(filename)) continue;

        
        int iterations = 100; 

        auto start = std::chrono::high_resolution_clock::now();
        
        int min_cut = 999999;
        for(int i=0; i<iterations; ++i) {
            
            int res = recursiveMinCut(g);
            if(res < min_cut) min_cut = res;
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - start;

        std::cout << g.V << "," << elapsed.count() << std::endl;
    }
    return 0;
}