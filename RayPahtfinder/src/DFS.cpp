#include "DFS.h"
#include <algorithm>

SimpleList<int> findPathDFS(const Graph& graph, int startNodeId, int endNodeId) {
    if (startNodeId < 0 || startNodeId >= graph.getNumNodes() ||
        endNodeId < 0 || endNodeId >= graph.getNumNodes()) {
        return {};
    }
    SimpleList<int> path;
    MyVector<bool> visited(graph.getNumNodes(), false);
    if (dfsRecursive(graph, startNodeId, endNodeId, path, visited)) {
        SimpleList<int> correctPath;
        while (!path.empty()) {
            correctPath.push_front(path.back());
            path.pop_back();
        }
        return correctPath;
    }
    return SimpleList<int>();
}

bool dfsRecursive(const Graph& graph, int currentId, int endNodeId, SimpleList<int>& path, MyVector<bool>& visited) {
    visited[currentId] = true;
    path.push_back(currentId);
    if (currentId == endNodeId) {
        return true;
    }
    const raylib::Vector2 currentPos = graph.getNode(currentId).position;
    const auto& neighbors = graph.getAdjacentNodes(currentId);
    for (const auto& neighborPair : neighbors) {
        int neighborId = neighborPair.first;
        if (!visited[neighborId]) {
            const raylib::Vector2 neighborPos = graph.getNode(neighborId).position;
            bool isBlocked = false;
            for (const auto& obstacle : graph.getObstacles()) {
                if (CheckCollisionPointRec(currentPos, obstacle.rect) ||
                    CheckCollisionPointRec(neighborPos, obstacle.rect)) {
                    isBlocked = true;
                    break;
                }
            }
            if (!isBlocked) {
                if (dfsRecursive(graph, neighborId, endNodeId, path, visited)) {
                    return true;
                }
            }
        }
    }
    path.pop_back();
    return false;
}
