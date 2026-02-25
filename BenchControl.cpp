
#include "BenchControl.hpp"

#include "ArithmeticTest.hpp"
#include "SortTest.hpp"
#include "MultithreadTest.hpp"
#include "MemoryTest.hpp"
#include "DiskTest.hpp"
#include "FloatTest.hpp"

#include <iostream>
using namespace std;

void runBenchChoice(int choice) {

    switch (choice) {

    case 1:
        runArithmeticTests();
        break;

    case 2:
        runSortTests();
        break;

    case 3:
        runMultithreadTests();
        break;

    case 4:
        cout << "\n=== MEMORY TESTS ===\n";
        runStreamTest();
        runMemcpyCacheTest();
        break;

    case 5:
        runDiskTest();
        break;

    case 6:
        runFLOPSTest();
        break;

    case 7:
        cout << "\n=== RUNNING ALL TESTS ===\n";
        runArithmeticTests();
        runSortTests();
        runMultithreadTests();
        runStreamTest();
        runMemcpyCacheTest();
        runDiskTest();
        runFLOPSTest();
        break;

    case 0:
        cout << "Closed App\n";
        break;

    default:
        cout << "Invalid option\n";
        break;
    }
}