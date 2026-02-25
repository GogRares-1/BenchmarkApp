
#ifndef SORTTEST_HPP
#define SORTTEST_HPP

#include <vector>

void runSortTests();
void bubbleSort(std::vector<int>& arr);
void quickSort(std::vector<int>& arr, int left, int right);
void mergeSort(std::vector<int>& arr, int left, int right);

#endif