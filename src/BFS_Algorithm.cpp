#include "SearchAlgorithm.hpp"
#include "Queue.hpp"
#include "SimpleMap.hpp"
#include "MyVector.hpp"
#include "Utils.hpp"

SimpleList<int> BFS_Algorithm::findPath(const Graph& graph, int startNodeId,
                                         int endNodeId) const {
  int numNodes = graph.getNumNodes();

  if (startNodeId < 0 || startNodeId >= numNodes || endNodeId < 0 ||
      endNodeId >= numNodes) {
    return {};
  }

  MyVector<bool> visited(numNodes, false);
  SimpleMap<int, int> cameFrom;
  Queue<int> q;

  q.push(startNodeId);
  visited[startNodeId] = true;

  while (!q.empty()) {
    int currentId = q.front();
    q.pop();

    if (currentId == endNodeId) {
      return reconstructPath(cameFrom, currentId);
    }

    const auto& neighbors = graph.getAdjacentNodes(currentId);
    for (const auto& neighborPair : neighbors) {
      int neighborId = neighborPair.first;

      if (!visited[neighborId]) {
        // Verifica colisión con obstáculos (similar a A*)
        const raylib::Vector2 currentPos = graph.getNode(currentId).position;
        const raylib::Vector2 neighborPos = graph.getNode(neighborId).position;
        bool isBlocked = false;
        for (const auto& obstacle : graph.getObstacles()) {
          if (Utils::CheckLineRectangleCollision(currentPos, neighborPos, obstacle.rect)) {
            isBlocked = true;
            break;
          }
        }
        if (isBlocked) continue;

        visited[neighborId] = true;
        cameFrom.set(neighborId, currentId);
        q.push(neighborId);
      }
    }
  }

  return {}; // Camino no encontrado
}

SimpleList<int> BFS_Algorithm::reconstructPath(const SimpleMap<int, int>& cameFrom,
                                             int currentId) const {
  MyVector<int> temp;
  while (true) {
    temp.push_back(currentId);
    if (!cameFrom.contains(currentId))
      break;
    int prev = cameFrom.get(currentId);
    if (prev == -1)
      break;
    currentId = prev;
  }

  SimpleList<int> totalPath;
  for (int i = temp.size() - 1; i >= 0; --i) {
    totalPath.push_back(temp[i]);
  }
  return totalPath;
}