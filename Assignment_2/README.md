# CS509 Laboratory Repository

## Repository Overview

- **Course:** CS509 — First-Year M.Tech CSE, 2026
- **Purpose:** This repository (folder) holds **Assignment 02**, submitted in **Single (Individual)** mode. It implements and benchmarks two classical shortest-path algorithms — **Bellman-Ford** (single-source shortest paths with negative edge weights) and **Floyd-Warshall** (all-pairs shortest paths) — following the CS509 Lab Work Guidelines for driver design, testing, timing, and documentation.
- **Repository naming:** Per Section 3.1 of the guidelines this folder should live inside the individual repository named `CS509_<EntryNumber>`, under a path such as `CS509_<EntryNumber>/assignment_02/`.

## Student Details

| Field | Value |
|---|---|
| Name | `<Your Name>` |
| Entry Number | `<Your Entry Number>` |
| Mode | Single / Individual |

## Language and Environment

| Item | Value |
|---|---|
| Language | C++ (C++17) |
| Compiler | `g++` (GCC) 13.3.0 |
| Build tool | GNU `make` |
| Compilation flags | `-O2 -std=c++17 -Iinclude -Wall` |
| OS / machine (reference run) | Linux x86_64, Intel Xeon @ 2.80 GHz, 1 core, 4 GB RAM |
| Timing method | `std::chrono::high_resolution_clock`, wrapped tightly around the algorithm call only |

> The timings recorded in the result tables below were produced on the reference machine described above using the exact commands listed under **Compilation** and **Execution**. Absolute numbers will vary slightly on a different machine but the relative growth trend (near-linear for Bellman-Ford, cubic for Floyd-Warshall) should hold.

## Directory Structure

```
Assignment_2(single)/
|-- README.md                 -> this file
|-- Makefile                  -> build rules for the assignment binary
|-- include/                  -> header files (interfaces)
|   |-- csr_graph.h
|   |-- bellman_ford.h
|   `-- floyd_warshall.h
|-- src/                      -> implementation + driver
|   |-- main.cpp              -> dedicated driver (menu-less, argv driven)
|   |-- csr_graph.cpp         -> input parsing + adjacency-list -> CSR conversion
|   |-- bellman_ford.cpp      -> Bellman-Ford algorithm (CSR based)
|   `-- floyd_warshall.cpp    -> Floyd-Warshall algorithm (dense matrix based)
|-- tests/                    -> one test case per file
|   |-- bf_10.txt, bf_100.txt, bf_10000.txt, bf_50000.txt, bf_100000.txt   (Bellman-Ford)
|   `-- fw_10.txt, fw_100.txt, fw_500.txt, fw_1000.txt, fw_2000.txt        (Floyd-Warshall)
|-- outputs/                  -> saved program output for every test case (optional, used here for large cases)
|-- scripts/
|   `-- generate_graphs.py    -> generates random connected test graphs at fixed V sizes
`-- build/
    `-- assignment2(.exe)     -> compiled binary
```

## Common Wrapper: Build and Usage

Section 7 of the guidelines requires a **single, repository-level** common wrapper that lets a user browse and run *all* submitted assignments from one menu. That wrapper lives at the root of the individual repository (`CS509_<EntryNumber>/common_wrapper/`), one level above this folder, and is documented in the top-level repository README. It calls into this assignment's dedicated driver (`src/main.cpp`) rather than duplicating any algorithm logic.

Within this assignment folder specifically, the dedicated driver can also be built and run standalone as described below.

## Conventions Used

- Every test file contains **exactly one** test case and is named `test-family_size.txt` (e.g. `bf_10000.txt`, `fw_500.txt`), where the family (`bf` / `fw`) indicates the target algorithm and the number indicates the vertex count `V`.
- Only the **algorithm's execution time** is timed — input reading, parsing, adjacency-list -> CSR conversion, and output printing are all outside the timed region (Section 5). The timer starts immediately before the algorithm call and stops immediately after it returns.
- Reported unit: **milliseconds (ms)**.
- For Bellman-Ford, the CSR structure is built *before* the timer starts, so only the algorithm itself is measured.
- Full program output for the larger test cases (thousands of vertices) is saved under `outputs/` rather than pasted in full into this README; the result tables below give the exact command, the measured time, and a compact summary of the output (min/max distance, negative-cycle status) with a pointer to the saved file.

---

## Assignment 02 — Shortest Path Algorithms (Bellman-Ford & Floyd-Warshall)

### Assignment Mode
Single (Individual)

### Objective
Implement and benchmark two shortest-path algorithms on graphs of increasing size:
1. **Bellman-Ford** — single-source shortest paths on a sparse, weighted, directed graph that may contain negative edge weights (with negative-cycle detection), using a CSR (Compressed Sparse Row) representation for cache-efficient edge traversal.
2. **Floyd-Warshall** — all-pairs shortest paths on a dense weighted graph given directly as an adjacency matrix, with negative-cycle detection via the diagonal of the final distance matrix.

