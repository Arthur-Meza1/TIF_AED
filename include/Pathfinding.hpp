#pragma once

#include "Graph.hpp"
#include "MyVector.hpp"
#include "Node.hpp"
#include "SimpleMap.hpp"
#include "list.hpp"
#include <utility>

class Pathfinding {
public:
  Pathfinding(const Graph& g);

  SimpleList<int> findPath(int startNodeId, int endNodeId);

private:
  const Graph& graph;

  MyVector<float> gScore;
  MyVector<float> fScore;
  SimpleMap<int, int>
      cameFrom; // Almacena el ID del nodo precedente en el camino óptimo
  MyVector<bool> closedSet; // Para saber si un nodo ya ha sido evaluado

  // Función heurística (distancia euclidiana)
  float calculateHeuristic(int nodeId1, int nodeId2) const;

  SimpleList<int> reconstructPath(int currentId) const;
};
