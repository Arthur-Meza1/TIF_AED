#include "SearchAlgorithm.hpp" 
#include "queue.hpp"
#include "Utils.hpp"
#include "Graph.hpp"
#include "pair.hpp"
#include "SimpleMap.hpp"
#include <limits>
#include <iostream>

// Función para encontrar el camino usando Best-First Search
SimpleList<int> BestFirst_Algorithm::findPath(const Graph& graph, int startNodeId, int endNodeId) const {
  if (startNodeId < 0 || startNodeId >= graph.getNumNodes() || endNodeId < 0 || endNodeId >= graph.getNumNodes()) {
    return {};
  }
  
  // Usamos tu PriorityQueue con tu clase Pair para {prioridad, nodo_id}
  PriorityQueue<Pair<float, int>> openSet;
  SimpleMap<int, int> cameFrom;

  // NUEVO: Mapa para rastrear los nodos que ya han sido visitados
  SimpleMap<int, bool> visited;

  const raylib::Vector2 endPos = graph.getNode(endNodeId).position;
  const raylib::Vector2 startPos = graph.getNode(startNodeId).position;
  float initialHeuristic = Utils::GetDistance(startPos, endPos);
  
  openSet.push(Pair<float, int>(initialHeuristic, startNodeId));
  visited.set(startNodeId, true); // Marcamos el nodo de inicio como visitado

  while (!openSet.empty()) {
    Pair<float, int> currentPair = openSet.top();
    int currentId = currentPair.second;
    openSet.pop();

    if (currentId == endNodeId) {
      return reconstructPath(cameFrom, currentId);
    }
    
    const auto& neighbors = graph.getAdjacentNodes(currentId);
    for (const auto& neighborPair : neighbors) {
      int neighborId = neighborPair.first;

      // CAMBIO CLAVE: Verificamos si el vecino NO ha sido visitado
      if (!visited.contains(neighborId)) {
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

        cameFrom.set(neighborId, currentId);
        visited.set(neighborId, true); // Lo marcamos como visitado al agregarlo a la cola
        
        float heuristic = Utils::GetDistance(neighborPos, endPos);
        openSet.push(Pair<float, int>(heuristic, neighborId));
      }
    }
  }

  return {}; // Camino no encontrado
}


SimpleList<int> BestFirst_Algorithm::reconstructPath(const SimpleMap<int, int>& cameFrom, int currentId) const {
  MyVector<int> temp;
  
  // Reconstruimos la ruta en orden inverso
  while (cameFrom.contains(currentId)) {
    temp.push_back(currentId);
    currentId = cameFrom.get(currentId);
  }
  temp.push_back(currentId);

  // Llenamos la SimpleList en el orden correcto
  SimpleList<int> totalPath;
  for (int i = temp.size() - 1; i >= 0; --i) {
    totalPath.push_back(temp[i]);
  }
  return totalPath;
}