# Maximum Leaf Spanning Tree (MLST) Algorithms

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

A collaborative project to implement an exhaustive and non-exhaustive algorithms for finding the spanning tree with the maximum number of leaves. This problem is NP-hard, and solutions have applications in network design and circuit layout optimization.

---

## Table of Contents
- [Problem Definition](#problem-definition)
- [Algorithms](#algorithms)
- [Installation](#installation)
- [Usage](#usage)
- [Repository Structure](#repository-structure)
- [Contributing](#contributing)
- [License](#license)
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
  - for the context of this proble, the optimal spanning tree is the spanning tree with the most leaves.
  - However, there are no efficient algorithm (known) to find the optimal solution as the MLST is NP-hard.
---

## Algorithms

### 1. Exhaustive Search (`algorithms/exhaustive`)
- **Approach**: Explore all possible spanning trees using Combination and get the spanning tree with the highest number of leaves
- **Pros**: Guarantees an optimal solution.
- **Cons**: Only feasible for small graphs (\(|V| < 20\)).
- **Complexity**: \( O(|V|^{|V|}) \) (impractical for large graphs).

### 2. Heuristic/Approximation (`algorithms/heuristic`)
- **Approach**: 
  - **Greedy Strategy**: Prioritize high-degree vertices as internal nodes.
  - **3-Approximation**: Based on [Lu & Ravi (1996)](https://doi.org/10.1006/jagm.1998.0944), iteratively prune "non-leafable" nodes.
- **Pros**: Runs in \( O(|E| \log |V|) \), suitable for large graphs.
- **Cons**: May not find the optimal solution.
- **Implementation**: Uses NetworkX for graph operations.

---

## Installation

1. **Clone the repository**:
   ```bash
   git clone https://github.com/your-username/maximum-leaf-spanning-tree.git
   cd maximum-leaf-spanning-tree