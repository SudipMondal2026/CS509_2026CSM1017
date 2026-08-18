# CS509 Laboratory Repository

## Repository Overview

- **Course:** CS509 — First-Year M.Tech CSE, 2026
- **Purpose:** This folder holds **Assignment 03 — Individual Task**, submitted in **Single (Individual)** mode. It implements and benchmarks **Kruskal's algorithm** and **Prim's algorithm** for the **Minimum Spanning Tree (MST)** problem on weighted, undirected, connected graphs supplied via a Compressed Sparse Row (CSR) representation, following the CS509 Assignment 3 spec for driver design, input formats, timing, and reporting.
- **Repository naming:** Per the course guidelines this folder should live inside the individual repository named `CS509_<EntryNumber>`, under a path such as `CS509_<EntryNumber>/assignment_03/`.
- **Scope note:** This deliverable covers the **individual task (MST)** only. The buddy tasks (Gradient Descent, Maxflow-Mincut) are separate pair submissions and are not part of this folder.

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
| OS / machine (reference run) | Linux x86_64, container environment, 1 core |
| Timing method | `std::chrono::high_resolution_clock`, wrapped tightly around each algorithm call only |

> The timings in the result table below were produced on the reference machine using the exact commands listed under **Execution**. Absolute numbers will vary on a different machine, but the relative trend (Kruskal's `E log E` sort dominating vs. Prim's lazy-heap `E log V` traversal) should hold.

## Directory Structure

```
Assignment_3/
|-- README.md                  -> this file
|-- Makefile                   -> build rules for the driver and the graph generator
|-- include/
|   |-- mst_io.h                -> MST adjacency-list file reader (new: format has no SOURCE line)
|   `-- mst.h                   -> Kruskal / Prim interfaces
|-- src/
|   |-- main.cpp                -> driver (argv driven: kruskal | prim | both)
|   |-- mst_io.cpp              -> MST-specific file parsing -> AdjList
|   |-- mst_kruskal.cpp         -> Kruskal's algorithm (DSU / Union-Find)
|   `-- mst_prim.cpp            -> Prim's algorithm (binary-heap priority queue)
|-- tools/
|   `-- generate_mst_graph.cpp  -> random connected weighted-graph generator (spanning tree + extra edges)
|-- tests/                      -> one MST test case per file
|   |-- mst_10.txt, mst_100.txt, mst_10000.txt, mst_50000.txt, mst_100000.txt
|-- outputs/                    -> saved full program output for every test case
`-- build/                      -> compiled binaries (assignment3, generate_mst_graph)

Note: this folder deliberately contains NO copy of csr_graph.h / csr_graph.cpp.
Those are compiled directly from ../Assignment_2(single)/ (see "Reuse of
Assignment 2's CSR Conversion" below) - Assignment_3/ must sit as a sibling
of Assignment_2(single)/ for the build to find them.
```

## Reuse of Assignment 2's CSR Conversion

Per the assignment spec ("You are expected to call the CSR conversion function from the previous assignment. DO NOT copy the code into this assignment"), this folder contains **no copy whatsoever** of `csr_graph.h` / `csr_graph.cpp`. The Makefile instead compiles and links `../Assignment_2(single)/src/csr_graph.cpp` directly from its original location, and includes `../Assignment_2(single)/include` on the include path:

```makefile
ASG2_DIR = ../Assignment_2(single)
CXXFLAGS = -O2 -std=c++17 -Iinclude -I"$(ASG2_DIR)/include" -Wall
LOCAL_SRC = src/main.cpp src/mst_io.cpp src/mst_kruskal.cpp src/mst_prim.cpp
ASG2_SRC = "$(ASG2_DIR)/src/csr_graph.cpp"
...
$(CXX) $(CXXFLAGS) $(LOCAL_SRC) $(ASG2_SRC) -o $(BIN)
```

