#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 20

typedef struct {
    int adj[MAX_NODES][MAX_NODES];
    int degree[MAX_NODES];
    int n;
} Graph;

typedef struct {
    int u, v;
} Edge;

// Initialize graph
void init_graph(Graph *g, int n) {
    g->n = n;
    for (int i = 0; i < n; i++) {
        g->degree[i] = 0;
        for (int j = 0; j < n; j++) {
            g->adj[i][j] = 0;
        }
    }
}

// Add edge to graph
void add_edge(Graph *g, int u, int v) {
    g->adj[u][v] = g->adj[v][u] = 1;
    g->degree[u]++;
    g->degree[v]++;
}

// Compare function for sorting (descending degree)
static int *g_degree = NULL;
int compare(const void *a, const void *b) {
    int u = *(int *)a;
    int v = *(int *)b;
    return g_degree[v] - g_degree[u]; // Descending
}

void dfs_greedy(const Graph *g, int node, int *visited, int parent, Edge tree[], int *tree_index) {
    visited[node] = 1;

    if (parent != -1) {
        tree[*tree_index].u = parent;
        tree[*tree_index].v = node;
        (*tree_index)++;
    }

    int neighbors[MAX_NODES], neighbor_count = 0;
    for (int i = 0; i < g->n; i++) {
        if (g->adj[node][i] && !visited[i]) {
            neighbors[neighbor_count++] = i;
        }
    }

    g_degree = (int *)g->degree;
    qsort(neighbors, neighbor_count, sizeof(int), compare);

    for (int i = 0; i < neighbor_count; i++) {
        int next = neighbors[i];
        dfs_greedy(g, next, visited, node, tree, tree_index);
    }
}

// Build greedy spanning tree
int build_spanning_tree_greedy(Graph *g, Edge tree[]) {
    int visited[MAX_NODES] = {0};
    int tree_index = 0;

    // Start DFS from the node with the highest degree
    int nodes[MAX_NODES];
    for (int i = 0; i < g->n; i++) nodes[i] = i;
    g_degree = g->degree;
    qsort(nodes, g->n, sizeof(int), compare);

    dfs_greedy(g, nodes[0], visited, -1, tree, &tree_index);

    return tree_index;
}

// Count leaves in the tree
int count_leaves(const Edge tree[], int n, int total_nodes) {
    int degree[MAX_NODES] = {0};
    for (int i = 0; i < n; i++) {
        degree[tree[i].u]++;
        degree[tree[i].v]++;
    }

    int count = 0;
    for (int i = 0; i < total_nodes; i++) {
        if (degree[i] == 1) count++;
    }
    return count;
}

// Print tree edges
void print_tree(const Edge tree[], int n) {
    printf("Greedy Spanning Tree Edges:\n");
    for (int i = 0; i < n; i++) {
        printf("(%d-%d) ", tree[i].u, tree[i].v);
    }
    printf("\n");
}

// Print adjacency matrix
void print_adjacency_matrix(const Edge tree[], int total_nodes) {
    int matrix[MAX_NODES][MAX_NODES] = {0};
    for (int i = 0; i < total_nodes - 1; i++) {
        matrix[tree[i].u][tree[i].v] = 1;
        matrix[tree[i].v][tree[i].u] = 1;
    }

    printf("\nAdjacency Matrix of Greedy Spanning Tree:\n");
    printf("   ");
    for (int i = 0; i < total_nodes; i++) {
        printf("%2d ", i);
    }
    printf("\n");

    for (int i = 0; i < total_nodes; i++) {
        printf("%2d ", i);
        for (int j = 0; j < total_nodes; j++) {
            printf("%2d ", matrix[i][j]);
        }
        printf("\n");
    }
}

