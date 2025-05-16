// This program finds a spanning tree with the maximum number of leaf nodes (Maximum Leaf Spanning Tree Problem)
// It uses exhaustive search: tries all possible combinations of edges that could form a spanning tree
// For each valid spanning tree, it counts the number of leaves (nodes with degree 1)
// The tree with the most leaves is saved and displayed at the end
//
// Key Concepts:
// - Spanning Tree: A subset of edges that connects all nodes without cycles (n-1 edges for n nodes)
// - Leaf Node: A node with only one connection (degree 1)
// - Exhaustive Search: Tries every possible combination to guarantee the best answer
//
// Main Steps:
// 1. Generate all combinations of n-1 edges from the input graph
// 2. For each combination, check if it forms a connected tree (using Union-Find/DSU)
// 3. Count the number of leaves in the tree
// 4. Track and print the best tree found (with the most leaves)
//
// Usage:
// - Edit the test cases in main() to try different graphs
// - Run the program to see all valid spanning trees and the one with the most leaves

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NODES 30
#define MAX_EDGES 40

typedef struct {
    int u, v;
} Edge;

// These help check if a set of edges forms a connected tree (no cycles, all nodes connected)
// Each node starts as its own parent. 'find' locates the root of a node's set.
int find(int x, int parent[]) {
    // No loop here, just recursion
    if (parent[x] != x)
        parent[x] = find(parent[x], parent); // Path compression for efficiency
    return parent[x];
}

// 'unite' joins two sets (nodes x and y) together
void unite(int x, int y, int parent[], int rank[]) {
    int rootX = find(x, parent);
    int rootY = find(y, parent);
    // Only unite if roots are different (to avoid cycles)
    if (rootX != rootY) {
        // Attach smaller tree to larger tree for efficiency
        if (rank[rootX] < rank[rootY])
            parent[rootX] = rootY;
        else
            parent[rootY] = rootX;
        // If ranks are equal, increase the rank
        if (rank[rootX] == rank[rootY])
            rank[rootX]++;
    }
}

// Check if the combination of edges forms a connected graph
// Returns 1 if all nodes are connected (i.e., the edges form a spanning tree), 0 otherwise
int is_connected(Edge *combo, int k, int n) {
    int parent[MAX_NODES], rank[MAX_NODES];
    // Loop to initialize each node's parent and rank
    for (int i = 0; i < n; i++) {
        parent[i] = i;
        rank[i] = 0;
    }

    // Loop through all edges in the combination to unite their nodes
    for (int i = 0; i < k; i++) {
        Edge e = combo[i];
        unite(e.u, e.v, parent, rank);
    }

    int root = find(0, parent);
    // Loop to check if all nodes are connected to the same root
    for (int i = 1; i < n; i++) {
        if (find(i, parent) != root)
            return 0; // Found a node not connected to the root
    }
    return 1; // All nodes are connected
}

// Count number of leaf nodes (nodes with degree 1) in the current tree
// Returns the number of leaves
int count_leaves(Edge *combo, int k, int n) {
    int degree[MAX_NODES] = {0};
    // Loop through all edges to count the degree of each node
    for (int i = 0; i < k; i++) {
        degree[combo[i].u]++;
        degree[combo[i].v]++;
    }

    int count = 0;
    // Loop through all nodes to count how many have degree 1 (leaves)
    for (int i = 0; i < n; i++) {
        if (degree[i] == 1) // Leaf node has degree 1
            count++;
    }
    return count;
}

// Print the current combination of edges and its leaf count
// Also prints the degree of each node in this tree
void print_combo_and_leaf_count(Edge *combo, int k, int n, int is_best) {
    printf("Combination: ");
    // Loop to print all edges in the current combination
    for (int i = 0; i < k; i++) {
        printf("(%d-%d) ", combo[i].u, combo[i].v);
    }

    int degree[MAX_NODES] = {0};
    // Loop to count the degree of each node in the current combination
    for (int i = 0; i < k; i++) {
        degree[combo[i].u]++;
        degree[combo[i].v]++;
    }

    printf(" | Degrees: ");
    // Loop to print the degree of each node
    for (int i = 0; i < n; i++) {
        printf("%d:%d ", i, degree[i]);
    }

    int leaves = count_leaves(combo, k, n);
    printf(" | Leaves: %d", leaves);

    if (is_best) {
        printf(" [BEST SO FAR]");
    }

    printf("\n");
}

// Global storage for best tree
Edge best_tree[MAX_EDGES];
int best_leaf_count = 0;

