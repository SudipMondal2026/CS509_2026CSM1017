#ifndef GEMM_BLOCKING_H
#define GEMM_BLOCKING_H

void gemm_blocking(const double* A, const double* B, double* C, int m, int n, int k, int block_size);

#endif