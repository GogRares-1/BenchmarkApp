
#include <iostream>
#include <iomanip>
#include "ArithmeticTest.hpp"
#include "Time.hpp"

#define RUNS 10

using namespace std;

void runBasicArithemticTest() {

    const long long iterations = 1000000000; // 1 miliard
    const int adds = 5;

    double sumAddsPerSecond = 0.0;

    for (int r = 0; r < RUNS; r++) {
        volatile int a = 1;
        volatile int b = 1;

        Timing::start();
        for (long long i = 0; i < iterations; i++) {
            a += b;
            b += a;
            a += b;
            b += a;
            a += b;
        }
        double elapsed = Timing::stop();

        long long totalAdds = iterations * adds;
        double addsPerSecond = (totalAdds / elapsed) / 1000000000; // 10^9 GB

        sumAddsPerSecond += addsPerSecond;
    }

    double avgAddsPerSecond = sumAddsPerSecond / RUNS;

    cout << "\n=== Basic Arithmetic Performance Test ===\n";
    cout << "Runs: " << RUNS << "\n";
    cout << "Average Result: " << fixed << setprecision(2)
        << avgAddsPerSecond << " GADDs/sec\n";
}

void runCALOTest() {
    const long long iterations = 1000000000; // 1 miliard
    const int ops = 9;

    double sumOpsPerSecond = 0.0;

    for (int r = 0; r < RUNS; r++) {
        volatile int a = 1;
        volatile int b = 2;
        volatile int c = 3;
        volatile int d = 4;

        Timing::start();
        for (long long i = 0; i < iterations; i++) {
            a = (a + b) ^ c;
            b = b * (a & 3);
            c = (c | b) - d;
            d = (a ^ c) + (b & c);
        }
        double elapsed = Timing::stop();

        long long totalOps = iterations * ops;
        double opsPerSecond = (totalOps / elapsed) / 1000000000; // 10^9 GB

        sumOpsPerSecond += opsPerSecond;
    }

    double avgOpsPerSecond = sumOpsPerSecond / RUNS;

    cout << "\n=== Combined Arithmetic and Logic Operations Test ===\n";
    cout << "Runs: " << RUNS << "\n";
    cout << "Average Result: " << fixed << setprecision(2)
        << avgOpsPerSecond << " GOPs/sec\n";
}

void runArithmeticTests() {
    runBasicArithemticTest();
    runCALOTest();
}