`main.cpp` then simply calls `adjlist_to_csr()` as an ordinary function from that header — the CSR conversion logic is never seen, edited, or duplicated by this assignment. This is why `Assignment_3/` must be extracted as a **sibling** of `Assignment_2(single)/` (both directly under the zip root) for the relative path to resolve; if the folders are moved apart, update `ASG2_DIR` in the Makefile accordingly.

The one addition is `mst_io.cpp`, a small format-specific *file reader* (not a CSR converter). It exists because the Assignment 3 MST input format (Section 5.1 of the spec) has **no trailing `SOURCE` line**, unlike the Assignment 2 single-source formats that `read_weighted_adjlist()` was built for — so reusing that specific reading function would reject every valid MST file. It only builds the `AdjList`; the conversion from `AdjList` to CSR is still done exclusively by Assignment 2's reused `adjlist_to_csr()`.

## Conventions Used

- Every test file contains **exactly one** test case and is named `mst_<V>.txt`, where `<V>` is the vertex count.
- Only the **algorithm's execution time** is timed — file reading, parsing, adjacency-list -> CSR conversion, and output printing are all outside the timed region (Section 8 of the spec). The timer starts immediately before each algorithm call and stops immediately after it returns. Kruskal's edge extraction from CSR and sort, and Prim's heap-based traversal, are both fully inside their respective timed calls.
- Reported unit: **milliseconds (ms)**.
- Both algorithms are run on the **same CSR graph** built once per test file, so the comparison between Kruskal and Prim is apples-to-apples.
- Prim always starts from vertex 0, as recommended by the spec, for reproducibility.

---

## Assignment 03 — Minimum Spanning Tree (Kruskal & Prim)

### Assignment Mode
Single (Individual)

### Objective
Implement and benchmark two classical MST algorithms on the same weighted, undirected, connected graphs of increasing size, using a CSR representation built once per test file:
1. **Kruskal's algorithm** — sort all edges by weight, greedily add an edge if it doesn't create a cycle, using a Disjoint Set Union (DSU) with path compression and union by rank.
2. **Prim's algorithm** — grow a single tree from vertex 0, repeatedly pulling the minimum-weight frontier edge via a binary-heap priority queue (lazy deletion).

### Algorithm / Approach

**Kruskal's algorithm** (`src/mst_kruskal.cpp`): the undirected edge list is extracted from the CSR arrays by keeping only the `(u, v)` copy where `v > u` (each undirected edge appears twice in the CSR, once per endpoint's row). The extracted edges are sorted by weight (`O(E log E)`), then processed in order; a DSU decides in near-`O(1)` (amortized) whether adding an edge would create a cycle. The loop stops as soon as `V-1` edges have been selected. Both the extraction and the sort happen inside the timed call, per the spec's Section 8 timing rule for Kruskal.

**Prim's algorithm** (`src/mst_prim.cpp`): a standard array-of-keys + min-priority-queue implementation. Starting from vertex 0, the algorithm repeatedly pops the cheapest `(weight, vertex)` pair from the heap, skips it if the vertex is already in the tree (lazy deletion instead of a `decrease-key`), and otherwise adds the tree edge and relaxes all CSR-adjacent edges into the heap. Runs in `O(E log V)`.

**CSR conversion** (`include/csr_graph.h`, `src/csr_graph.cpp`, reused verbatim from Assignment 2): the adjacency list built by `read_mst_adjlist()` is converted once into `row_ptr` / `col_idx` / `values` arrays before any timing starts.

### Input / Output Format
Both algorithms consume the same file, matching Section 5.1 of the spec exactly:
```
V E
u0 degree n1 w1 n2 w2 ...
...
u(V-1) degree n1 w1 n2 w2 ...
```
Output matches Section 5.3 exactly (`Algorithm:`, `MST edges:`, `Total MST weight:`, `Execution time:`), for both algorithms.

### Compilation
```bash
# Assignment_3/ must sit as a sibling of Assignment_2(single)/ (both directly
# under the zip root) since the Makefile compiles csr_graph.cpp straight out
# of ../Assignment_2(single)/src/ - no copy of it exists in this folder.
cd Assignment_3
make            # builds build/assignment3
make gen        # builds build/generate_mst_graph (test-graph generator)
```

