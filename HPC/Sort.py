import threading
import time
import random

# Generate random array
arr = [random.randint(1, 100) for _ in range(10)]

# ---------------- SEQUENTIAL BUBBLE SORT ----------------
def bubble_sort(arr):
    a = arr.copy()
    n = len(a)
    for i in range(n):
        for j in range(0, n-i-1):
            if a[j] > a[j+1]:
                a[j], a[j+1] = a[j+1], a[j]
    return a


# ---------------- PARALLEL BUBBLE SORT (FIXED) ----------------
def parallel_bubble_sort(arr):
    a = arr.copy()
    n = len(a)

    def compare_swap(i):
        if a[i] > a[i+1]:
            a[i], a[i+1] = a[i+1], a[i]

    for phase in range(n):
        threads = []
        start = phase % 2          # 0 = even phase, 1 = odd phase
        for i in range(start, n-1, 2):
            t = threading.Thread(target=compare_swap, args=(i,))
            threads.append(t)
            t.start()
        for t in threads:
            t.join()
    return a


# ---------------- SEQUENTIAL MERGE SORT ----------------
def merge_sort(arr):
    if len(arr) <= 1:
        return arr

    mid = len(arr) // 2
    left = merge_sort(arr[:mid])
    right = merge_sort(arr[mid:])

    result = []
    i = j = 0

    while i < len(left) and j < len(right):
        if left[i] < right[j]:
            result.append(left[i]); i += 1
        else:
            result.append(right[j]); j += 1

    result.extend(left[i:])
    result.extend(right[j:])
    return result


# ---------------- PARALLEL MERGE SORT ----------------
def parallel_merge_sort(arr):
    if len(arr) <= 1:
        return arr

    mid = len(arr) // 2
    left = []
    right = []

    def sort_left():
        nonlocal left
        left = parallel_merge_sort(arr[:mid])

    def sort_right():
        nonlocal right
        right = parallel_merge_sort(arr[mid:])

    t1 = threading.Thread(target=sort_left)
    t2 = threading.Thread(target=sort_right)

    t1.start()
    t2.start()
    t1.join()
    t2.join()

    # merge
    result = []
    i = j = 0
    while i < len(left) and j < len(right):
        if left[i] < right[j]:
            result.append(left[i]); i += 1
        else:
            result.append(right[j]); j += 1

    result.extend(left[i:])
    result.extend(right[j:])
    return result


# ---------------- PERFORMANCE ----------------
print("Original Array:", arr)

# Bubble Sort
start = time.time()
print("\Parallel Bubble:", bubble_sort(arr))
print("Time:", time.time() - start)

start = time.time()
print("\nSequential Bubble:", parallel_bubble_sort(arr))
print("Time:", time.time() - start)

# Merge Sort
start = time.time()
print("\nParallel Merge:", merge_sort(arr))
print("Time:", time.time() - start)

start = time.time()
print("\nSequential Merge:", parallel_merge_sort(arr))
print("Time:", time.time() - start)