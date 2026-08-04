#include <iostream>
#include "gemm_blocking.h"

void gemm_blocking(const double* A, const double* B, double* C, int m, int n, int k, int block_size) {
    // Perform matrix multiplication C = A * B using blocking
    std::cout << "ALGORITHM: blocked GEMM" << std::endl;
    for (int i = 0; i < m; i += block_size) {
        for (int j = 0; j < n; j += block_size) {
            for (int p = 0; p < k; p += block_size) {
                // Compute the block
                for (int ii = i; ii < std::min(i + block_size, m); ii++) {
                    for (int jj = j; jj < std::min(j + block_size, n); jj++) {
                        double sum = 0.0;
                        for (int pp = p; pp < std::min(p + block_size, k); pp++) {
                            sum += A[ii * k + pp] * B[pp * n + jj];
                        }
                        C[ii * n + jj] += sum;
                    }
                }
            }
        }
    }
}