// Recursive function to generate all combinations of k edges from the list of edges
// For each combination, checks if it forms a valid spanning tree and updates the best tree if needed
/**
 * Generates all possible combinations of k edges from a given set of m edges.
 *
 * @param edges   Pointer to the array of all available edges.
 * @param m       Total number of available edges in the edges array.
 * @param k       Number of edges to select for each combination.
 * @param n       Total number of vertices in the graph (may be used for validation or further processing).
 * @param start   Current starting index in the edges array for combination generation.
 * @param current Pointer to the array holding the current combination of edges being constructed.
 * @param cpos    Current position in the current combination array to insert the next edge.
 */
void generate_combinations(Edge *edges, int m, int k, int n, int start, Edge *current, int cpos) {
    // Step 1: Base case - if we've picked k edges, process this combination
    if (cpos == k) {
        // Step 2: Check if the current combination forms a valid spanning tree
        if (is_connected(current, k, n)) {
            int leaves = count_leaves(current, k, n);
            static int combo_index = 1;
            printf("Valid Spanning Tree #%d | Leaves: %d\n", combo_index++, leaves);
            print_combo_and_leaf_count(current, k, n, 0);

            // Step 3: If this tree has more leaves than the best so far, update the best
            if (leaves > best_leaf_count) {
                best_leaf_count = leaves;
                memcpy(best_tree, current, k * sizeof(Edge));
                printf("  [New Best Tree Found]\n");
            }
        }
        // Step 4: Return to stop further recursion for this combination
        return;
    }

    // Step 5: Recursive case - try each possible edge at the current position
    for (int i = start; i < m; i++) {
        current[cpos] = edges[i]; // Place edge at current position
        // Step 6: Recurse to fill the next position in the combination
        generate_combinations(edges, m, k, n, i + 1, current, cpos + 1);
    }
}

// Print the adjacency matrix of the best spanning tree found
// Shows which nodes are connected in the best tree
void print_adjacency_matrix(int n) {
    int matrix[MAX_NODES][MAX_NODES] = {0};

    // Loop through all edges in the best tree to fill the adjacency matrix
    for (int i = 0; i < n - 1; i++) {
        matrix[best_tree[i].u][best_tree[i].v] = 1;
        matrix[best_tree[i].v][best_tree[i].u] = 1;
    }

    printf("\nAdjacency Matrix of Best Spanning Tree:\n");
    printf("   ");
    // Loop to print column headers (node numbers)
    for (int i = 0; i < n; i++) {
        printf("%2d ", i);
    }
    printf("\n");

    // Loop to print each row of the adjacency matrix
    for (int i = 0; i < n; i++) {
        printf("%2d ", i);
        for (int j = 0; j < n; j++) {
            printf("%2d ", matrix[i][j]);
        }
        printf("\n");
    }
}

// Print the best spanning tree found (with the most leaves)
// Shows the edges and the degree of each node in the best tree
void print_best_tree(int n) {
    printf("\nFinal Best Spanning Tree with %d leaves:\n", best_leaf_count);
    printf("Edges: ");
    // Loop to print all edges in the best tree
    for (int i = 0; i < n - 1; i++) {
        printf("(%d-%d) ", best_tree[i].u, best_tree[i].v);
    }
    printf("\n");

    int degree[MAX_NODES] = {0};
    // Loop through all edges in the best tree to count degrees for each node
    for (int i = 0; i < n - 1; i++) {
        degree[best_tree[i].u]++;
        degree[best_tree[i].v]++;
    }

    printf("Node Degrees: ");
    // Loop through all nodes to print their degree in the best tree
    for (int i = 0; i < n; i++) {
        printf("%d:%d ", i, degree[i]);
    }
    printf("\n");
}

