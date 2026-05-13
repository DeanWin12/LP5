#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <omp.h>

using namespace std;

const int MAX = 100;

// Graph representation
vector<int> graph[MAX];
bool visited[MAX];

// ================= PARALLEL BFS =================
void parallelBFS(int start)
{
    queue<int> q;

    // Reset visited array
    for(int i = 0; i < MAX; i++)
        visited[i] = false;

    visited[start] = true;
    q.push(start);

    cout << "\nParallel BFS Traversal:\n";

    while(!q.empty())
    {
        int size = q.size();

        #pragma omp parallel for
        for(int i = 0; i < size; i++)
        {
            int node;

            // Critical section for queue access
            #pragma omp critical
            {
                node = q.front();
                q.pop();

                cout << node << " ";
            }

            // Process adjacent nodes
            for(int j = 0; j < graph[node].size(); j++)
            {
                int adj = graph[node][j];

                if(!visited[adj])
                {
                    #pragma omp critical
                    {
                        if(!visited[adj])
                        {
                            visited[adj] = true;
                            q.push(adj);
                        }
                    }
                }
            }
        }
    }

    cout << endl;
}

// ================= PARALLEL DFS =================
void parallelDFS(int start)
{
    stack<int> s;

    // Reset visited array
    for(int i = 0; i < MAX; i++)
        visited[i] = false;

    s.push(start);

    cout << "\nParallel DFS Traversal:\n";

    while(!s.empty())
    {
        int curr;

        // Critical section for stack access
        #pragma omp critical
        {
            curr = s.top();
            s.pop();
        }

        if(!visited[curr])
        {
            visited[curr] = true;

            cout << curr << " ";

            // Parallel exploration of neighbors
            #pragma omp parallel for
            for(int i = 0; i < graph[curr].size(); i++)
            {
                int adj = graph[curr][i];

                if(!visited[adj])
                {
                    #pragma omp critical
                    {
                        s.push(adj);
                    }
                }
            }
        }
    }

    cout << endl;
}

// ================= MAIN FUNCTION =================
int main()
{
    int n, m, start;

    cout << "Enter Number of Nodes: ";
    cin >> n;

    cout << "Enter Number of Edges: ";
    cin >> m;

    cout << "Enter Edges (u v):\n";

    for(int i = 0; i < m; i++)
    {
        int u, v;
        cin >> u >> v;

        graph[u].push_back(v);
        graph[v].push_back(u);
    }

    cout << "Enter Starting Node: ";
    cin >> start;

    // Perform Parallel BFS
    parallelBFS(start);

    // Perform Parallel DFS
    parallelDFS(start);

    return 0;
}