### Algorithm / Approach

**Bellman-Ford** (`src/bellman_ford.cpp`): standard relax-all-edges loop repeated up to `V - 1` times, with an early exit if a full pass makes no change. Each vertex's outgoing edges are traversed via the CSR `row_ptr` / `col_idx` / `values` arrays instead of a vector-of-vectors adjacency list, so the inner loop touches only contiguous memory. A final `V`-th pass checks whether any edge can still be relaxed; if so, a negative-weight cycle reachable from the source is reported.

**Floyd-Warshall** (`src/floyd_warshall.cpp`): classic triple-nested dynamic-programming loop — for every intermediate vertex `k`, `dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j])`. After the DP completes, a negative diagonal entry `dist[i][i] < 0` indicates a negative-weight cycle, in which case the distance matrix is discarded and `negative_cycle` is reported instead.

### Input Format

**Bellman-Ford** (weighted adjacency list, converted to CSR before timing):
```
V E
u_0 degree_0  nbr_0 w_0  nbr_1 w_1 ...
u_1 degree_1  ...
...
SOURCE s
```
- `V`, `E`: vertex and edge counts.
- One line per vertex: its id, out-degree, then `(neighbor, weight)` pairs. Weights may be negative.
- Final line names the source vertex for the SSSP computation.

**Floyd-Warshall** (dense adjacency matrix, read directly — CSR conversion is not applicable):
```
V
row_0 (V space-separated entries: integer or the literal INF)
row_1
...
row_(V-1)
```
- `matrix[i][j]` is the direct edge weight from `i` to `j`, or `INF` if no direct edge.
- Diagonal entries `matrix[i][i]` must be `0`.

### Helper Functions / CSR Conversion
- `read_weighted_adjlist()` / `read_adjmatrix()` (`csr_graph.cpp`, `floyd_warshall.cpp`) — parse the two input formats above, with validation (positive `V`, in-range vertex ids, well-formed rows, zero diagonal for the matrix format).
- `adjlist_to_csr()` (`csr_graph.cpp`) — converts the parsed adjacency list into `row_ptr` / `col_idx` / `values` CSR arrays. This conversion happens **before** the timer starts, per Section 5 of the guidelines.
- `print_csr()` — debug helper to dump a CSR graph.

### File Structure
| File | Role |
|---|---|
| `include/csr_graph.h`, `src/csr_graph.cpp` | Adjacency-list parsing + CSR conversion (shared) |
| `include/bellman_ford.h`, `src/bellman_ford.cpp` | Bellman-Ford algorithm |
| `include/floyd_warshall.h`, `src/floyd_warshall.cpp` | Dense-matrix parsing + Floyd-Warshall algorithm |
| `src/main.cpp` | Dedicated driver — dispatches to either algorithm based on `argv[1]`, times the call, prints results |
| `tests/bf_*.txt` | Bellman-Ford test cases (5 sizes) |
| `tests/fw_*.txt` | Floyd-Warshall test cases (5 sizes) |
| `scripts/generate_graphs.py` | Generates random connected graphs (spanning tree + extra random edges) at the required sizes |

### Compilation
From the `Assignment_2(single)/` directory:
```bash
make
```
This runs:
```bash
g++ -O2 -std=c++17 -Iinclude -Wall src/main.cpp src/csr_graph.cpp src/bellman_ford.cpp src/floyd_warshall.cpp -o build/assignment2
```
Clean rebuild:
```bash
make clean && make
```

### Execution
The driver supports running **one selected test file** per invocation:
```bash
./build/assignment2 bellmanford   tests/bf_10.txt
./build/assignment2 floydwarshall tests/fw_10.txt
```
To run **all test files** for this assignment (as required by the common wrapper):
```bash
for f in tests/bf_*.txt;  do ./build/assignment2 bellmanford   "$f"; done
for f in tests/fw_*.txt;  do ./build/assignment2 floydwarshall "$f"; done
```
Output for every run can be redirected to `outputs/` for record-keeping, e.g.:
```bash
./build/assignment2 bellmanford tests/bf_10000.txt > outputs/bf_10000_bellmanford.txt
```

### Test Cases and Result Table — Bellman-Ford

