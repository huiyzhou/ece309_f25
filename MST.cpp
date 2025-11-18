#include <iostream>
#include <utility>
#include <vector>
#include <queue>

using namespace std;

class Graph {
private:
    int n;  // number of vertices
    vector<vector<pair<int,int>>> adj; 
    // adj[u] = { {v, weight}, ... }

public:
    Graph(int n) : n(n), adj(n) {}

    void addEdge(int u, int v, int w) {
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    }

    // Prim's Minimum Spanning Tree
    pair<long long, vector<tuple<int,int,int>>> primMST(int start = 0) {
        vector<bool> used(n, false);
        long long mst_weight = 0;
        vector<tuple<int,int,int>> mst_edges;

        // Min-heap: {weight, current_node, parent_node}
        using T = tuple<int,int,int>;
        priority_queue<T, vector<T>, greater<T>> pq;

        // Start with the start node, parent = -1
        pq.emplace(0, start, -1);

        while (!pq.empty()) {
            auto [w, u, parent] = pq.top();
            pq.pop();

            if (used[u]) continue;
            used[u] = true;

            if (parent != -1) {
                mst_weight += w;
                mst_edges.emplace_back(parent, u, w);
            }

            for (auto &[v, weight] : adj[u]) {
                if (!used[v]) {
                    pq.emplace(weight, v, u);
                }
            }
        }

        return {mst_weight, mst_edges};
    }
};

int main() {
    // ---- Create a graph manually here ----
    int n = 6;
    Graph g(n);

    // Example edges (undirected graph)
    g.addEdge(0, 1, 4);
    g.addEdge(0, 2, 3);
    g.addEdge(1, 2, 1);
    g.addEdge(1, 3, 2);
    g.addEdge(2, 3, 4);
    g.addEdge(3, 4, 2);
    g.addEdge(4, 5, 6);
    g.addEdge(3, 5, 3);
    
    auto [weight, edges] = g.primMST();

    cout << "MST Weight = " << weight << "\n";
    cout << "Edges in MST:\n";
    for (auto &[u, v, w] : edges) {
        cout << u << " - " << v << " (" << w << ")\n";
    }

    return 0;
}
