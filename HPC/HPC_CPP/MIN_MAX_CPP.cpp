// Title - Parallel Reduction
// Problem Statement - Implement Min, Max, Sum and Average operations using Parallel Reduction.

#include <iostream>
#include <vector>
#include <omp.h>

using namespace std;

int minSequential(int n, vector<int>& arr){
    int minValue = arr[0];
    for (int i=0; i<n; i++) {
        if (minValue > arr[i]){
            minValue = arr[i];
        }
    }
    return minValue;
}

int minParallel(int n, vector<int>& arr) {
    int minValue = arr[0];

    #pragma omp parallel for default(none) shared(arr, n) reduction(min : minValue) 
    for (int i=0; i<n; i++) {
        if (minValue > arr[i]){
            minValue = arr[i];
        }
    }
    return minValue;
}

int maxSequential(int n, vector<int>& arr) {
    int maxValue = arr[0];
    for (int i=0; i<n; i++) {
        if (maxValue < arr[i]){
            maxValue = arr[i];
        }
    }
    return maxValue;
}

int maxParallel(int n, vector<int>& arr) {
    int maxValue = arr[0];

    #pragma omp parallel for default(none) shared(arr, n) reduction(max : maxValue) 
    for (int i=0; i<n; i++) {
        if (maxValue < arr[i]){
            maxValue = arr[i];
        }
    }
    return maxValue;
}

int sumSequential(int n, vector<int>& arr) {
    int sum = 0;
    for (int i=0; i<n; i++) {
        sum = sum + arr[i];
    }
    return sum;
}

int sumParallel(int n, vector<int>& arr) {
    int sum = 0;

    #pragma omp parallel for default(none) shared(arr, n) reduction(+ : sum) 
    for (int i=0; i<n; i++) {
        sum = sum + arr[i];
    }
    return sum;
}

double averageSequential(int n, vector<int>& arr) {
    double average = (double) sumSequential(n, arr) / n;
    return average;
}

double averageParallel(int n, vector<int>& arr) {
    double average = (double) sumParallel(n, arr) / n;
    return average;
}

int main() {
    int n = 10000000;

    vector<int> arr;
    for(int i=0; i<n; i++){
        arr.push_back(rand() % n);
    }

    auto start = omp_get_wtime();
    cout << "Minimum Value (sequential) : " << minSequential(n, arr) << endl;
    auto stop = omp_get_wtime();
    auto duration = stop - start;
    cout << "Time Taken (sequential) : " << duration << endl;

    start = omp_get_wtime();
    cout << "Minimum Value (parallel) : " << minParallel(n, arr) << endl;
    stop = omp_get_wtime();
    duration = stop - start;
    cout << "Time Taken (parallel) : " << duration << endl;

    start = omp_get_wtime();
    cout << "Maximum Value (sequential) : " << maxSequential(n, arr) << endl;
    stop = omp_get_wtime();
    duration = stop - start;
    cout << "Time Taken (sequential) : " << duration << endl;

    start = omp_get_wtime();
    cout << "Maximum Value (parallel) : " << maxParallel(n, arr) << endl;
    stop = omp_get_wtime();
    duration = stop - start;
    cout << "Time Taken (parallel) : " << duration << endl;

    start = omp_get_wtime();
    cout << "Sum (sequential) : " << sumSequential(n, arr) << endl;
    stop = omp_get_wtime();
    duration = stop - start;
    cout << "Time Taken (sequential) : " << duration << endl;

    start = omp_get_wtime();
    cout << "Sum (parallel) : " << sumParallel(n, arr) << endl;
    stop = omp_get_wtime();
    duration = stop - start;
    cout << "Time Taken (parallel) : " << duration << endl;

    start = omp_get_wtime();
    cout << "Average (sequential) : " << averageSequential(n, arr) << endl;
    stop = omp_get_wtime();
    duration = stop - start;
    cout << "Time Taken (sequential) : " << duration << endl;

    start = omp_get_wtime();
    cout << "Average (parallel) : " << averageParallel(n, arr) << endl;
    stop = omp_get_wtime();
    duration = stop - start;
    cout << "Time Taken (parallel) : " << duration << endl;

    return 0;
}

// Run Program -
sudo apt update
sudo apt install g++
sudo apt install build-essential libomp-dev
g++ -fopenmp Assignment3.cpp -o assignment3
./assignment3