| Mode | Test File | Input Type | Input Size | Expected Output | Actual Output | Algorithm Time |
|---|---|---|---|---|---|---|
| Single | `bf_10.txt` | Weighted adjacency list -> CSR | V = 10, E = 20 | Finite SSSP distances from source 0 for all 10 vertices (graph is connected, no negative cycle) | Distances range **-3 .. 14**; all 10 vertices reachable (0 marked `INF`); full listing in `outputs/bf_10_bellmanford.txt` | 0.000961 ms |
| Single | `bf_100.txt` | Weighted adjacency list -> CSR | V = 100, E = 200 | Finite SSSP distances for all 100 vertices | Distances range **-3 .. 30**; 0 vertices `INF`; full listing in `outputs/bf_100_bellmanford.txt` | 0.009904 ms |
| Single | `bf_10000.txt` | Weighted adjacency list -> CSR | V = 10000, E = 15000 | Finite SSSP distances for all 10000 vertices | Distances range **-4 .. 99**; 0 vertices `INF`; full listing in `outputs/bf_10000_bellmanford.txt` | 1.41505 ms |
| Single | `bf_50000.txt` | Weighted adjacency list -> CSR | V = 50000, E = 75000 | Finite SSSP distances for all 50000 vertices | Distances range **0 .. 132**; 0 vertices `INF`; full listing in `outputs/bf_50000_bellmanford.txt` | 7.11204 ms |
| Single | `bf_100000.txt` | Weighted adjacency list -> CSR | V = 100000, E = 150000 | Finite SSSP distances for all 100000 vertices | Distances range **-1 .. 146**; 0 vertices `INF`; full listing in `outputs/bf_100000_bellmanford.txt` | 16.9784 ms |

### Test Cases and Result Table — Floyd-Warshall

| Mode | Test File | Input Type | Input Size | Expected Output | Actual Output | Algorithm Time |
|---|---|---|---|---|---|---|
| Single | `fw_10.txt` | Dense adjacency matrix (direct, no CSR) | V = 10 | Finite 10x10 all-pairs distance matrix, no negative cycle | Matrix entries range **-4 .. 17**; `Negative cycle: none`; full matrix in `outputs/fw_10_floydwarshall.txt` | 0.003854 ms |
| Single | `fw_100.txt` | Dense adjacency matrix (direct, no CSR) | V = 100 | Finite 100x100 all-pairs distance matrix, no negative cycle | Matrix entries range **-43 .. 21**; `Negative cycle: none`; full matrix in `outputs/fw_100_floydwarshall.txt` | 9.77896 ms |
| Single | `fw_500.txt` | Dense adjacency matrix (direct, no CSR) | V = 500 | Finite 500x500 all-pairs distance matrix, no negative cycle | Matrix entries range **-203 .. 27**; `Negative cycle: none`; full matrix in `outputs/fw_500_floydwarshall.txt` | 189.764 ms |
| Single | `fw_1000.txt` | Dense adjacency matrix (direct, no CSR) | V = 1000 | Finite 1000x1000 all-pairs distance matrix, no negative cycle | Matrix entries range **-415 .. 31**; `Negative cycle: none`; full matrix in `outputs/fw_1000_floydwarshall.txt` | 1596.09 ms |
| Single | `fw_2000.txt` | Dense adjacency matrix (direct, no CSR) | V = 2000 | Finite 2000x2000 all-pairs distance matrix, no negative cycle | Matrix entries range **-769 .. 27**; `Negative cycle: none`; full matrix in `outputs/fw_2000_floydwarshall.txt` | 14091.5 ms |

### Complexity

| Algorithm | Time Complexity | Space Complexity |
|---|---|---|
| Bellman-Ford (CSR) | O(V x E) | O(V + E) for the CSR graph, O(V) for the distance/predecessor arrays |
| Floyd-Warshall | O(V^3) | O(V^2) for the distance matrix |

The measured timings match these bounds qualitatively: Bellman-Ford scales roughly linearly with `V` here because `E` grows linearly with `V` in the generated test graphs (average degree held constant), while Floyd-Warshall's time grows roughly as the cube of `V` (e.g. doubling `V` from 1000 to 2000 multiplies the time by close to 8x).

### References
- T. H. Cormen, C. E. Leiserson, R. L. Rivest, C. Stein, *Introduction to Algorithms* (CLRS) — chapters on Bellman-Ford and Floyd-Warshall / all-pairs shortest paths.
- Compressed Sparse Row (CSR) format for sparse graph/matrix storage.

---

## Notes on Reproducing These Results
1. Run `python3 scripts/generate_graphs.py` (from the assignment root) to regenerate random test graphs if needed; note it targets filenames `bfs_*` / `dfs_*` / `sssp_*` for a related BFS/DFS/SSSP assignment and would need its size list / filename prefixes adjusted to reproduce the `bf_*` / `fw_*` files used here.
2. All timings above were captured with `make clean && make` followed by the exact `./build/assignment2 <mode> <test file>` command shown under **Execution**, on the reference machine described in **Language and Environment**. Re-run on your own machine and update the table if you need timings specific to your hardware.
