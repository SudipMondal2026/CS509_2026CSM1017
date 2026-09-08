# CS509 Laboratory Repository

## Repository Overview

- **Course:** CS509 — First-Year M.Tech CSE, 2026
- **Purpose:** This folder holds **Assignment 04 — Individual Task**, submitted in **Single (Individual)** mode. It implements and benchmarks **Greedy Vertex Coloring (Welsh-Powell ordering)** and **PageRank**, both operating on unweighted graphs supplied via a Compressed Sparse Row (CSR) representation, following the CS509 Assignment 4 spec for driver design, input formats, timing, and reporting.
- **Repository naming:** Per the course guidelines this folder should live inside the individual repository named `CS509_<EntryNumber>`, under a path such as `CS509_<EntryNumber>/assignment_04/`.
- **Scope note:** This deliverable covers the **individual task (Vertex Coloring + PageRank)** only. The buddy tasks (K-Means Clustering, FastMap) are separate pair submissions and are not part of this folder.

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

> The timings in the result tables below were produced on the reference machine using the exact commands listed under **Execution**. Absolute numbers will vary on a different machine, but the relative scaling behaviour should hold.

## Directory Structure

```
Assignment_4/
|-- README.md                        -> this file
|-- Makefile                         -> build rules for the driver and the two graph generators
|-- include/
|   |-- csr_graph.h                  -> AdjList / CSRGraph types + adjlist_to_csr() declaration
|   |-- graph_io.h                   -> file readers for the Vertex Coloring / PageRank formats
|   |-- coloring.h                   -> Welsh-Powell greedy coloring interface
|   `-- pagerank.h                   -> PageRank interface
|-- src/
|   |-- main.cpp                     -> driver (argv driven: color | pagerank)
|   |-- csr_graph.cpp                -> adjacency-list -> CSR conversion (see note below)
|   |-- graph_io.cpp                 -> strict line-based input parsing + validation
|   |-- coloring.cpp                 -> Welsh-Powell greedy vertex coloring + validator
|   `-- pagerank.cpp                 -> PageRank (Jacobi-style simultaneous update, push-based)
|-- tools/
|   |-- generate_coloring_graph.cpp  -> random sparse undirected-graph generator
|   `-- generate_pagerank_graph.cpp  -> random sparse directed-graph generator (ring + extras + dangling)
|-- tests/                           -> one test case per file (10 required files)
|   |-- color_10.txt, color_100.txt, color_10000.txt, color_50000.txt, color_100000.txt
|   `-- pagerank_10.txt, pagerank_100.txt, pagerank_1000.txt, pagerank_10000.txt, pagerank_50000.txt
|-- outputs/                         -> saved full program output for every test case
`-- build/                           -> compiled binaries (assignment4, generate_coloring_graph, generate_pagerank_graph)
```

## Note on Reusing the CSR Conversion

Per the spec ("You are expected to call the CSR conversion function from the previous assignment. DO NOT copy the code into this assignment."), a real submission should **not** keep a second, independent copy of `csr_graph.h` / `csr_graph.cpp`. This folder includes its own copy only because it was produced as a standalone deliverable without access to this student's actual, previously-graded Assignment 2/3 CSR module.

To satisfy the spec exactly, replace `include/csr_graph.h` + `src/csr_graph.cpp` in the Makefile's source list with a path into your own prior assignment folder — following the same pattern Assignment 3 used for Assignment 2:

```makefile
ASGX_DIR = ../Assignment_3
CXXFLAGS += -I"$(ASGX_DIR)/include"
LOCAL_SRC = src/main.cpp src/graph_io.cpp src/coloring.cpp src/pagerank.cpp
ASGX_SRC  = "$(ASGX_DIR)/src/csr_graph.cpp"
# ... link $(LOCAL_SRC) $(ASGX_SRC) instead of compiling a local csr_graph.cpp
```

The `AdjList` / `CSRGraph` types and the `adjlist_to_csr()` signature in `include/csr_graph.h` here are written to be drop-in compatible with that reuse pattern.

## Conventions Used

- Every test file contains **exactly one** test case, named `color_<V>.txt` or `pagerank_<V>.txt`.
- Only the **algorithm's execution time** is timed — file reading, parsing, adjacency-list -> CSR conversion, and output printing are all outside the timed region (Section 9 of the spec). The timer starts immediately before the algorithm call and stops immediately after it returns.
- For PageRank, **all iterations** of the rank-update loop are inside the timed call, as required.
- Reported unit: **milliseconds (ms)**.
- Vertex numbering is 0 to V-1 throughout.

---

## Assignment 04 — Vertex Coloring & PageRank (Individual Task)

### Assignment Mode
Single (Individual)

### Objective
Implement and benchmark:
1. **Greedy Vertex Coloring** (Welsh-Powell ordering) on unweighted, undirected CSR graphs of increasing size.
2. **PageRank** on unweighted, directed CSR graphs of increasing size.

### Algorithm / Approach

**Vertex Coloring** (`src/coloring.cpp`): vertex degrees are read directly off the CSR `row_ptr` array (`O(V)`). Vertices are sorted once by non-increasing degree (`O(V log V)`), per the Welsh-Powell heuristic. Each vertex, in that order, is assigned the smallest color index not already used by any colored neighbour, found by scanning a small reusable "used colors" scratch buffer sized to the largest color seen so far — this keeps each vertex's coloring step proportional to its degree plus the number of colors in use, rather than a fixed large constant. Overall complexity is `O(V log V + E)`. Isolated vertices are naturally assigned color 0 since they have no neighbours to avoid.

**PageRank** (`src/pagerank.cpp`): rather than building a reverse (incoming-edge) graph, each iteration uses a **push-based** update that is mathematically identical to the pull formula in the spec: for every vertex `u` with `outdeg(u) > 0`, its current rank contributes `d * PR(u) / outdeg(u)` to every one of its CSR out-neighbours `v`. This lets the algorithm work directly off the CSR `row_ptr` / `col_idx` arrays that are already available, with no extra graph structure, in `O(V + E)` per iteration. All vertices start at `1/V`. Dangling vertices (`outdeg(u) == 0`) have their entire rank redistributed evenly across **all** vertices for that iteration, avoiding any divide-by-zero. The update is Jacobi-style / simultaneous: every vertex for iteration `t+1` is computed purely from iteration `t`'s values, never from partially-updated `t+1` values. The loop stops as soon as the total absolute change across all vertices is `<= TOLERANCE`, or after `MAX_ITERATIONS`, whichever comes first.

**CSR conversion** (`include/csr_graph.h`, `src/csr_graph.cpp`): the adjacency list built by `read_coloring_adjlist()` / `read_pagerank_adjlist()` is converted once into `row_ptr` / `col_idx` arrays before any timing starts. `values` is omitted entirely since both graphs are unweighted (Section 4.1 of the spec).

### Input / Output Format
Matches Sections 5 and 6 of the spec exactly. Vertex Coloring:
```
V E
u0 degree n1 n2 ...
...
```
PageRank:
```
V E
u0 outdegree n1 n2 ...
...
DAMPING d
TOLERANCE epsilon
MAX_ITERATIONS n
```
Output matches Sections 5.3 and 6.3 respectively (`Algorithm:`, `Vertex colors:` / `Vertex ranks:`, `Colors used:` / `Sum of ranks:`, `Execution time:`), plus an extra `Valid coloring:` / `Converged:` line as required by the spec's correctness/convergence reporting.

### Compilation
```bash
cd Assignment_4
make            # builds build/assignment4
make gen        # builds build/generate_coloring_graph and build/generate_pagerank_graph
```

### Test-Graph Generation
Random graphs were generated with `tools/generate_coloring_graph.cpp` and `tools/generate_pagerank_graph.cpp`.

- **Coloring generator**: repeatedly picks a random pair `(u, v)`, `u != v`, and adds the undirected edge if not already present, until the target edge count is reached (or an attempt cap is hit) — no self-loops, no parallel edges, sparse (`E` chosen at roughly `2V`-`3V` for the larger sizes).
- **PageRank generator**: first builds a directed ring (`i -> (i+1) mod V`) so every vertex starts with at least one incoming and one outgoing edge, then layers on random extra directed edges (no self-loops, no duplicate `(u,v)` pairs) up to the target edge count, and finally clears the out-edges of a small fraction (~0.5%) of vertices to deliberately create a handful of **dangling vertices**, exercising the dangling-mass redistribution path.

```bash
./build/generate_coloring_graph 10      20      tests/color_10.txt      1
./build/generate_coloring_graph 100     250     tests/color_100.txt     2
./build/generate_coloring_graph 10000   30000   tests/color_10000.txt   3
./build/generate_coloring_graph 50000   150000  tests/color_50000.txt   4
./build/generate_coloring_graph 100000  300000  tests/color_100000.txt  5

