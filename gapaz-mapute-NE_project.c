/*
This is a non-exhaustive algorithm that finds a spanning tree with the maximum number of leaves
in a given graph. It uses the 2-approximation algorithm by Solis-Oba and a greedy approach to find the maximum leaf spanning tree.

Key Functions:
- createGraph: Initializes a graph with a given number of vertices.
- addEdge: Adds an undirected edge between two vertices.
- DFS: Performs a depth-first search to create an initial spanning tree.
- computeDegrees: Computes the degree of each vertex in the graph.
- countLeaves: Counts the number of leaf nodes in the graph.
- dsu_init: Initializes the disjoint set union data structure.
- dsu_find: Finds the root of a node in the disjoint set union.
- dsu_union: Merges two nodes in the disjoint set union.
- applyExpansion: Applies the 4 expansion rules to find the maximum leaf spanning tree.

Algorithm:
- The program starts by creating a graph and adding edges to it.
- It then performs a depth-first search to create an initial spanning tree.
- The degrees of each vertex are computed.
- The disjoint set union is initialized to keep track of connected components.
- The program applies the expansion rules to find the maximum leaf spanning tree.

Usage:
- Edit the test cases in the main function to try different graphs.
- Run the program to see all valid spanning trees and the one with the most leaves.

Note:
    A 2-approximation algorithm guarantees that the solution will be at most half of the optimal solution.
    Example:
        - a graph which has the maximized spanning tree with 4 leaves may have a spanning tree with 2 leaves given this algorithm.

Pros:
    - Fast and efficient for large graphs.
    - Provides a good approximation for the maximum leaf spanning tree problem (better than 3 or 5 approximation).
Cons:
    - Not guaranteed to find the optimal solution.
    - May not work well for graphs with specific structures (e.g., dense graphs).
    - The algorithm is not exhaustive, so it may miss some valid spanning trees.
*/


