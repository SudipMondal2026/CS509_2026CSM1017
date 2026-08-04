# CS509 – Assignment 1: GEMM (Single Task)

This repository contains the individual single-task portion of Assignment 1: **General Matrix
Multiplication (GEMM)**, implemented in two versions — a simple nested-loop implementation and a
cache-blocking (tiled) implementation. The buddy tasks (BFS, DFS, SSSP) are submitted separately
as a pair assignment and are not part of this repository.

## 1. Project Structure

```
Assignment_1/
├── driver/
│   └── main.cpp          # Reads input, runs both GEMM versions, prints results and timings
├── src/
│   ├── gemm_simple.h
│   ├── gemm_simple.cpp    # Simple triple-nested-loop GEMM
│   ├── gemm_blocking.h
│   └── gemm_blocking.cpp  # Cache-blocked (tiled) GEMM, block size = 64
├── test/
│   ├── test_1.txt
│   ├── test_2.txt
│   ├── test_3.txt
│   └── test_4.txt
├── build/
│   └── assignment1.exe    # Compiled binary
├── Makefile
└── README.md
```

## 2. Algorithm Overview

GEMM computes `C = A x B`, where `A` is `M x K` and `B` is `K x N`, producing result matrix `C`
of size `M x N`. Each element `C[i][j]` is the dot product of row `i` of `A` and column `j` of `B`.

- **GEMM Simple** (`src/gemm_simple.cpp`): direct triple-nested-loop implementation.
- **GEMM Blocking** (`src/gemm_blocking.cpp`): divides the matrices into `64 x 64` blocks/tiles so
  that data is reused more efficiently in cache before moving to the next block.
- Both implementations are run on the same input matrices in `driver/main.cpp` so that their
  results and execution times can be compared directly, and both produce the same result matrix.

## 3. Input File Format

Each test case is a single `.txt` file with space-separated integer/decimal values:

```
M K N
A row 0 values
A row 1 values
...
A row M-1 values
B row 0 values
B row 1 values
...
B row K-1 values
```

`A` is `M x K`, `B` is `K x N`, and the result `C` is `M x N`.

### Example (`test_1.txt`)

```
2 3 2
1 2 3
4 5 6
7 8
9 10
11 12
```

This represents `A = [[1,2,3],[4,5,6]]` and `B = [[7,8],[9,10],[11,12]]`.

## 4. Output Format

The driver prints the result matrix from each implementation followed by its execution time:

```
Algorithm: GEMM Simple
Result matrix:
58 64
139 154
Execution time: <value> ms

Algorithm: GEMM Blocking
Result matrix:
58 64
139 154
Execution time: <value> ms
```

## 5. Timing Methodology

- The timer is started immediately before the call to `gemm_simple` / `gemm_blocking` and stopped
  immediately after the call returns, using `std::chrono::high_resolution_clock`.
- File reading, input parsing, memory allocation for the matrices, and result printing are **not**
  included in the reported execution time.
- Time is reported in milliseconds (ms).

## 6. Build and Run Instructions

### Build

```bash
make
```

This compiles `driver/main.cpp`, `src/gemm_simple.cpp`, and `src/gemm_blocking.cpp` into
`build/assignment1`.

### Run

```bash
./build/assignment1
```

The current driver reads its input from `test/test_4.txt` (hardcoded path in `main.cpp`). To run a
different test case, update the file path in `driver/main.cpp` and rebuild, or point the input
stream to the desired file in `test/`.

### Clean

```bash
make clean
```

## 7. GEMM Results Table

| Test File | Input Type / Size | Expected Output | Actual Output | Simple Time | Blocking Time | Block Size | Status |
|---|---|---|---|---|---|---|---|
| test_1.txt | M×K and K×N = 2×3 and 3×2 | Result matrix `[[58,64],[139,154]]` | Matches expected | 0.19 ms | 0.03 ms | 64 | Pass |
| test_2.txt | M×K and K×N = 300×300 and 300×300 | Result matrix (300×300) | Matches simple output | 34.24 ms | 23.89 ms | 64 | Pass |
| test_3.txt | M×K and K×N = 3×4 and 4×3 (includes negative values) | Result matrix (3×3) | Matches expected | 0.12 ms | 0.05 ms | 64 | Pass |
| test_4.txt | M×K and K×N = 100×80 and 80×120 (includes negative values) | Result matrix (100×120) | Matches simple output | 1.24 ms | 1.09 ms | 64 | Pass |

*Status = Pass indicates the simple and blocking implementations produced identical result
matrices for that test file. Timings above were measured on the development machine and may vary
slightly by run/hardware; each value shown is from a single run.*

## 8. Notes

- The blocking implementation uses a fixed block size of 64, passed as a parameter to
  `gemm_blocking`.
- `test_2.txt` and `test_4.txt` contain larger matrices (up to 300×300) used to observe the
  performance difference between the simple and blocked implementations; the blocked version is
  consistently faster on these larger inputs due to improved cache reuse.
- `test_3.txt` and `test_4.txt` include negative values to confirm correctness beyond simple
  positive-integer cases.
- Buddy tasks (BFS, DFS, SSSP, and the CSR graph implementation) are not included in this
  repository; they are documented and submitted separately as required by the assignment
  instructions.