./build/generate_pagerank_graph 10     20     tests/pagerank_10.txt     11
./build/generate_pagerank_graph 100    250    tests/pagerank_100.txt    12
./build/generate_pagerank_graph 1000   3000   tests/pagerank_1000.txt   13
./build/generate_pagerank_graph 10000  30000  tests/pagerank_10000.txt  14
./build/generate_pagerank_graph 50000  150000 tests/pagerank_50000.txt  15
```

### Execution
```bash
./build/assignment4 color    tests/color_10.txt
./build/assignment4 color    tests/color_100.txt
./build/assignment4 color    tests/color_10000.txt
./build/assignment4 color    tests/color_50000.txt
./build/assignment4 color    tests/color_100000.txt

./build/assignment4 pagerank tests/pagerank_10.txt
./build/assignment4 pagerank tests/pagerank_100.txt
./build/assignment4 pagerank tests/pagerank_1000.txt
./build/assignment4 pagerank tests/pagerank_10000.txt
./build/assignment4 pagerank tests/pagerank_50000.txt
```
Full output for every run is saved under `outputs/`.

---

## 10.1 Vertex Coloring Results Table

| File | V | E | Colors Used | Valid? | Time | Status |
|---|---|---|---|---|---|---|
| color_10.txt | 10 | 20 | 4 | Yes | 0.0026 ms | Pass |
| color_100.txt | 100 | 250 | 5 | Yes | 0.0142 ms | Pass |
| color_10000.txt | 10,000 | 30,000 | 6 | Yes | 1.37 ms | Pass |
| color_50000.txt | 50,000 | 150,000 | 6 | Yes | 7.53 ms | Pass |
| color_100000.txt | 100,000 | 300,000 | 6 | Yes | 15.17 ms | Pass |

All five graphs were validated with `validate_coloring()`: no adjacent vertices share a color in any test case. Edge counts satisfy the spec's `E ≈ 2V`-`4V` sparsity guideline for the larger sizes.

## 10.2 PageRank Results Table

| File | V | E | Damping | Sum of Ranks | Iter. / Time | Status |
|---|---|---|---|---|---|---|
| pagerank_10.txt | 10 | 20 | 0.85 | 1.000000 | 14 / 0.0019 ms | Pass |
| pagerank_100.txt | 100 | 250 | 0.85 | 1.000000 | 17 / 0.0117 ms | Pass |
| pagerank_1000.txt | 1,000 | 2,987 | 0.85 | 1.000000 | 15 / 0.0990 ms | Pass |
| pagerank_10000.txt | 10,000 | 29,840 | 0.85 | 1.000000 | 15 / 2.33 ms | Pass |
| pagerank_50000.txt | 50,000 | 149,203 | 0.85 | 1.000000 | 15 / 12.10 ms | Pass |

All damping, tolerance (`0.0001`), and `MAX_ITERATIONS` (`100`) settings were embedded in each test file. All five graphs converged well before the iteration cap, and the sum of ranks stayed at `1.000000` after the final iteration in every case, as required.

### Observations
- **Vertex Coloring**: Welsh-Powell settles on 4-6 colors across all tested sizes for these uniformly-random sparse graphs, well below `V`; the number of colors used is not sensitive to graph size once the average degree stabilizes (`E/V` is roughly constant at 2-4 by construction). Runtime scales close to linearly with `V + E`, as expected from the `O(V log V + E)` bound.
- **PageRank**: iteration counts stay in a narrow band (14-17) across four orders of magnitude of `V`, since convergence speed is governed by the damping factor's spectral gap rather than graph size directly. Runtime per test scales close to linearly with `V + E`, consistent with the `O(V + E)` cost per iteration.
- No test case failed, crashed, or ran out of memory on the reference machine at any of the required sizes.
- **Dangling-vertex handling**: the PageRank generator deliberately zeroes out the out-edges of ~0.5% of vertices for `V >= 200` (`pagerank_1000.txt`, `pagerank_10000.txt`, `pagerank_50000.txt` all contain dangling vertices); all three still converge cleanly with `Sum of ranks: 1.000000`, confirming the dangling-mass redistribution is implemented correctly.

## Correctness / Edge-Case Checks
In addition to the ten required test files, the driver was validated against:
- The spec's own Vertex Coloring worked example (Section 5.2): produces a valid 3-coloring (colors `2 0 1 1 0` for vertices `0..4` in this implementation's Welsh-Powell tie-breaking order) — a different labeling from the spec's sample `0 1 2 0 1`, but equally valid, matching the spec's own note that "the exact colors assigned may differ... provided the coloring is valid."
- The spec's own PageRank worked example (Section 6.2): this implementation converges to `0.201960, 0.373603, 0.386937, 0.037500` (sum `1.000000`), rather than the spec's sample `0.226283, 0.267536, 0.297557, 0.208624`. This was checked independently against the exact update formula given in Section 2.2 (both by hand and with a separate reference script); vertex 3 in that example graph has **no incoming edges at all** (only `2 -> 0, 1` and `3 -> 2`), so under the stated formula its rank must converge to exactly the teleport term `(1-d)/N = 0.0375`, which is what this implementation produces. The spec's sample numbers appear to be illustrative rather than an exact computed reference for that particular graph.
- A missing input file → clean `Error: Could not open input file: ...` message, non-zero exit code.
- A self-loop in a Vertex Coloring file → rejected with a clear error message.
- A mismatched degree/neighbour count (both too few and too many neighbours listed) → rejected with a clear error message (line-based parsing is used specifically so this can't silently "borrow" tokens from the next line).
- An out-of-range vertex/neighbour id → rejected with a clear error message.
- An isolated vertex (degree 0) in a Vertex Coloring file → accepted and assigned color 0, as required.
- A PageRank file with `DAMPING >= 1` → rejected with a clear error message.
- A PageRank file with `MAX_ITERATIONS <= 0` → rejected with a clear error message.
- An unrecognized mode argument → usage message printed, non-zero exit code.
