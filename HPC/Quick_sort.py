from multiprocessing import Pool
import time

# -------- QuickSort --------
def quicksort(arr):
    if len(arr) <= 1:
        return arr
    pivot = arr[0]
    left = [x for x in arr[1:] if x <= pivot]
    right = [x for x in arr[1:] if x > pivot]
    return quicksort(left) + [pivot] + quicksort(right)

# -------- Parallel QuickSort --------
def parallel_quicksort(arr):
    if len(arr) <= 1:
        return arr

    pivot = arr[0]
    left = [x for x in arr[1:] if x <= pivot]
    right = [x for x in arr[1:] if x > pivot]

    with Pool(2) as p:
        left_sorted, right_sorted = p.map(quicksort, [left, right])

    return left_sorted + [pivot] + right_sorted


# -------- MAIN --------
if __name__ == "__main__":
    arr = [9, 3, 7, 1, 5, 2, 8]

    start = time.perf_counter()
    sorted_arr = parallel_quicksort(arr)
    t2 = time.perf_counter() - start

    print("Original Array :", arr)
    print("ParallelSorted Array   :", sorted_arr)
    print("Parallel Time  :", round(t2, 6))