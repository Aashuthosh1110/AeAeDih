#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <string>
#include <fstream>
#include <cmath>

struct Edge {
    int u, v;
};

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
    
  
    std::vector<Edge> clean;
    for(auto& e : g.edges) {
        if(e.u != e.v) clean.push_back(e);
    }
    g.edges = clean;
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
    int res1 = recursiveMinCut(g1);

 
    Graph g2 = contractTo(g, t); 
    int res2 = recursiveMinCut(g2);

    return std::min(res1, res2);
}



void runSuccessRateExperiment() {
    std::string filename;
    std::cin >> filename;

    Graph g;
    if (!g.loadFromFile(filename)) {
        std::cerr << "Error loading file." << std::endl;
        return;
    }

    int correct_answer;
    std::cin >> correct_answer;

    int num_trials;
    std::cin >> num_trials;
    
    std::cout << "Iterations,SuccessRate" << std::endl;

  
    std::vector<int> T_values = {1, 5, 10, 20, 50, 100, 150};

    for (int T : T_values) {
        int success_count = 0;
        for (int i = 0; i < num_trials; ++i) {
            int min_found = 999999;
            
            
            for(int k=0; k<T; ++k) {
                int val = recursiveMinCut(g);
                if(val < min_found) min_found = val;
            }

            if (min_found == correct_answer) {
                success_count++;
            }
        }
        double rate = (double)success_count / num_trials;
        std::cout << T << "," << rate << std::endl;
    }
}

int main() {
    srand(time(NULL));
    int choice;
    std::cin >> choice;
    runSuccessRateExperiment();
    return 0;
}