# Maximum Leaf Spanning Tree (MLST) Algorithms


A collaborative project to implement an exhaustive and non-exhaustive algorithms for finding the spanning tree with the maximum number of leaves. This problem is NP-hard, and solutions have applications in network design and circuit layout optimization.

---

## Table of Contents
- [Problem Definition](#problem-definition)
- [Algorithms](#algorithms)
- [Installation](#installation)
- [Usage](#usage)
- [Repository Structure](#repository-structure)
- [References](#references)

---

## Problem Definition

Given a connected undirected graph \( G = (V, E) \), the **Maximum Leaf Spanning Tree (MLST)** problem aims to find a spanning tree \( T \) of \( G \) such that the number of *leaves* (vertices with degree 1 in \( T \)) is maximized.

### Key Details:
- **NP-Hard**: No known polynomial-time solution for large graphs.
- **Applications**: 
  - Designing efficient communication networks (maximizing coverage nodes).
  - Circuit layouts to minimize redundant connections.
- **Challenges**: Combinatorial explosion for exhaustive methods; trade-offs between optimality and speed.
- **k-approximation algorithm**:
  - Refers to how close the algorithm's output is to the optimal solution.
  - A k-approximation algorithm guarantees that the solution it finds is at most k times worse (for minimization problems) or at least 1/k times as good (for maximization problems), compared to the optimal solution
  - Ex: For the maximization problem (MLSTP), a 2-approximation algorithm guarantees:
        Approximate solution >= 1/2 * Optimal solution
    So, if the optimal spanning tree has 10 leaves, a 2-approximation algorith should give you a spanning tree with at least 5 leaves.

- **Optimal Spanning Tree for an MLST**
  - for the context of this problem, the optimal spanning tree is the spanning tree with the most leaves.
  - However, there are no efficient algorithm (known) to find the optimal solution as the MLST is NP-hard.
---

## Algorithms

### 1. Brute Force (Exhaustive Search)
- **Implementation**: See `gapaz-mapute_project.c`
- **Approach**: Tries all possible combinations of edges that could form a spanning tree. For each valid spanning tree, it counts the number of leaves (nodes with degree 1). The tree with the most leaves is saved and displayed at the end.
- **How it works**:
  1. Generate all combinations of n-1 edges from the input graph.
  2. For each combination, check if it forms a connected tree (using Union-Find/DSU).
  3. Count the number of leaves in the tree.
  4. Track and print the best tree found (with the most leaves).
- **Pros**: Guarantees an optimal solution.
- **Cons**: Only feasible for small graphs due to combinatorial explosion.
- **Complexity**: O(C(m, n-1) × n) (impractical for large graphs).
- **Usage**: Edit the test cases in `gapaz-mapute_project.c` to try different graphs. Run the program to see all valid spanning trees and the one with the most leaves.

### 2. Heuristic/Approximation (Solis-Oba 2-Approximation)
- **Implementation**: See `gapaz-mapute-NE_project.c` for implementation.
- **Approach**:
  - **Greedy Strategy**: Prioritize high-degree vertices as internal nodes.
  - **2-Approximation**: Based on [Solis-Oba (1996)](https://link.springer.com/article/10.1007/s00453-015-0080-0),
      - **Maximally Leafy Forest**: Construct a forest where nodes are constrained to have **degree >= 3** where possible, maximizing internal nodes.
      - **Tiered Edge Connection**: Merge forest components using unused edges in strict priority order:
          1. Internal-internal edges (preserve leaves),
          2. Internal-leaf edges,
          3. Leaf-leaf edges (minimize leaf loss).
- **Pros**: 2-approximation guarantees (50% of optimal leaves).
- **Cons**: More complex than simpler heuristics; not guaranteed to be optimal.
- **Complexity**: O(|E| 𝛼(|V|)) using Union-Find with path compression (near-linear time).
- **Usage**: Edit the test cases in `gapaz-mapute-NE_project.c` to try different graphs. Run the program to see all valid spanning trees and the one with the most leaves.
---

## Installation

1. **Clone the repository:**
  ```bash
  git clone https://github.com/your-username/Algorithms-for-Solving-the-Maximum-Leaf-Spanning-Tree-Problem.git
  cd Algorithms-for-Solving-the-Maximum-Leaf-Spanning-Tree-Problem
  ```

2. **Compile the C programs:**
  ```bash
  gcc gapaz-mapute_project.c -o brute_force
  gcc gapaz-mapute-NE_project.c -o two_approx
  ```

## Usage

- **Run the brute force (exhaustive) algorithm:**
  ```bash
  ./brute_force
  ```

- **Run the heuristic (2-approximation) algorithm:**
  ```bash
  ./two_approx
  ```

- *Note:* You can edit the test cases directly in the respective `.c` files before compiling to try different graphs.

## References
### 1. 2-Approximation Algorithm for Finding a Spanning Tree with Maximum Number of Leaves by Solis-Oba (`References`)


