import threading

# Undirected Graph
graph = {
    0: [1, 2],
    1: [0, 3, 4],
    2: [0, 5],
    3: [1],
    4: [1, 6],
    5: [2],
    6: [4]
}

# ── Parallel BFS ──
def parallel_bfs(start):
    visited = set([start])
    queue = [start]
    result = []
    lock = threading.Lock()

    while queue:
        result.extend(queue)
        next_level = []

        def visit(node):
            for neighbor in graph[node]:
                with lock:
                    if neighbor not in visited:
                        visited.add(neighbor)
                        next_level.append(neighbor)

        threads = [threading.Thread(target=visit, args=(n,)) for n in queue]
        for t in threads: t.start()
        for t in threads: t.join()
        queue = next_level

    return result

# ── Parallel DFS (fixed) ──
def parallel_dfs(start):
    visited = set()
    result = []
    lock = threading.Lock()

    def dfs(node):
        with lock:
            if node in visited:
                return
            visited.add(node)
            result.append(node)

        # Visit each unvisited neighbor sequentially (depth-first order preserved)
        for neighbor in graph[node]:
            with lock:
                already = neighbor in visited
            if not already:
                t = threading.Thread(target=dfs, args=(neighbor,))
                t.start()
                t.join()  # join immediately = depth first, not breadth first

    dfs(start)
    return result

# ── Run ──
print("Graph:", graph)
print("\nParallel BFS:", parallel_bfs(0))
print("Parallel DFS:", parallel_dfs(0))