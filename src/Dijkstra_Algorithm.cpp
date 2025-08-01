#include "SearchAlgorithm.hpp"
#include "Utils.hpp"
#include "pair.hpp"

SimpleList<int> Dijkstra_Algorithm::findPath(const Graph& graph, int startNodeId,
                                              int endNodeId) const {
  int numNodes = graph.getNumNodes();

  if (startNodeId < 0 || startNodeId >= numNodes || endNodeId < 0 ||
      endNodeId >= numNodes) {
    return {};
  }

  MyVector<float> dist(numNodes, std::numeric_limits<float>::infinity());
  SimpleMap<int, int> cameFrom;
  PriorityQueue<Pair<float, int>> pq; 

  dist[startNodeId] = 0;
  pq.push({0, startNodeId});

  while (!pq.empty()) {
    float d = pq.top().first;
    int currentId = pq.top().second;
    pq.pop();

    if (currentId == endNodeId) {
      return reconstructPath(cameFrom, currentId);
    }

    if (d > dist[currentId]) {
      continue;
    }

    const auto& neighbors = graph.getAdjacentNodes(currentId);
    for (const auto& neighborPair : neighbors) {
      int neighborId = neighborPair.first;
      float edgeCost = neighborPair.second;

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

      float newDist = dist[currentId] + edgeCost;
      if (newDist < dist[neighborId]) {
        dist[neighborId] = newDist;
        cameFrom.set(neighborId, currentId);
        pq.push({newDist, neighborId});
      }
    }
  }

  return {}; // Camino no encontrado
}

SimpleList<int> Dijkstra_Algorithm::reconstructPath(const SimpleMap<int, int>& cameFrom,
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