
#include <iostream>
#include <string>
#include "BenchControl.hpp"
#include "SystemInfo.hpp"
#include "Results.hpp"

using namespace std;

void printMenu() {
    cout << "\n=== CPU & System Benchmark ===\n";
    cout << "1. Arithmetic Test\n";
    cout << "2. Sorting Test\n";
    cout << "3. Multithreaded Numerical Test\n";
    cout << "4. Memory Test\n";
    cout << "5. Disk Performance Test\n";
    cout << "6. Floating Point (FLOPS) Test\n";
    cout << "7. Run ALL Tests\n";
    cout << "8. System Information\n";
    cout << "0. Exit\n";
    cout << "Choice: ";
}

int main(int argc, char* argv[])
{
    if (argc > 1) {
        string arg = argv[1];

        if (arg == "--system") {
            printSystemInfo();
            return 0;
        }
        if (arg == "--all") {
            runBenchChoice(7);
            return 0;
        }
        if (arg == "--cpu") {
            runBenchChoice(1);
            return 0;
        }
        if (arg == "--sort") {
            runBenchChoice(2);
            return 0;
        }
        if (arg == "--multithread") {
            runBenchChoice(3);
            return 0;
        }
        if (arg == "--memory") {
            runBenchChoice(4);
            return 0;
        }
        if (arg == "--disk") {
            runBenchChoice(5);
            return 0;
        }
        if (arg == "--flops") {
            runBenchChoice(6);
            return 0;
        }

        if (arg == "--save") {
            if (argc >= 3) {
                string text = argv[2];
                size_t pos = 0;
                while ((pos = text.find("\\n", pos)) != string::npos) {
                    text.replace(pos, 2, "\n");
                    pos++;
                }

                saveResultsToFile(text);
            }
            else {
                cout << "No text provided for saving\n";
            }
            return 0;
        }

        cout << "Unknown argument: " << arg << "\n";
        return 0;
    }

    int choice = -1;
    while (choice != 0) {
        printMenu();
        cin >> choice;

        if (choice == 8)
            printSystemInfo();
        else
            runBenchChoice(choice);
    }

    return 0;
}