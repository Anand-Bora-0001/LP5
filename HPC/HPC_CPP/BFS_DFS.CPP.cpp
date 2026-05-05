#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>

using namespace std;

class Graph {
    int v;
    vector<vector<int>> adj;

public:
    Graph(int v) {
        this->v = v;
        adj.resize(v);
    }

    void addEdge(int v1, int v2) {
        adj[v1].push_back(v2);
        adj[v2].push_back(v1);
    }

    void printGraph() {
        for (int i = 0; i < v; i++) {
            cout << i << " -> [ ";
            for (int j = 0; j < adj[i].size(); j++) {
                cout << adj[i][j] << " ";
            }
            cout << "]" << endl;
        }
    }

    // -------- Sequential BFS --------
    void sequentialBFS(int start) {
        vector<bool> visited(v, false);
        queue<int> q;

        visited[start] = true;
        q.push(start);

        while (!q.empty()) {
            int vertex = q.front();
            q.pop();

            for (int neighbour : adj[vertex]) {
                if (!visited[neighbour]) {
                    visited[neighbour] = true;
                    q.push(neighbour);
                }
            }
        }
    }

    // -------- Parallel BFS (SAFE VERSION) --------
    void parallelBFS(int start) {
        vector<bool> visited(v, false);
        queue<int> q;

        visited[start] = true;
        q.push(start);

        while (!q.empty()) {
            int size = q.size();

#pragma omp parallel for
            for (int i = 0; i < size; i++) {
                int vertex;

#pragma omp critical
                {
                    if (!q.empty()) {
                        vertex = q.front();
                        q.pop();
                    } else {
                        continue;
                    }
                }

                for (int neighbour : adj[vertex]) {
#pragma omp critical
                    {
                        if (!visited[neighbour]) {
                            visited[neighbour] = true;
                            q.push(neighbour);
                        }
                    }
                }
            }
        }
    }

    // -------- Sequential DFS --------
    void sequentialDFS(int start) {
        vector<bool> visited(v, false);
        sequentialDFSUtil(start, visited);
    }

    void sequentialDFSUtil(int node, vector<bool>& visited) {
        visited[node] = true;

        for (int neighbour : adj[node]) {
            if (!visited[neighbour]) {
                sequentialDFSUtil(neighbour, visited);
            }
        }
    }

    // -------- Parallel DFS (FIXED) --------
    void parallelDFS(int start) {
        vector<bool> visited(v, false);

#pragma omp parallel
        {
#pragma omp single
            {
                parallelDFSUtil(start, visited);
            }
        }
    }

    void parallelDFSUtil(int node, vector<bool>& visited) {
#pragma omp critical
        {
            if (visited[node]) return;
            visited[node] = true;
        }

        for (int neighbour : adj[node]) {
#pragma omp task
            parallelDFSUtil(neighbour, visited);
        }

#pragma omp taskwait
    }
};

// -------- MAIN --------
int main() {
    Graph g(7);

    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(1, 4);
    g.addEdge(2, 5);
    g.addEdge(2, 6);

    g.printGraph();

    omp_set_num_threads(4);

    double start, stop;

    start = omp_get_wtime();
    g.sequentialBFS(0);
    stop = omp_get_wtime();
    cout << "Time Taken By Sequential BFS : " << stop - start << endl;

    start = omp_get_wtime();
    g.parallelBFS(0);
    stop = omp_get_wtime();
    cout << "Time Taken By Parallel BFS : " << stop - start << endl;

    start = omp_get_wtime();
    g.sequentialDFS(0);
    stop = omp_get_wtime();
    cout << "Time Taken By Sequential DFS : " << stop - start << endl;

    start = omp_get_wtime();
    g.parallelDFS(0);
    stop = omp_get_wtime();
    cout << "Time Taken By Parallel DFS : " << stop - start << endl;

    return 0;
}




'''
sudo apt update
sudo apt install g++
sudo apt install build-essential libomp-dev
g++ -fopenmp bfs_dfs.cpp -o bfs_dfs
./bfs_dfs