#include <iostream>
#include "gemm_simple.h"

void gemm_simple(const double* A, const double* B, double* C, int m, int n, int k) {
    // Perform matrix multiplication C = A * B
    std::cout << "ALGORITHM: simple GEMM" << std::endl;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            C[i * n + j] = 0.0;
            for (int p = 0; p < k; p++) {
                C[i * n + j] += A[i * k + p] * B[p * n + j];
            }
        }
    }
}