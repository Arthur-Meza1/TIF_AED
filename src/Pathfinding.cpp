#include "Pathfinding.hpp"

Pathfinding::Pathfinding(const Graph& graph) : graph(graph) {
  
  algorithms.set("A*", std::make_unique<AStar_Algorithm>());
  algorithms.set("BFS", std::make_unique<BFS_Algorithm>());
  algorithms.set("BestFirst-Search", std::make_unique<BestFirst_Algorithm>());
  algorithms.set("DFS", std::make_unique<DFS_Algorithm>());
  algorithms.set("Dijkstra", std::make_unique<Dijkstra_Algorithm>());

  // Establecemos el algoritmo por defecto
  currentAlgorithm = algorithms.get("A*").get();
}

void Pathfinding::selectAlgorithm(const std::string& name) {
  auto it = algorithms.find(name);
  if (algorithms.contains(name)) {
    currentAlgorithm = it->value.get();
  } else {
    std::cout << "Algoritmo '" << name << "' no encontrado. Seleccionando A*." << std::endl;
    currentAlgorithm = algorithms.get("A*").get();
  }
}

SimpleList<int> Pathfinding::findPath(int startNodeId, int endNodeId) const {
   if(currentAlgorithm){
    return currentAlgorithm->findPath(this->graph, startNodeId, endNodeId);
  }
  return SimpleList<int>();
}

void Pathfinding::setAlgorithm(const SearchAlgorithm* algorithm) {
    if (algorithm) {
        this->currentAlgorithm = algorithm;
    }
}

std::string Pathfinding::getCurrentAlgorithmName() const {
    if(currentAlgorithm)
      return currentAlgorithm->getName();
    return "Ninguno";
}