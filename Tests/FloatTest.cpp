
#include "FloatTest.hpp"
#include "Time.hpp"

#include <iostream>
#include <vector>
#include <cstdlib>

#define RUNS 10

using namespace std;

void runFLOPSTest() {
    cout << "\n=== Floating Point Performance Test ===\n";

    const int N = 300;
    const int FLOPS_PER_MULTIPLY = 2;

    long long totalOps = (long long)FLOPS_PER_MULTIPLY * N * N * N;
    double sumGFLOPS = 0.0;
    srand(0);

    for (int r = 0; r < RUNS; r++) {

        vector<vector<double>> A(N, vector<double>(N));
        vector<vector<double>> B(N, vector<double>(N));
        vector<vector<double>> C(N, vector<double>(N, 0.0));

        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                A[i][j] = (double)(rand() % 1000) / 100.0;
                B[i][j] = (double)(rand() % 1000) / 100.0;
            }
        }

        Timing::start();
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                double sum = 0.0;
                for (int k = 0; k < N; k++) {
                    sum += A[i][k] * B[k][j];
                }
                C[i][j] = sum;
            }
        }
        double elapsed = Timing::stop();

        double gflops = (double)totalOps / elapsed / 1000000000; //GB
        sumGFLOPS += gflops;
    }

    double avgGFLOPS = sumGFLOPS / RUNS;

    cout << "Runs: " << RUNS << "\n";
    cout << "Matrix size: " << N << " x " << N << "\n";
    cout << "Average performance: " << avgGFLOPS << " GFlops/sec\n";
}