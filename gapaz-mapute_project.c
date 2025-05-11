#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 10
#define MAX_EDGES 20

typedef struct {
    int u, v;
} Edge;

// DSU (Union-Find) functions
int find(int x, int parent[]) {
    if (parent[x] != x)
        parent[x] = find(parent[x], parent);
    return parent[x];
}

void unite(int x, int y, int parent[], int rank[]) {
    int rootX = find(x, parent);
    int rootY = find(y, parent);
    if (rootX != rootY) {
        if (rank[rootX] < rank[rootY])
            parent[rootX] = rootY;
        else
            parent[rootY] = rootX;
        if (rank[rootX] == rank[rootY])
            rank[rootX]++;
    }
}

// Check if the combination of edges forms a connected graph
int is_connected(Edge *combo, int k, int n) {
    int parent[MAX_NODES], rank[MAX_NODES];
    for (int i = 0; i < n; i++) {
        parent[i] = i;
        rank[i] = 0;
    }

    for (int i = 0; i < k; i++) {
        Edge e = combo[i];
        unite(e.u, e.v, parent, rank);
    }

    int root = find(0, parent);
    for (int i = 1; i < n; i++) {
        if (find(i, parent) != root)
            return 0;
    }
    return 1;
}

// Count number of leaf nodes (degree == 1)
int count_leaves(Edge *combo, int k, int n) {
    int degree[MAX_NODES] = {0};
    for (int i = 0; i < k; i++) {
        degree[combo[i].u]++;
        degree[combo[i].v]++;
    }

    int count = 0;
    for (int i = 0; i < n; i++) {
        if (degree[i] == 1)
            count++;
    }
    return count;
}

// Print current subtree combination and its leaf count
void print_combo_and_leaf_count(Edge *combo, int k, int n, int is_best) {
    printf("Combination: ");
    for (int i = 0; i < k; i++) {
        printf("(%d-%d) ", combo[i].u, combo[i].v);
    }

    int degree[MAX_NODES] = {0};
    for (int i = 0; i < k; i++) {
        degree[combo[i].u]++;
        degree[combo[i].v]++;
    }

    printf(" | Degrees: ");
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

// Recursive function to generate all combinations of k edges
void generate_combinations(Edge *edges, int m, int k, int n, int start, Edge *current, int cpos) {
    if (cpos == k) {
        if (is_connected(current, k, n)) {
            int leaves = count_leaves(current, k, n);
            static int combo_index = 1;
            printf("Valid Spanning Tree #%d | Leaves: %d\n", combo_index++, leaves);
            print_combo_and_leaf_count(current, k, n, 0);

            if (leaves > best_leaf_count) {
                best_leaf_count = leaves;
                memcpy(best_tree, current, k * sizeof(Edge));
                printf("  [New Best Tree Found]\n");
            }
        }
        return;
    }

    for (int i = start; i < m; i++) {
        current[cpos] = edges[i];
        generate_combinations(edges, m, k, n, i + 1, current, cpos + 1);
    }
}

// Function to print the adjacency matrix of the best tree
void print_adjacency_matrix(int n) {
    int matrix[MAX_NODES][MAX_NODES] = {0};

    for (int i = 0; i < n - 1; i++) {
        matrix[best_tree[i].u][best_tree[i].v] = 1;
        matrix[best_tree[i].v][best_tree[i].u] = 1;
    }

    printf("\nAdjacency Matrix of Best Spanning Tree:\n");
    printf("   ");
    for (int i = 0; i < n; i++) {
        printf("%2d ", i);
    }
    printf("\n");

    for (int i = 0; i < n; i++) {
        printf("%2d ", i);
        for (int j = 0; j < n; j++) {
            printf("%2d ", matrix[i][j]);
        }
        printf("\n");
    }
}

// Function to print the best tree
void print_best_tree(int n) {
    printf("\nFinal Best Spanning Tree with %d leaves:\n", best_leaf_count);
    printf("Edges: ");
    for (int i = 0; i < n - 1; i++) {
        printf("(%d-%d) ", best_tree[i].u, best_tree[i].v);
    }
    printf("\n");

    int degree[MAX_NODES] = {0};
    for (int i = 0; i < n - 1; i++) {
        degree[best_tree[i].u]++;
        degree[best_tree[i].v]++;
    }

    printf("Node Degrees: ");
    for (int i = 0; i < n; i++) {
        printf("%d:%d ", i, degree[i]);
    }
    printf("\n");
}

int main() {
    // Test Case 1: Complete Graph K4
    // Expected Max Leaves: 3
    const int N = 4;
    Edge edges[] = {
        {0, 1}, {0, 2}, {0, 3}, {1, 2}, {1, 3}, {2, 3}
    };
    int m = 6;

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

    Edge current_combo[MAX_NODES];  // Fixed-size array to avoid variable-length array error

    printf("Exhaustive Search: Evaluating All Possible Spanning Trees\n");
    printf("----------------------------------------------------------\n");

    generate_combinations(edges, m, N - 1, N, 0, current_combo, 0);

    printf("----------------------------------------------------------\n");
    printf("Exhaustive Search Complete: All combinations have been checked.\n\n");

    print_best_tree(N);
    print_adjacency_matrix(N);

    return 0;
}