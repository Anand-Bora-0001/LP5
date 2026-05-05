#include <iostream>
#include <vector>
#include <omp.h>
#include <chrono>
#include <random>

using namespace std;

// -------- Sequential Bubble Sort --------
void sequentialBubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i-1; j++) {
            if (arr[j] > arr[j+1]) {
                swap(arr[j], arr[j+1]);
            }
        }
    }
}

// -------- Parallel Bubble Sort (FIXED) --------
void parallelBubbleSort(vector<int>& arr) {
    int n = arr.size();
    bool swapped = true;

    while (swapped) {
        swapped = false;

        // Even phase
#pragma omp parallel for reduction(||:swapped)
        for (int i = 0; i < n-1; i += 2) {
            if (arr[i] > arr[i+1]) {
                swap(arr[i], arr[i+1]);
                swapped = true;
            }
        }

        // Odd phase
#pragma omp parallel for reduction(||:swapped)
        for (int i = 1; i < n-1; i += 2) {
            if (arr[i] > arr[i+1]) {
                swap(arr[i], arr[i+1]);
                swapped = true;
            }
        }
    }
}

// -------- Merge Function --------
void merge(vector<int>& arr, int left, int mid, int right) {
    vector<int> temp(right - left + 1);
    int i = left, j = mid + 1, k = 0;

    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j]) temp[k++] = arr[i++];
        else temp[k++] = arr[j++];
    }

    while (i <= mid) temp[k++] = arr[i++];
    while (j <= right) temp[k++] = arr[j++];

    for (i = 0; i < k; i++) {
        arr[left + i] = temp[i];
    }
}

// -------- Sequential Merge Sort --------
void sequentialMergeSort(vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        sequentialMergeSort(arr, left, mid);
        sequentialMergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

// -------- Parallel Merge Sort --------
void parallelMergeSort(vector<int>& arr, int left, int right, int depth = 0) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        if (depth < 3) {
#pragma omp parallel sections
            {
#pragma omp section
                parallelMergeSort(arr, left, mid, depth + 1);

#pragma omp section
                parallelMergeSort(arr, mid + 1, right, depth + 1);
            }
        } else {
            sequentialMergeSort(arr, left, mid);
            sequentialMergeSort(arr, mid + 1, right);
        }

        merge(arr, left, mid, right);
    }
}

// -------- MAIN --------
int main() {
    const int SIZE = 50000;
    vector<int> arr(SIZE);

    // Random data
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, 10000);

    for (int i = 0; i < SIZE; i++) {
        arr[i] = dis(gen);
    }

    vector<int> arr_copy = arr;

    // Threads
    omp_set_num_threads(4);

    // -------- Bubble Sort --------
    auto start = chrono::high_resolution_clock::now();
    sequentialBubbleSort(arr);
    auto end = chrono::high_resolution_clock::now();
    cout << "Sequential Bubble Sort Time: "
         << chrono::duration_cast<chrono::milliseconds>(end - start).count()
         << " ms\n";

    arr = arr_copy;

    start = chrono::high_resolution_clock::now();
    parallelBubbleSort(arr);
    end = chrono::high_resolution_clock::now();
    cout << "Parallel Bubble Sort Time: "
         << chrono::duration_cast<chrono::milliseconds>(end - start).count()
         << " ms\n";

    // -------- Merge Sort --------
    arr = arr_copy;

    start = chrono::high_resolution_clock::now();
    sequentialMergeSort(arr, 0, arr.size() - 1);
    end = chrono::high_resolution_clock::now();
    cout << "Sequential Merge Sort Time: "
         << chrono::duration_cast<chrono::milliseconds>(end - start).count()
         << " ms\n";

    arr = arr_copy;

    start = chrono::high_resolution_clock::now();
    parallelMergeSort(arr, 0, arr.size() - 1);
    end = chrono::high_resolution_clock::now();
    cout << "Parallel Merge Sort Time: "
         << chrono::duration_cast<chrono::milliseconds>(end - start).count()
         << " ms\n";

    return 0;
}



'''
sudo apt update
sudo apt install g++
sudo apt install build-essential libomp-dev
g++ -fopenmp sort.cpp -o sort
./sort