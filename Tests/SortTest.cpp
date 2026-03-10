
#include "SortTest.hpp"
#include "Time.hpp"

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

#define RUNS 10


using namespace std;

void bubbleSort(vector<int>& array) {
    int n =  (int)array.size();

    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (array[j] > array[j + 1]) {
                int temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
    }
}

void quickSort(vector<int>& array, int left, int right) {
    if (left >= right)
        return;

    int pivot = array[right];
    int i = left - 1;

    for (int j = left; j < right; j++) {
        if (array[j] < pivot) {
            i++;
            int temp = array[i];
            array[i] = array[j];
            array[j] = temp;
        }
    }

    int pivotPos = i + 1;
    int temp = array[pivotPos];
    array[pivotPos] = array[right];
    array[right] = temp;

    quickSort(array, left, pivotPos - 1);
    quickSort(array, pivotPos + 1, right);
}

void mergeArrays(vector<int>& array, int left, int mid, int right) {
    vector<int> temp;

    int i = left;
    int j = mid + 1;

    while (i <= mid && j <= right) {
        if (array[i] < array[j]) {
            temp.push_back(array[i++]);
        }
        else {
            temp.push_back(array[j++]);
        }
    }

    while (i <= mid) temp.push_back(array[i++]);
    while (j <= right) temp.push_back(array[j++]);

    for (int k = 0; k < temp.size(); k++) {
        array[left + k] = temp[k];
    }
}

void mergeSort(vector<int>& arr, int left, int right) {
    if (left >= right)
        return;

    int mid = (left + right) / 2;
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);
    mergeArrays(arr, left, mid, right);
}

void runSortTests() {

    cout << "\n=== Sorting Performance Test ===\n";
    cout << "Runs: " << RUNS << "\n";
    const int SIZE = 30000;

    vector<int> startingArray(SIZE);
    srand(0);

    for (int i = 0; i < SIZE; i++) {
        startingArray[i] = rand() % 1000000 + 1;
    }

    double sumBubble = 0.0;
    double sumQuick = 0.0;
    double sumMerge = 0.0;

    for (int r = 0; r < RUNS; r++) {

        vector<int> arrayBubble = startingArray;
        Timing::start();
        bubbleSort(arrayBubble);
        sumBubble += Timing::stop();

        vector<int> arrayQuick = startingArray;
        Timing::start();
        quickSort(arrayQuick, 0, SIZE - 1);
        sumQuick += Timing::stop();

        vector<int> arrayMerge = startingArray;
        Timing::start();
        mergeSort(arrayMerge, 0, SIZE - 1);
        sumMerge += Timing::stop();
    }

    cout << "Buffer size: " << SIZE << "\n\n";
    cout << "Bubble Sort: "<< sumBubble / RUNS << " sec | Time:O(n) to O(n^2) | Space: O(1)\n";
    cout << "Quick Sort:  "<< sumQuick / RUNS << " sec | Time: O(n log n) to O(n^2) | Space: O(log n)\n";
    cout << "Merge Sort:  " << sumMerge / RUNS << " sec | Time: O(n log n) | Space: O(n)\n";
}