#include <iostream>
#include <fstream>
#include <chrono>
#include "gemm_simple.h"
#include "gemm_blocking.h"

using namespace std;

int main() {
    ifstream fin("test/test_4.txt");
    if (!fin) {
        cerr << "Error opening input file." << endl;
        return 1;
    }

    int m, k, n;
    fin >> m >> k >> n;

    double* A = new double[m * k];
    double* B = new double[k * n];
    double* C_simple = new double[m * n];
    double* C_blocking = new double[m * n];

    // Read matrix A
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < k; j++) {
            fin >> A[i * k + j];
        }
    }

    // Read matrix B
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < n; j++) {
            fin >> B[i * n + j];
        }
    }

    fin.close();

    auto start_simple = chrono::high_resolution_clock::now();
    gemm_simple(A, B, C_simple, m, n, k);
    auto end_simple = chrono::high_resolution_clock::now();
    double ms_simple = chrono::duration<double, milli>(end_simple - start_simple).count();

    // Print result matrix C
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            cout << C_simple[i * n + j] << " ";
        }
        cout << "\n";
    }

    cout << "gemm_simple took " << ms_simple << " ms\n";

    auto start_blocking = chrono::high_resolution_clock::now();
    gemm_blocking(A, B, C_blocking, m, n, k, 64);
    auto end_blocking = chrono::high_resolution_clock::now();
    double ms_blocking = chrono::duration<double, milli>(end_blocking - start_blocking).count();

    // Print result matrix C
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            cout << C_blocking[i * n + j] << " ";
        }
        cout << "\n";
    }

    cout << "gemm_blocking took " << ms_blocking << " ms\n";

    delete[] A;
    delete[] B;
    delete[] C_simple;
    delete[] C_blocking;

    return 0;
}