
#include "Results.hpp"
#include <fstream>
#include <filesystem>
#include <iostream>

using namespace std;
namespace fs = std::filesystem;

void saveResultsToFile(const string& text)
{
    int index = 1;
    string filename;

    while (true) {
        filename = "Test" + to_string(index) + ".txt";
        if (!fs::exists(filename))
            break;
        index++;
    }

    ofstream out(filename);
    if (!out) {
        cerr << "Error: Could not write file\n";
        return;
    }

    out << text;
    out.close();

    cout << "Saved results to: " << filename << "\n";
}