### Test-Graph Generation
Random connected weighted graphs were generated with `tools/generate_mst_graph.cpp`. It first builds a random spanning tree over a shuffled vertex order (guaranteeing connectivity), then adds extra random edges (avoiding duplicates/self-loops, weights drawn uniformly from `[1, 1000]`) up to the target edge count, keeping large graphs sparse as required (`E ≈ 2V`–`4V` for the two largest sizes).

```bash
./build/generate_mst_graph 10     20      tests/mst_10.txt      1
./build/generate_mst_graph 100    300     tests/mst_100.txt     2
./build/generate_mst_graph 10000  30000   tests/mst_10000.txt   3
./build/generate_mst_graph 50000  150000  tests/mst_50000.txt   4
./build/generate_mst_graph 100000 300000  tests/mst_100000.txt  5
```

### Execution
```bash
./build/assignment3 both tests/mst_10.txt
./build/assignment3 both tests/mst_100.txt
./build/assignment3 both tests/mst_10000.txt
./build/assignment3 both tests/mst_50000.txt
./build/assignment3 both tests/mst_100000.txt
```
(`kruskal` or `prim` alone are also supported as separate modes.) Full output for every run is saved under `outputs/`.

---

## 9.1 MST Results Table

| File | V | E | Kruskal Wt. | Prim Wt. | Kruskal Time | Prim Time | Equal? | Status |
|---|---|---|---|---|---|---|---|---|
| mst_10.txt | 10 | 20 | 1697 | 1697 | 0.0019 ms | 0.0026 ms | Yes | Pass |
| mst_100.txt | 100 | 300 | 22636 | 22636 | 0.0204 ms | 0.0204 ms | Yes | Pass |
| mst_10000.txt | 10,000 | 30,000 | 1,986,988 | 1,986,988 | 7.01 ms | 4.93 ms | Yes | Pass |
| mst_50000.txt | 50,000 | 150,000 | 9,924,604 | 9,924,604 | 12.79 ms | 24.55 ms | Yes | Pass |
| mst_100000.txt | 100,000 | 300,000 | 19,787,613 | 19,787,613 | 25.73 ms | 59.69 ms | Yes | Pass |

All five test cases: Kruskal's and Prim's total MST weight match exactly, confirming correctness. All test graphs were generated as connected (spanning-tree-first construction), so no "not connected" warning was raised on any run. Edge weights are positive integers in `[1, 1000]`; graphs stay sparse (`E ≈ 2V`–`3V`) as required.

### Observations
- Both algorithms agree on the total MST weight on every test, as required by the spec (the specific edge sets may legitimately differ when multiple minimum spanning trees exist, e.g. tied edge weights).
- **Kruskal** is dominated by its `O(E log E)` sort; it scales with the number of edges regardless of graph shape.
- **Prim** is dominated by heap operations proportional to edge relaxations (`O(E log V)`); with lazy deletion its constant factor grows a bit faster than Kruskal's at these sizes on this sparse, uniformly-random graph family, which is visible at V = 50,000 and V = 100,000 where Prim's time exceeds Kruskal's — both remain comfortably sub-100ms even at 100,000 vertices / 300,000 edges.
- No test case failed, crashed, or ran out of memory on the reference machine at any of the required sizes.

## Correctness / Edge-Case Checks
In addition to the five required sizes, the driver was validated against:
- The spec's own worked example (Section 5.2), reproducing the exact expected output (edges `0-1 (2)`, `1-2 (3)`, `1-4 (5)`, `0-3 (6)`, total weight `16`) for both algorithms.
- A missing input file → clean `Error: Could not open input file: ...` message, non-zero exit code.
- An isolated vertex in a V > 1 graph → rejected with a clear error message (invalid per Section 5.1).
- A self-loop edge → rejected with a clear error message.
- An unrecognized mode argument → usage message printed, non-zero exit code.
