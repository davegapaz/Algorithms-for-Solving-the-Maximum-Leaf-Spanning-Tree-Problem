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

### 1. Exhaustive Search  
- **Approach**: Explore all possible spanning trees using Combination and get the spanning tree with the highest number of leaves
- **Pros**: Guarantees an optimal solution.
- **Cons**: Only feasible for small graphs (\(|V| < 20\)).
- **Complexity**: \( O(|V|^{|V|}) \) (impractical for large graphs).

### 2. Heuristic/Approximation (`algorithms/heuristic`)
- **Approach**: 
  - **Greedy Strategy**: Prioritize high-degree vertices as internal nodes.
  - **2-Approximation**: Based on [Solis-Oba (1996)](https://link.springer.com/article/10.1007/s00453-015-0080-0),
      - **Maximally Leafy Forest**: Construct a forest where nodes are constrained to have **degree >= 3** where possible, maximizing internal nodes.
      - **Tiered Edge Connection**: Merge foreces components using unused edges in strict priority order:
          1. Internal-internal edges (preserve leaves),
          2. Internal-leaf edges,
          3. Leaf-leaf edges (minimize leaf loss).
- **Pros**: 2-approximation guarantees (50% of optimal leaves).
- **Cons**: More complex than simpler heuristics; not guaranteed to be optimal
- **Complexity**: O(|E| 𝛼(|𝑉|)) using Union-Find with path compression (near-linear time).

---

## Installation

## References
### 1. 2-Approximation Algorithm for Finding a Spanning Tree with Maximum Number of Leaves by Solis-Oba (`References`)