int main() {
    // Test cases for different types of graphs are provided below.
    // Uncomment the test case you want to run, or add your own.
    // Each test case shows the expected maximum number of leaves for that graph.

    // Test Case 1: Complete Graph K4
    // Expected Max Leaves: 3
    // const int N = 4;
    // Edge edges[] = {
    //     {0, 1}, {0, 2}, {0, 3}, {1, 2}, {1, 3}, {2, 3}
    // };
    // int m = 6;

    // Test Case 2: Path Graph (0-1-2-3)
    // Expected Max Leaves: 2
    // const int N = 4;
    // Edge edges[] = {
    //     {0, 1}, {1, 2}, {2, 3}
    // };
    // int m = 3;

    // Test Case 3: Star Graph (0 connected to 1,2,3)
    // Expected Max Leaves: 3
    // const int N = 4;
    // Edge edges[] = {
    //     {0, 1}, {0, 2}, {0, 3}
    // };
    // int m = 3;

    // Test Case 4: Cycle + Chord (0-1-2-3-0 + 0-2)
    // Expected Max Leaves: 3
    // const int N = 4;
    // Edge edges[] = {
    //     {0, 1}, {1, 2}, {2, 3}, {3, 0}, {0, 2}
    // };
    // int m = 5;

    // Test Case 5: Two triangles (0-1-2, 3-4-5), connected by bridge (2-3), plus extra edges
    // Expected Max Leaves: 4
    // const int N = 6;
    // Edge edges[] = {
    //     // Triangle 1
    //     {0, 1}, {1, 2}, {2, 0},
    //     // Triangle 2
    //     {3, 4}, {4, 5}, {5, 3},
    //     // Bridge
    //     {2, 3},
    //     // Optional extra edges
    //     {0, 5}, {1, 4}
    // };
    // int m = 9;

    // Test Case 6: 2x2 Grid Graph (Square)
    // Expected Max Leaves: 2
    // const int N = 4;
    // Edge edges[] = {
    //     {0, 1}, {0, 2}, {1, 3}, {2, 3}
    // };
    // int m = 4;

    // Test Case 7: Full Binary Tree (3 levels)
    // Nodes: 0 (root), 1-2 (level 1), 3-4-5-6 (level 2)
    // Expected Max Leaves: 4
    // const int N = 7;
    // Edge edges[] = {
    //     {0, 1}, {0, 2},
    //     {1, 3}, {1, 4},
    //     {2, 5}, {2, 6}
    // };
    // int m = 6;

    // Test Case 8: Wheel Graph (5 nodes: 0-center, 1-2-3-4)
    // Edges: Outer cycle (1-2-3-4-1) + spokes (0-1, 0-2, 0-3, 0-4)
    // Expected Max Leaves: 4
    // const int N = 5;
    // // Edge edges[] = {
    // //     {0, 1}, {0, 2}, {0, 3}, {0, 4},
    // //     {1, 2}, {2, 3}, {3, 4}, {4, 1}
    // // };
    // // int m = 8;

    // Test Case 9: Tree with Articulation Point
    // Graph: Triangle (0-1-2) connected to another triangle (2-3-4)
    // Expected Max Leaves: 4
    // const int N = 5;
    // Edge edges[] = {
    //     {0, 1}, {1, 2}, {2, 0},
    //     {2, 3}, {3, 4}, {4, 2}
    // };
    // int m = 6;

    // Test Case 10: Complete Graph K5
    // Expected Max Leaves: 4 (star-shaped tree)
    // const int N = 5;
    // Edge edges[] = {
    //     {0, 1}, {0, 2}, {0, 3}, {0, 4},
    //     {1, 2}, {1, 3}, {1, 4},
    //     {2, 3}, {2, 4},
    //     {3, 4}
    // };
    // int m = 10;

    // Test Case 11: Big graph with 30 nodes
    // Expected Max Leaves: 23
    const int N = 30;
    Edge edges[] = {
        // Binary tree structure (0 to 14)
        {0,1}, {0,2},
        {1,3}, {1,4},
        {2,5}, {2,6},
        {3,7}, {3,8},
        {4,9}, {4,10},
        {5,11}, {5,12},
        {6,13}, {6,14},

        // Extra leaves (15 to 26)
        {0,15}, {1,16}, {2,17}, {3,18}, {4,19}, {5,20},
        {6,21}, {0,22}, {1,23}, {2,24}, {3,25}, {4,26},

        // Extra leaves for redundancy
        {0,27}, {1,28},

        // Final node to keep graph connected
        {2,29}
    };
    int m = sizeof(edges)/sizeof(edges[0]);

    Edge current_combo[MAX_NODES];  // Temporary array to store current combination of edges

    printf("Exhaustive Search: Evaluating All Possible Spanning Trees\n");
    printf("----------------------------------------------------------\n");

    // add start timer
    clock_t start_time = clock();
    // Try all possible combinations of N-1 edges
    generate_combinations(edges, m, N - 1, N, 0, current_combo, 0);
    clock_t end_time = clock();

    printf("----------------------------------------------------------\n");
    printf("Exhaustive Search Complete: All combinations have been checked.\n\n");

    // Print the best tree found and its adjacency matrix
    print_best_tree(N);
    print_adjacency_matrix(N);

    double time_taken = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Time taken: %f seconds\n", time_taken);
    printf("----------------------------------------------------------\n");
    return 0;
}