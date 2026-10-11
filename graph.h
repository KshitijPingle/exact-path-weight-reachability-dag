#pragma once

#include <vector>
#include <string>

// Namespace and class itself are not the same to avoid naming collisions
namespace graph {

    /**
     * @brief Graph class to use for coding practice on graph related problems
     */

    struct Edge {
        int from;
        int weight;
    };

    class Graph {
    public:
        int num_vertices;
        std::vector<std::string> labels;
        std::vector<std::vector<Edge>> adj;         // Forward edges (children)
        std::vector<std::vector<Edge>> in_adj;      // Backward edges (to remember parents)

        Graph(int n, const std::vector<std::string>& node_labels) 
            : num_vertices(n), labels(node_labels), adj(n), in_adj(n) {}

        void addEdge(int u, int v, int weight) {
            adj[u].push_back({v, weight});      // Child link
            in_adj[v].push_back({u, weight});   // Parent link
        }
    };

} // namespace Graph