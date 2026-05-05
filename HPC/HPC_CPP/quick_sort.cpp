#include <iostream>
#include <vector>
#include <omp.h>
#include <cstdlib>
using namespace std;

// -------- Partition Function --------
int partition(vector<int>& arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }

    swap(arr[i + 1], arr[high]);
    return i + 1;
}

// -------- Sequential (used internally only) --------
void sequentialQuickSort(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        sequentialQuickSort(arr, low, pi - 1);
        sequentialQuickSort(arr, pi + 1, high);
    }
}

// -------- Parallel QuickSort --------
void parallelQuickSort(vector<int>& arr, int low, int high, int depth = 0) {
    if (low < high) {
        int pi = partition(arr, low, high);

        if (depth < 3) {
#pragma omp parallel sections
            {
#pragma omp section
                parallelQuickSort(arr, low, pi - 1, depth + 1);

#pragma omp section
                parallelQuickSort(arr, pi + 1, high, depth + 1);
            }
        } else {
            sequentialQuickSort(arr, low, pi - 1);
            sequentialQuickSort(arr, pi + 1, high);
        }
    }
}

// -------- MAIN --------
int main() {
    int n = 20;  // small size so you can SEE output
    vector<int> arr(n);

    // Generate random data
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 100;
    }

    cout << "Original Array:\n";
    for (int x : arr) cout << x << " ";
    cout << "\n";

    omp_set_num_threads(4);

    double start = omp_get_wtime();
    parallelQuickSort(arr, 0, n - 1);
    double end = omp_get_wtime();

    cout << "\nSorted Array:\n";
    for (int x : arr) cout << x << " ";
    cout << "\n";

    cout << "\nParallel QuickSort Time: " << end - start << " seconds\n";

    return 0;
}



'''
sudo apt update
sudo apt install g++
sudo apt install libomp-dev
g++ -fopenmp quicksort.cpp -o quicksort
./quicksort

'''