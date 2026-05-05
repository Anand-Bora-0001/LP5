import threading

# Fixed array (easy to explain output)
arr = [10, 20, 30, 40, 50]

# -------- Parallel Reduction --------
def parallel_reduction(arr):
    mid = len(arr) // 2

    part1 = arr[:mid]
    part2 = arr[mid:]

    results = []

    def worker(sub):
        results.append((min(sub), max(sub), sum(sub)))

    t1 = threading.Thread(target=worker, args=(part1,))
    t2 = threading.Thread(target=worker, args=(part2,))

    t1.start()
    t2.start()
    t1.join()
    t2.join()

    # Combine results
    final_min = min(r[0] for r in results)
    final_max = max(r[1] for r in results)
    final_sum = sum(r[2] for r in results)
    avg = final_sum / len(arr)

    print("Min:", final_min)
    print("Max:", final_max)
    print("Sum:", final_sum)
    print("Average:", avg)


# Run
print("Array:", arr)
parallel_reduction(arr)