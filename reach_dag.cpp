/* Reach a vertex in a Directed Acyclic Graph (DAG) problem

    Write a function that inputs 2 vertices (starting vertex s, terminal vertex t), along with
    a positive integer T, and checks if you can go from s to t in exactly T steps

    G is a DAG with vertices V, and edges E                                 G(V, E)
    V holds all vertices

Author Info:
Kshitij Pingle
pinglekshitij15@gmail.com

10 October, 2026
Saturday


Test Case 1:
Graph:                a
                2/    |    \6
               s      |2     t
                3\    |    /3
                      b

Vertices:
         s, a, b, t
    V = [0, 1, 2, 3]


Test Case 2:
Graph:
       (3)         (4)
    /------> [a] ------> [c] \ (2)
   /          |           ^   \
[s]           |(1)     (1)|    \-----> [t]
   \          v           |           /
    \------> [b] ---------/          /
       (1)         (5)              / 
   \-------------------------------/
                (6)

Vertices:
         s, a, b, c, t
    V = [0, 1, 2, 3, 4]

*/

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

#include "graph.h"

int DEBUG = 0;      // 0 = False

// Function forward declarations
int reach_recursive(int vertex, int T, graph::Graph& graph);
void test(std::vector<std::string>& names, int T, graph::Graph& graph);
std::vector<std::vector<int>> reach(int vertex, int T, std::vector<std::string>& names, graph::Graph& graph);
void print_dp_table(const std::vector<std::vector<int>>& table, const std::vector<std::string>& names, int T);


int main() {

    // Test Case 1
    std::vector<std::string> names = {"s", "a", "b", "t"};
    graph::Graph graph(names.size(), names);

    // Test Case 1 edges: (u, v, weight)
    graph.addEdge(0, 1, 2); // s -> a (wt 2)
    graph.addEdge(0, 2, 3); // s -> b (wt 3)
    graph.addEdge(1, 2, 2); // a -> b (wt 2)
    graph.addEdge(1, 3, 6); // a -> t (wt 6)
    graph.addEdge(2, 3, 3); // b -> t (wt 3)

    int s = 0, a = 1, b = 2, t = 3, T = 7;

    if (DEBUG) {
        // Access parent edges for node t via in_adj
        std::cout << "Parents of node t (" << graph.labels[t] << "):\n";
        for (const auto& parent_edge : graph.in_adj[t]) {
            int parent_index = parent_edge.from;
            int weight = parent_edge.weight;
            std::cout << "  - " << graph.labels[parent_index] 
                    << " with edge weight " << weight << "\n";
        }
    }

    // // Test all vertices recursively
    // test(names, T, graph);

    std::vector<std::vector<int>> dp_table = reach(t, T, names, graph);

    print_dp_table(dp_table, names, T);




    // Test Case 2
    std::vector<std::string> names2 = {"s", "a", "b", "c", "t"};
    graph::Graph graph2(names2.size(), names2);

    // Edges: (u, v, weight)
    graph2.addEdge(0, 1, 3); // s -> a (wt 3)
    graph2.addEdge(0, 2, 1); // s -> b (wt 1)
    graph2.addEdge(0, 4, 6); // s -> t (wt 6)
    graph2.addEdge(1, 2, 1); // a -> b (wt 1)
    graph2.addEdge(1, 3, 4); // a -> c (wt 4)
    graph2.addEdge(2, 3, 5); // b -> c (wt 5)
    graph2.addEdge(3, 4, 2); // c -> t (wt 2)

    int T_max = 10;
    int t_idx = 4; // index of terminal vertex t

    // Run iterative reach table generator
    dp_table = reach(t_idx, T_max, names2, graph2);

    print_dp_table(dp_table, names2, T_max);


    return 0;
}

int reach_recursive(int vertex, int T, graph::Graph& graph) {
    // Base Cases
    if ((vertex == 0) && (T > 0)) {
        // Reach(s, T > 0) -> False
        return 0;
    }
    if (T < 0) {
        return 0;
    }
    if ((vertex == 0) && (T == 0)) {
        // Reach(s, T = 0) -> True
        return 1;
    }
    if ((vertex != 0) && (T == 0)) {
        return 0;
    }

    int max = 0;

    // recurse through parent vertices
    for (const auto& parent_edge : graph.in_adj[vertex]) {
        max = reach_recursive(parent_edge.from, T - parent_edge.weight, graph);

        if (max == 1) {
            break;
        }
    }

    return max;
}

std::vector<std::vector<int>> reach(int vertex, int T, std::vector<std::string>& names, graph::Graph& graph) {

    // DP Table: table[v][len] = 1 if path exists from s to v of length 'len', else 0
    std::vector<std::vector<int>> table(names.size(), std::vector<int>(T + 1, 0));

    table[0][0] = 1;    // Base Case: Reach(s, 0) = 1

    // Iterate through lengths first
    //   Allows us to fill all columns in a row first, then move onto the next row
    for (int j = 0; j <= T; ++j) {

        // Iterate through vertices
        for (int i = 1; i < names.size(); ++i) {
            int val = 0;

            // Iterate over parents of vertex i
            for (const auto& parent_edge : graph.in_adj[i]) {
                int parent = parent_edge.from;
                int weight = parent_edge.weight;

                // Weight for next calculations must be positive
                if ((j - weight) >= 0) {
                    if (table[parent][j - weight] == 1) {
                        val = 1;
                        break;          // Path found
                    }
                }
            }

            table[i][j] = val;
        }
    }

    return table;
}

void test(std::vector<std::string>& names, int T, graph::Graph& graph) {
    for (size_t i = 0; i < names.size(); ++i) {
        for (size_t j = 0; j <= T; ++j) {
            std::cout << "Reached(" << names[i] << ", " << j << ") = " << reach_recursive(i, j, graph) << std::endl;
        }
    }
}

// Helper function to print the resulting 2D table cleanly
//  Helper function made by Gemini
void print_dp_table(const std::vector<std::vector<int>>& table, 
                    const std::vector<std::string>& names, 
                    int T) {
    std::cout << "\n--- DP Table (Rows: Vertices, Columns: Distance 0 to " << T << ") ---\n" << std::endl;
    std::cout << std::setw(8) << "V \\ T";
    for (int j = 0; j <= T; ++j) {
        std::cout << std::setw(4) << j;
    }
    std::cout << "\n" << std::string(8 + (T + 1) * 4, '-') << "\n";

    for (size_t i = 0; i < names.size(); ++i) {
        std::cout << std::setw(6) << names[i] << " |";
        for (int j = 0; j <= T; ++j) {
            std::cout << std::setw(4) << table[i][j];
        }
        std::cout << "\n";
    }

    std::cout << "\n";
}