#pragma once

#include "Graph.hpp"
#include "MyVector.hpp"
#include "Node.hpp"
#include "SimpleMap.hpp"
#include "SearchAlgorithm.hpp"
#include "list.hpp"
#include <memory>
#include <utility>

class Pathfinding {
public:
  Pathfinding(const Graph& graph);
  ~Pathfinding() = default;

  Pathfinding(const Pathfinding&) = delete;
  Pathfinding& operator=(const Pathfinding&) = delete;

  // Métodos para el control del algoritmo
  void setAlgorithm(const SearchAlgorithm* algorithm);
  void selectAlgorithm(const std::string& name);
  SimpleList<int> findPath(int startNodeId, int endNodeId) const;
  std::string getCurrentAlgorithmName() const;

private:
  const Graph& graph;
  SimpleMap<std::string, std::unique_ptr<SearchAlgorithm>> algorithms;
   const SearchAlgorithm* currentAlgorithm = nullptr;
};
