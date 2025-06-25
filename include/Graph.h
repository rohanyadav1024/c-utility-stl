#ifndef Graph_H
#define Graph_H

#include <iostream>
#include <vector>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <stack>
#include <set>
#include <algorithm>
#include <stdexcept>
#include <utility>

struct PAIR_HASH{
    template<typename T1, typename T2>
    std::size_t operator()(const std::pair<T1, T2> &pair) const{
        return std::hash<T1>{}(pair.first) ^ std::hash<T2>{}(pair.second);
    }
};

template <typename T>
class Graph {
    private:
        std::unordered_map<T, std::unordered_set<std::pair<T, int> , PAIR_HASH>> adjList;
        bool isDirected;
        bool isWeighted;
        bool isNegativelyWeighted;
        std::unordered_map<T, bool> visited;
       
        void addEdgeHelper(T u, T v, int w = 1){
            adjList[u].insert({v, w});
        }

        void removeEdgeHelper(T u, T v){
            adjList[u].erase(v);
        }

        bool detectCycleUndirectedDFS();
        bool detectCycleDirectedDFS();

        void topoSortDFS(T start, std::stack<T> &s);

        void getDistanceTopoDFS(T start, std::vector<std::pair<T, int>> &disVector);
        void getDistanceBFS(T start, std::vector<std::pair<T, int>> &disVector);
        void getDistanceDijkstra(T start, std::vector<std::pair<T, int>> &disVector);
        void getDistanceBellmanFord(T start, std::vector<std::pair<T, int>> &disVector);

    public:

        Graph() : isDirected(false), isWeighted(false){}
        Graph(const bool isDirected) : isDirected(isDirected), isWeighted(false){}
        Graph(const bool isDirected, const bool isWeighted) : isDirected(isDirected), isWeighted(isWeighted) {}
        // for unweighted graph
        Graph(const std::vector<std::pair<T, T>>& edges, bool isDirected = false): isDirected(isDirected), isWeighted(false){
            for(auto& [u, v] : edges){
                addEdgeHelper(u, v, 1);
                if(!isDirected){
                    addEdgeHelper(v, u, 1);
                }
            }
        }
        // for weighted graph
        Graph(const std::vector<std::tuple<T, T, int>>& edges, bool isDirected = false): isDirected(isDirected), isWeighted(true){
            for(auto& [u, v, w] : edges){
                addEdgeHelper(u, v, w);
                if(!isDirected){
                    addEdgeHelper(v, u, w);
                }
            }
        }

        void addEdge(T u, T v, int w = 1);
        void removeEdge(T u, T v);
        void addVertex(T v);
        void removeVertex(T v);
        void printGraph();
        std::vector<T> getVertices();
        std::vector<std::pair<T, std::pair<T, int>> > getEdges();
        std::vector<T> getBFS(T start);
        std::vector<T> getDFS(T start);
        bool hasCycle();
        std::vector<T> getTopologicalOrderDFS(); // DFS based topological order
        std::vector<T> getTopologicalOrderBFS(); // Kahn's algorithm using bfs
        std::vector<std::pair<T, int>> getSinglePointShortestPath(T start);
        std::vector<std::pair<int, std::pair<T, int> >> getAllShortestPath();
        std::vector<std::pair<T, std::pair<T, int>>> getMST();

};

