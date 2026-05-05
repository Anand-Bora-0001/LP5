#Parallel Linear Regression (basic ML) using threads.
import threading

# Simple dataset (x, y)
x = [1, 2, 3, 4, 5]
y = [2, 4, 6, 8, 10]   # y = 2x

# -------- Parallel Computation --------
sum_x = 0
sum_y = 0
sum_xy = 0
sum_x2 = 0

lock = threading.Lock()

def worker(start, end):
    global sum_x, sum_y, sum_xy, sum_x2

    local_x = local_y = local_xy = local_x2 = 0

    for i in range(start, end):
        local_x += x[i]
        local_y += y[i]
        local_xy += x[i] * y[i]
        local_x2 += x[i] * x[i]

    with lock:
        sum_x += local_x
        sum_y += local_y
        sum_xy += local_xy
        sum_x2 += local_x2


# Split work into 2 threads
n = len(x)
mid = n // 2

t1 = threading.Thread(target=worker, args=(0, mid))
t2 = threading.Thread(target=worker, args=(mid, n))

t1.start()
t2.start()
t1.join()
t2.join()

# -------- Compute Linear Regression --------
# formula for slope (m) and intercept (b)
m = (n*sum_xy - sum_x*sum_y) / (n*sum_x2 - sum_x**2)
b = (sum_y - m*sum_x) / n

print("Slope (m):", m)
print("Intercept (b):", b)