int main() {
    // Test Case 1: Complete Graph K4
    // Expected Max Leaves: 3
    // const int n = 4;
    // Graph g;
    // init_graph(&g, n);
    // add_edge(&g, 0, 1); add_edge(&g, 0, 2); add_edge(&g, 0, 3);
    // add_edge(&g, 1, 2); add_edge(&g, 1, 3); add_edge(&g, 2, 3);

    // Test Case 2: Path Graph (0-1-2-3)
    // Expected Max Leaves: 2
    // const int n = 4;
    // Graph g;
    // init_graph(&g, n);
    // add_edge(&g, 0, 1); add_edge(&g, 1, 2); add_edge(&g, 2, 3);

    // Test Case 3: Star Graph (0 connected to 1,2,3)
    // Expected Max Leaves: 3
    // const int n = 4;
    // Graph g;
    // init_graph(&g, n);
    // add_edge(&g, 0, 1); add_edge(&g, 0, 2); add_edge(&g, 0, 3);

    // Test Case 4: Cycle + Chord (0-1-2-3-0 + 0-2)
    // Expected Max Leaves: 3
    // const int n = 4;
    // Graph g;
    // init_graph(&g, n);
    // add_edge(&g, 0, 1); add_edge(&g, 1, 2); add_edge(&g, 2, 3);
    // add_edge(&g, 3, 0); add_edge(&g, 0, 2);

    // Test Case 5: Two triangles (0-1-2, 3-4-5), connected by bridge (2-3), plus extra edges
    // Expected Max Leaves: 4
    const int n = 6;
    Graph g;
    init_graph(&g, n);
    add_edge(&g, 0, 1); add_edge(&g, 1, 2); add_edge(&g, 2, 0);
    add_edge(&g, 3, 4); add_edge(&g, 4, 5); add_edge(&g, 5, 3);
    add_edge(&g, 2, 3); add_edge(&g, 0, 5); add_edge(&g, 1, 4);

    // Test Case 6: 2x2 Grid Graph (Square)
    // Expected Max Leaves: 2
    // const int n = 4;
    // Graph g;
    // init_graph(&g, n);
    // add_edge(&g, 0, 1); add_edge(&g, 0, 2); add_edge(&g, 1, 3); add_edge(&g, 2, 3);

    // Test Case 7: Full Binary Tree (3 levels)
    // Expected Max Leaves: 4
    // const int n = 7;
    // Graph g;
    // init_graph(&g, n);
    // add_edge(&g, 0, 1); add_edge(&g, 0, 2);
    // add_edge(&g, 1, 3); add_edge(&g, 1, 4);
    // add_edge(&g, 2, 5); add_edge(&g, 2, 6);

    // Test Case 8: Wheel Graph (5 nodes)
    // Expected Max Leaves: 4
    // const int n = 5;
    // Graph g;
    // init_graph(&g, n);
    // add_edge(&g, 0, 1); add_edge(&g, 0, 2); add_edge(&g, 0, 3); add_edge(&g, 0, 4);
    // add_edge(&g, 1, 2); add_edge(&g, 2, 3); add_edge(&g, 3, 4); add_edge(&g, 4, 1);

    // Test Case 9: Tree with Articulation Point
    // Expected Max Leaves: 3
    // const int n = 5;
    // Graph g;
    // init_graph(&g, n);
    // add_edge(&g, 0, 1); add_edge(&g, 1, 2); add_edge(&g, 2, 0);
    // add_edge(&g, 2, 3); add_edge(&g, 3, 4); add_edge(&g, 4, 2);

    // Test Case 10: Complete Graph K5
    // Expected Max Leaves: 4
    // const int n = 5;
    // Graph g;
    // init_graph(&g, n);
    // add_edge(&g, 0, 1); add_edge(&g, 0, 2); add_edge(&g, 0, 3); add_edge(&g, 0, 4);
    // add_edge(&g, 1, 2); add_edge(&g, 1, 3); add_edge(&g, 1, 4);
    // add_edge(&g, 2, 3); add_edge(&g, 2, 4);
    // add_edge(&g, 3, 4);

    Edge tree[MAX_NODES]; // Spanning tree will have n - 1 edges
    int tree_size = build_spanning_tree_greedy(&g, tree);

    int leaf_count = count_leaves(tree, tree_size, g.n);

    print_tree(tree, tree_size);
    print_adjacency_matrix(tree, g.n);
    printf("Number of leaves: %d\n", leaf_count);

    return 0;
}