
#include "MultithreadTest.hpp"
#include "Time.hpp"

#include <iostream>
#include <vector>
#include <thread>
#include <cmath>

#define RUNS 10

using namespace std;

vector<int> smallSieve(int limit) {
    vector<char> prime(limit + 1, 1);
    prime[0] = 0;
    prime[1] = 0;

    for (int p = 2; p * p <= limit; p++) {
        if (prime[p] == 1) {
            for (int i = p * p; i <= limit; i += p) {
                prime[i] = 0;
            }
        }
    }

    vector<int> primes;
    for (int i = 2; i <= limit; i++) {
        if (prime[i] == 1) {
            primes.push_back(i);
        }
    }

    return primes;
}

void sieveBlock(long long low, long long high,
    const vector<int>& basePrimes,
    long long& localCount) {

    long long blockSize = high - low + 1;
    vector<char> block(blockSize, 1);

    for (int p : basePrimes) {
        long long start = max(1LL * p * p,
            ((low + p - 1) / p) * 1LL * p);

        for (long long i = start; i <= high; i += p) {
            block[i - low] = 0;
        }
    }

    localCount = 0;
    for (long long i = 0; i < blockSize; i++) {
        if (block[i] == 1 && (low + i) >= 2) {
            localCount++;
        }
    }
}

void segmentedSieve(long long N, int threadCount) {
    long long limit = sqrt(N);
    vector<int> basePrimes = smallSieve(limit);

    long long blockSize = 1'000'000;
    long long numBlocks = (N + blockSize) / blockSize;

    vector<long long> threadResults(threadCount, 0);
    vector<thread> threads;

    for (int t = 0; t < threadCount; t++) {
        threads.push_back(thread([&, t]() {
            for (long long block = t; block < numBlocks; block += threadCount) {

                long long low = block * blockSize;
                long long high = low + blockSize - 1;

                if (high > N) high = N;
                if (low < 2)  low = 2;

                long long localCount = 0;
                sieveBlock(low, high, basePrimes, localCount);
                threadResults[t] += localCount;
            }}));
    }

    for (auto& th : threads) {
        th.join();
    }
}

void runMultithreadTests() {
    cout << "\n=== Multithreaded Numerical Computation Test ===\n";
    cout << "Runs: " << RUNS << "\n";

    long long N = 500'000'000;
    int threadCounts[] = { 1, 2, 4, 8, 16, 32, 64 };

    for (int t : threadCounts) {
        double sumTime = 0.0;

        for (int r = 0; r < RUNS; r++) {
            Timing::start();
            segmentedSieve(N, t);
            double elapsed = Timing::stop();
            sumTime += elapsed;
        }

        double avgTime = sumTime / RUNS;

        cout << "\nRunning test with " << t << " threads\n";
        cout << "Average time: " << avgTime << " sec\n";
        cout << "GRAPH_MT threads=" << t << " time=" << avgTime << "\n";
    }
}