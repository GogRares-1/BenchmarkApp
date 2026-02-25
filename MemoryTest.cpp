
#include "MemoryTest.hpp"
#include "Time.hpp"

#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstring>

#define RUNS 10

using namespace std;

void runStreamTest() {
    cout << "\n=== Memory STREAM Test ===\n";

    const int N = 50'000'000;
    const double scalar = 3.0;

    vector<double> a(N, 1.0);
    vector<double> b(N, 2.0);
    vector<double> c(N, 0.0);

    double copySum = 0.0;
    double scaleSum = 0.0;
    double addSum = 0.0;
    double triadSum = 0.0;

    for (int r = 0; r < RUNS; r++) {
        long long bytes;
        double elapsed, bandwidth;

        // COPY
        Timing::start();
        for (int i = 0; i < N; i++) {
            c[i] = a[i];
        }
        elapsed = Timing::stop();
        bytes = sizeof(double) * N * 2;
        bandwidth = bytes / elapsed / 1000000000;
        copySum += bandwidth;

        // SCALE
        Timing::start();
        for (int i = 0; i < N; i++) {
            b[i] = scalar * c[i];
        }
        elapsed = Timing::stop();
        bytes = sizeof(double) * N * 2;
        bandwidth = bytes / elapsed / 1000000000;
        scaleSum += bandwidth;

        // ADD
        Timing::start();
        for (int i = 0; i < N; i++) {
            c[i] = a[i] + b[i];
        }
        elapsed = Timing::stop();
        bytes = sizeof(double) * N * 3;
        bandwidth = bytes / elapsed / 1000000000;
        addSum += bandwidth;

        // TRIAD
        Timing::start();
        for (int i = 0; i < N; i++) {
            a[i] = b[i] + scalar * c[i];
        }
        elapsed = Timing::stop();
        bytes = sizeof(double) * N * 3;
        bandwidth = bytes / elapsed / 1000000000;
        triadSum += bandwidth;
    }

    cout << "Runs: " << RUNS << "\n";
    cout << "Copy: " << copySum / RUNS << " GB/sec\n";
    cout << "Scale:" << scaleSum / RUNS << " GB/sec\n";
    cout << "Add:  " << addSum / RUNS << " GB/sec\n";
    cout << "Triad:" << triadSum / RUNS << " GB/sec\n";
}

void runMemcpyCacheTest() {
    cout << "\n=== memcpy() Cache-Level Memory Test ===\n";

    struct TestRegion {
        string name;
        size_t size;
    };

    vector<TestRegion> tests = {
        {"L1 Cache",   16 * 1024},         // 16 KB
        {"L2 Cache",   256 * 1024},        // 256 KB
        {"L3 Cache",   4 * 1024 * 1024},   // 4 MB
        {"RAM",        64 * 1024 * 1024}   // 64 MB
    };

    for (auto& t : tests) {
        double sumGBps = 0.0;

        for (int r = 0; r < RUNS; r++) {
            size_t sz = t.size;
            vector<char> src(sz, 1);
            vector<char> dst(sz, 2);

            long long reps = max(1LL, (long long)(500000000 / sz));

            Timing::start();
            for (long long i = 0; i < reps; i++) {
                memcpy(dst.data(), src.data(), sz);
            }
            double elapsed = Timing::stop();

            double totalBytes = (double)sz * reps;
            double gbps = totalBytes / elapsed / 1000000000; //GB
            sumGBps += gbps;
        }

        double avgGBps = sumGBps / RUNS;

        if (t.size < 1024 * 1024) // printed based on sizes in KB or MB
        {
            cout << t.name<< ": " << t.size / 1024 << " KB -> " << avgGBps << " GB/s\n";
        }
        else 
        {
            cout << t.name<< ": " << t.size / (1024 * 1024) << " MB -> " << avgGBps << " GB/s\n";
        }

        cout << "GRAPH_MEMCPY block=" << t.size << " speed=" << avgGBps << "\n\n";
    }
}