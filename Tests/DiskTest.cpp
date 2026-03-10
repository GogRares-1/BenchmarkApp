
#define _CRT_SECURE_NO_WARNINGS

#include "DiskTest.hpp"
#include "Time.hpp"

#include <iostream>
#include <fstream>
#include <cstdio>
#include <vector>

#define RUNS 10

using namespace std;

void runWriteReadTest() {
    cout << "\n=== WRITE/READ Test ===\n";

    const long long dataSize = 200LL * 1024 * 1024; // 200 MB
    vector<char> buffer(1024 * 1024, 'A');          // 1 MB block

    double sumWriteMBps = 0.0;
    double sumReadMBps = 0.0;

    for (int r = 0; r < RUNS; r++) {
        long long written = 0;

        // WRITE
        Timing::start();
        ofstream out("disk_test.bin");
        while (written < dataSize) {
            out.write(buffer.data(), buffer.size());
            written += buffer.size();
        }
        out.close();

        double writeTime = Timing::stop();
        double writeMBps = (double)dataSize / (1024 * 1024) / writeTime;
        sumWriteMBps += writeMBps;

        // READ
        Timing::start();
        ifstream in("disk_test.bin");
        long long readBytes = 0;

        while (in.read(buffer.data(), buffer.size())) {
            readBytes += in.gcount();
        }
        in.close();

        double readTime = Timing::stop();
        double readMBps = (double)readBytes / (1024 * 1024) / readTime;
        sumReadMBps += readMBps;

        remove("disk_test.bin");
    }

    cout << "Average write speed: " << sumWriteMBps / RUNS << " MB/s\n";
    cout << "Average read speed:  " << sumReadMBps / RUNS << " MB/s\n";
}

void runFwriteFreadTest() {
    cout << "\n=== FREAD/FWRITE Test ===\n";

    const long long dataSize = 200LL * 1024 * 1024; // 200 MB
    vector<char> buffer(1024 * 1024, 'B');          // 1 MB block

    double sumWriteMBps = 0.0;
    double sumReadMBps = 0.0;

    for (int r = 0; r < RUNS; r++) {
        long long written = 0;

        // FWRITE
        Timing::start();
        FILE* fout = fopen("disk_test2.bin", "wb");
        while (written < dataSize) {
            fwrite(buffer.data(), 1, buffer.size(), fout);
            written += buffer.size();
        }
        fclose(fout);

        double writeTime = Timing::stop();
        double writeMBps = (double)dataSize / (1024 * 1024) / writeTime;
        sumWriteMBps += writeMBps;

        // FREAD
        Timing::start();
        FILE* fin = fopen("disk_test2.bin", "rb");
        long long readBytes = 0;

        while (true) {
            size_t rbytes = fread(buffer.data(), 1, buffer.size(), fin);
            if (rbytes == 0) break;
            readBytes += rbytes;
        }
        fclose(fin);

        double readTime = Timing::stop();
        double readMBps = (double)readBytes / (1024 * 1024) / readTime;
        sumReadMBps += readMBps;

        remove("disk_test2.bin");
    }

    cout << "Average write speed: " << sumWriteMBps / RUNS << " MB/s\n";
    cout << "Average read speed:  " << sumReadMBps / RUNS << " MB/s\n";
}

void runDiskTest() {
    cout << "\n=== Disk Performance Test ===\n";
    cout << "Runs: " << RUNS << "\n";
    runWriteReadTest();
    runFwriteFreadTest();
}