/*
2-approximation algorithm for the maximum leaf spanning tree problem (Solis-Oba)
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h> 

#define MAX_NODES 100

typedef struct Node {
    int vertex;
    struct Node* next;
} Node;

typedef struct {
    Node* head;
} AdjList;

typedef struct {
    int V;
    AdjList* array;
} Graph;

typedef struct {
    int u, v;
} Edge;

bool visited[MAX_NODES];
int parent[MAX_NODES];
int degree[MAX_NODES];
int dsu_parent[MAX_NODES];
Graph* dfsTree;

//creates a graph with V vertices and initializes the adjacency list with NULL values
//Time: O(V)
Graph* createGraph(int V) {
    Graph* graph = malloc(sizeof(Graph));
    graph->V = V;
    graph->array = malloc(V * sizeof(AdjList));
    for (int i = 0; i < V; i++)
        graph->array[i].head = NULL;
    return graph;
}

//adds an undirected edge between vertices u and v
//Time: O(1)
void addEdge(Graph* graph, int u, int v) {
    Node* node = malloc(sizeof(Node));
    node->vertex = v;
    node->next = graph->array[u].head;
    graph->array[u].head = node;

    node = malloc(sizeof(Node));
    node->vertex = u;
    node->next = graph->array[v].head;
    graph->array[v].head = node;
}


// perform DFS for creating the initial spanning tree
//Time: O(V + E)
void DFS(Graph* graph, int u) {
    visited[u] = true;
    Node* temp = graph->array[u].head;
    while (temp != NULL) {
        int v = temp->vertex;
        if (!visited[v]) {
            parent[v] = u;
            addEdge(dfsTree, u, v);
            DFS(graph, v);
        }
        temp = temp->next;
    }
}


//compute the degree of each vertex in the graph
// O(V + E)
void computeDegrees(Graph* g, int V) {
    for (int i = 0; i < V; i++) {
        degree[i] = 0;
        Node* cur = g->array[i].head;
        while (cur) {
            degree[i]++;
            cur = cur->next;
        }
    }
}

//count the number of leaf nodes (degree 1) in the graph
// O(V)
int countLeaves(Graph* g, int V) {
    int count = 0;
    for (int i = 0; i < V; i++)
        if (degree[i] == 1)
            count++;
    return count;
}

//initializes an instance of the disjoint set union data struc
void dsu_init(int V) {
    for (int i = 0; i < V; i++)
        dsu_parent[i] = i;
}

//find the a direct connection from a root to a leaf
int dsu_find(int u) {
    if (dsu_parent[u] != u)
        dsu_parent[u] = dsu_find(dsu_parent[u]);
    return dsu_parent[u];
}

//merges a root to leaf edge while maintaining connectivity
void dsu_union(int u, int v) {
    int ru = dsu_find(u);
    int rv = dsu_find(v);
    if (ru != rv)
        dsu_parent[rv] = ru;
}

//application of the 4 expansion rules
//O(V + E)
void applyExpansion(Graph* original, Graph* tree, int V) {
    for (int u = 0; u < V; u++) {
        if (degree[u] >= 3) { //priority is degree >= 3 nodes
            Node* temp = original->array[u].head;
            while (temp) {
                int v = temp->vertex;
                if (dsu_find(u) != dsu_find(v)) { //makes sure that adding u and v do not form a cycle
                    addEdge(tree, u, v); //makes sure that u and v do not form a cycle
                    dsu_union(u, v);  //connects u and v
                }
                temp = temp->next;
            }
        }
    }
}

//O(V^2) - this was disregarded in the analysis of the time complexity since these are just additional functions for the presentation 
void printAdjMatrix(Graph* g, int V, const char* label) {
    int mat[MAX_NODES][MAX_NODES] = {0};
    for (int i = 0; i < V; i++) {
        Node* temp = g->array[i].head;
        while (temp) {
            mat[i][temp->vertex] = 1;
            temp = temp->next;
        }
    }

    printf("\n%s\n", label);
    printf("   ");
    for (int i = 0; i < V; i++) printf("%d ", i);
    printf("\n");
    for (int i = 0; i < V; i++) {
        printf("%d: ", i);
        for (int j = 0; j < V; j++)
            printf("%d ", mat[i][j]);
        printf("\n");
    }
}

int main() {
    clock_t start, end;
    double cpu_time_used;

    start = clock();  // Start timing


    /* Test Cases*/

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
    // Edge edges[] = {
    //     {0, 1}, {0, 2}, {0, 3}, {0, 4},
    //     {1, 2}, {2, 3}, {3, 4}, {4, 1}
    // };
    // int m = 8;

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

    // // Test Case 11: Large Tree with Extra Connections
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

    Graph* graph = createGraph(N); 
    dfsTree = createGraph(N); //create an empty spanning tree

    //build the original graph    
    for (int i = 0; i < m; i++)
        addEdge(graph, edges[i].u, edges[i].v);

    //create the initial spanning tree using DFS
    DFS(graph, 0);
    computeDegrees(dfsTree, N); //get the degree of each vertex in the DFS
    dsu_init(N);//initialize the disjoint set union data struc

    //combine all the connected components (if there is direct path between root and leaf)
    for (int i = 0; i < N; i++) {
        Node* temp = dfsTree->array[i].head;
        while (temp) {
            if (i < temp->vertex)
                dsu_union(i, temp->vertex);
            temp = temp->next;
        }
    }

    applyExpansion(graph, dfsTree, N);
    computeDegrees(dfsTree, N);
    int leaves = countLeaves(dfsTree, N);

    printAdjMatrix(graph, N, "Original Graph (K5):");
    printAdjMatrix(dfsTree, N, "Approximate Spanning Tree:");
    printf("\nNumber of Leaves: %d\n", leaves);

    end = clock();  // End timing
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time taken: %f seconds\n", cpu_time_used);
    return 0;
}