template <typename T>
bool Graph<T>::detectCycleUndirectedDFS(){
    std::unordered_map<T, T> parent;
    for(auto& [u, _] : adjList){
        if(visited.find(u) == visited.end()){
            visited[u] = true;
            parent[u] = u;
            std::stack<T> s;
            s.push(u);
            while(!s.empty()){
                T node = s.top();
                s.pop();
                for(auto& [neighbor, _] : adjList[node]){
                    if(visited.find(neighbor) == visited.end()){
                        visited[neighbor] = true;
                        parent[neighbor] = node;
                        s.push(neighbor);
                    }
                    else if(parent[node] != neighbor){
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

template <typename T>
bool Graph<T>:: detectCycleDirectedDFS() {
            visited.clear();
            std::unordered_map<T, bool> recStack;
            std::stack<std::pair<T, bool>> s;

            for (auto& [u, _] : adjList) {
                if (visited.find(u) == visited.end()) {
                    s.push({u, false});

                    while (!s.empty()) {
                        T node = s.top().first;
                        bool processing = s.top().second;
                        s.pop();

                        if (processing) {
                            // We're done processing this node, remove it from the recursion stack
                            recStack[node] = false;
                        } else {
                            if (recStack[node]) {
                                // Node is already in the recursion stack, cycle detected
                                return true;
                            }

                            // Mark this node as being processed
                            recStack[node] = true;
                            visited[node] = true;
                            s.push({node, true});
                            // Add all neighbors to the stack
                            for (auto& [neighbor, _] : adjList[node]) {
                                if (visited.find(neighbor) == visited.end()) {
                                    s.push({neighbor, false});
                                } else if (recStack[neighbor]) {
                                    // If neighbor is in the recursion stack, we found a cycle
                                    return true;
                                }
                            }
                        }
                    }
                }
            }

            return false;
        }

template <typename T>
void Graph<T>::topoSortDFS(T node, std::stack<T> &s){

    visited[node] = 1;
        for(auto &[v, w] : adjList[node]){
            if(visited.find(v) == visited.end()){
                topoSortDFS(v, s);
            }
        }
        s.push(node);
    return;
}

template <typename T>
void Graph<T>::getDistanceBFS(T start, std::vector<std::pair<T, int>> &disVector)
{
    std::unordered_map<T, int> distance;
    std::queue<T> q;
    for (auto &[u, neighbours] : adjList)
    {
        distance[u] = INT_MAX;
        for (auto &[v, w] : neighbours)
        {
            distance[v] = INT_MAX;
        }
    }
    distance[start] = 0;
    q.push(start);
    while (!q.empty())
    {
        T front = q.front();
        q.pop();
        for (auto &[ngb, w] : adjList[front])
        {
            if (distance[front] + 1 < distance[ngb])
            {
                distance[ngb] = distance[front] + 1;
                q.push(ngb);
            }
        }
    }
    for (auto &[node, dis] : distance)
    {
        dis = dis == INT_MAX ? -1 : dis;
        disVector.push_back({node, dis});
    }
    // return disVector;
}

template <typename T>
void Graph<T>::getDistanceTopoDFS(T start, std::vector<std::pair<T, int>> &disVector){

    visited.clear();
    std::stack<T> st;
    std::unordered_map<T, int> distance;
    for (auto &[u, neighbours] : adjList){
        distance[u] = INT_MAX;
        for (auto &[v, w] : neighbours){
            distance[v] = INT_MAX;
        }
    }

    for(auto &v: getVertices()){
        if(visited.find(v) == visited.end()){
            topoSortDFS(v, st);
        }
    }

    distance[start] = 0;

    while (!st.empty())
    {
        T top = st.top();
        st.pop();
        for (auto &[ngb, w] : adjList[top]){
            if (distance[top] + w < distance[ngb]){
                distance[ngb] = distance[top] + w;
            }
        }
    }

    for (auto &[node, dis] : distance){
        dis = dis == INT_MAX ? -1 : dis;
        disVector.push_back({node, dis});
    }
    // return disVector;
}


template <typename T>
void Graph<T>::getDistanceDijkstra(T start, std::vector<std::pair<T, int>> &disVector){
    std::set<std::pair<int, T>> s;
    std::unordered_map<T, int> distance;

    for(auto &[u, neighbours] : adjList){
        distance[u] = INT_MAX;
        for(auto &[v, w] : neighbours){
            distance[v] = INT_MAX;
        }
    }

    s.insert({0, start});
    distance[start] = 0;

    while(!s.empty()){
        auto [dis, node] = *s.begin();
        s.erase(s.begin());

        for(auto &[ngb, w] : adjList[node]){
            if(dis + w < distance[ngb]){
                if(distance[ngb] != INT_MAX){
                    s.erase({distance[ngb], ngb});
                }
                distance[ngb] = dis + w;
                s.insert({distance[ngb], ngb});
            }
        }
    }

    for(auto &[node, dis] : distance){
        dis = (dis == INT_MAX ? -1 : dis);
        disVector.push_back({node, dis});
    }
}


template <typename T>
void Graph<T>::getDistanceBellmanFord(T start, std::vector<std::pair<T, int>> &disVector){
    std::unordered_map<T, int> distance;

    for(auto &[u, neighbours] : adjList){
        distance[u] = INT_MAX;
        for(auto &[v, w] : neighbours){
            distance[v] = INT_MAX;
        }
    }

    distance[start] = 0;

    for(int i=0; i<getVertices().size(); ++i){
        for(auto &[u, ngbs] : adjList){
            for(auto &[v, w] : ngbs){
                if(distance[u] != INT_MAX && distance[u] + w < distance[v]){
                    distance[v] = distance[u] + w;
                }
            }
        }
    }

    for(auto &[u, ngbs] : adjList){
        for(auto &[v, w] : ngbs){
            if(distance[u] != INT_MAX && distance[u] + w < distance[v]){
                throw std::runtime_error("Graph has a negative weight cycle");
            }
        }
    }

    for(auto &[node, dis] : distance){
        dis = (dis == INT_MAX ? -1 : dis);
        disVector.push_back({node, dis});
    }
}

template<typename T>
void Graph<T>::addEdge(T u, T v, int w){
    // if(isWeighted && w < 0){
    //     throw std::invalid_argument("Weight cannot be negative");
    // }
    if(!isWeighted){
        w = 1;
    }

    addEdgeHelper(u, v, w);
    if(!isDirected){
        addEdgeHelper(v, u, w);
    }
}

template<typename T>
void Graph<T>::removeEdge(T u, T v){
    removeEdgeHelper(u, v);
    if(!isDirected){
        removeEdgeHelper(v, u);
    }
}

template<typename T>
void Graph<T>::addVertex(T v){
    if(adjList.find(v) == adjList.end()){
        adjList[v] = {};
    }
}

template<typename T>
void Graph<T>::removeVertex(T v){
    adjList.erase(v);
    for(auto& [u, neighbors] : adjList){
        neighbors.erase(v);
    }
}

template<typename T>
void Graph<T>::printGraph(){
    for(auto& [u, neighbors] : adjList){
        std::cout << u << " -> ";
        for(auto& [v, w] : neighbors){
            std::cout << "(" << v << ", " << w << ") ";
        }
        std::cout << std::endl;
    }
}

template<typename T>
std::vector<T> Graph<T>::getVertices(){
    std::vector<T> vertices;
    for(auto& [u, neighbors] : adjList){
        vertices.push_back(u);
    }
    return vertices;
}

template<typename T>
std::vector<std::pair<T, std::pair<T, int>> > Graph<T>::getEdges(){
    std::vector<std::pair<T, std::pair<T, int>> > edges;
    for(auto& [u, neighbors] : adjList){
        for(auto& [v, w] : neighbors){
            edges.push_back({u, {v, w}});
        }
    }
    return edges;
}

template<typename T>
std::vector<T> Graph<T>::getBFS(T start){
    if(adjList.find(start) == adjList.end()){
        throw std::invalid_argument("Vertex not found");
    }

    // implement BFS
    std::vector<T> bfs;
    std::queue<T> q;
    q.push(start);
    visited.clear();
    visited[start] = true;
    
    while (!q.empty()){
        T node = q.front();
        q.pop();
        bfs.push_back(node);
        for(auto& [neighbor, _] : adjList[node]){
            if(visited.find(neighbor) == visited.end()){
                visited[neighbor] = true;
                q.push(neighbor);
            }
        }
    }
    return bfs;
}


template<typename T>
std::vector<T> Graph<T>::getDFS(T start){
    if(adjList.find(start) == adjList.end()){
        throw std::invalid_argument("Vertex not found");
    }

    // implement DFS
    std::vector<T> dfs;
    std::stack<T> s;
    s.push(start);
    visited.clear();
    visited[start] = true;
    
    while (!s.empty()){
        T node = s.top();
        s.pop();
        dfs.push_back(node);
        for(auto& [neighbor, _] : adjList[node]){
            if(visited.find(neighbor) == visited.end()){
                visited[neighbor] = true;
                s.push(neighbor);
            }
        }
    }
    return dfs;
}

template<typename T>
bool Graph<T>::hasCycle(){
    if(isDirected){
        return detectCycleDirectedDFS();
    }

    return detectCycleUndirectedDFS();
}

template<typename T>
std::vector<T> Graph<T>::getTopologicalOrderDFS(){
    if(hasCycle()) {
        throw std::runtime_error("Graph has a cycle. Topological sort not possible.");
    }

    visited.clear();
    std::vector<T> topologicalOrder;
    std::stack<std::pair<bool, T>> recStack;
    std::stack<T> postOrder;

    for(auto& [u, _] : adjList){
        if(visited.find(u) == visited.end()){
            // visited[u] = true;
            recStack.push({false, u});

            while(!recStack.empty()){
                std::pair<bool, T> node = recStack.top();
                recStack.pop();

                if(node.first){
                    postOrder.push(node.second);
                    continue;
                }

                if(visited.find(node.second) != visited.end()){
                    continue;
                }

                visited[node.second] = true;
                recStack.push({true, node.second});
                for(auto& [neighbor, _] : adjList[node.second]){
                    if(visited.find(neighbor) == visited.end()){
                        // visited[neighbor] = true;
                        recStack.push({false, neighbor});
                    }
                }
            }
        }
    }

    while(!postOrder.empty()){
        topologicalOrder.push_back(postOrder.top());
        postOrder.pop();
    }
    return topologicalOrder;
}

template<typename T>
std::vector<T> Graph<T>::getTopologicalOrderBFS() {
    if(hasCycle()) {
        throw std::runtime_error("Graph has a cycle. Topological sort not possible.");
    }
    visited.clear();
    std::vector<T> topologicalOrder;
    std::unordered_map<T, int> inDegree;
    std::queue<T> q;

    // Initialize in-degree of all vertices to 0
    for (auto& u : getVertices()) {
        inDegree[u] = 0;
    }

    // Compute in-degree of each vertex
    for (auto& [u, neighbors] : adjList) {
        for (auto& [v, _] : neighbors) {
            inDegree[v]++;
        }
    }

    // Push all vertices with in-degree 0 to the queue
    for (auto& u : getVertices()) {
        if (inDegree[u] == 0) {
            q.push(u);
        }
    }

    // Process vertices in queue
    while (!q.empty()) {
        T node = q.front();
        q.pop();

        topologicalOrder.push_back(node);
        for (auto& [neighbor, _] : adjList[node]) {
            inDegree[neighbor]--;
            if (inDegree[neighbor] == 0) {
                q.push(neighbor);
            }
        }
    }

    return topologicalOrder;
}

template<typename T>
std::vector<std::pair<T, int>> Graph<T>::getSinglePointShortestPath(T start){

    if(adjList.find(start) == adjList.end()){
        throw std::invalid_argument("Vertex not found");
    }

    std::vector<std::pair<T, int>> distances;
    if(!isWeighted){
        if(hasCycle()){
            // for directed Acyclic graph TC: O(V+E)
            getDistanceTopoDFS(start, distances);
        }else{
            // for unweighted graph TC: O(V+E)
            getDistanceBFS(start, distances);
        }
    }
    else{
        if(isNegativelyWeighted){
            // for negative weights TC: O(VE)
            getDistanceBellmanFord(start, distances);
        }else{
            // for any graph which is not dense TC: O(E+V)logV
            getDistanceDijkstra(start, distances);
        }
    }

    return distances;
}


template <typename T>
std::vector<std::pair<T, std::pair<T, int>>> Graph<T> getMST(){
    std::vector<std::pair<T, T>> mst;

    return mst;
}

#endif







 // std::unordered_map<T, T> unionParent;
        // std::unordered_map<T, int> distance;
        // std::vector<T> path;
        // std::vector<T> cycle;
        // std::vector<T> topologicalOrder;
        // std::vector<std::vector<T>> allPaths;
        // std::vector<std::vector<T>> allCycles;
        // std::vector<std::vector<T>> allTopologicalOrders;
        // std::vector<T> articulationPoints;
        // std::vector<std::pair<T, T>> bridges;
        // std::vector<std::pair<T, T>> allEdges;
        // std::vector<std::pair<T, T>> allEdgesInMST;
        // std::vector<std::pair<T, T>> allEdgesNotInMST;
        // std::vector<std::pair<T, T>> allEdgesInShortestPath;
        // std::vector<std::pair<T, T>> allEdgesNotInShortestPath;
        // std::vector<std::pair<T, T>> allEdgesInShortestPathTree;
        // std::vector<std::pair<T, T>> allEdgesNotInShortestPathTree;
        // std::vector<std::pair<T, T>> allEdgesInShortestPathDAG;
        // std::vector<std::pair<T, T>> allEdgesNotInShortestPathDAG;
        // std::vector<std::pair<T, T>> allEdgesInShortestPathTreeDAG;
        // std::vector<std::pair<T, T>> allEdgesNotInShortestPathTreeDAG;
        // std::vector<std::pair<T, T>> allEdgesInShortestPathDAGTree;
        // std::vector<std::pair<T, T>> allEdgesNotInShortestPathDAGTree;
        // std::vector<std::pair<T, T>> allEdgesInShortestPathTreeDAGTree;
        // std::vector<std::pair<T, T>> allEdgesNotInShortestPathTreeDAGTree;
        // std::vector<std::pair<T, T>> allEdgesInShortestPathDAGDAG;
        // std::vector<std::pair<T, T>> allEdgesNotInShortestPathDAGDAG;
        // std::vector<std::pair<T, T>> allEdgesInShortestPathTreeDAGD
