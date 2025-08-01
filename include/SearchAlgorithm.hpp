#pragma once

#include "Graph.hpp"
#include "MyVector.hpp"
#include "SimpleMap.hpp"
#include "list.hpp"
#include "pair.hpp"
#include "queue.hpp"
#include <limits>
#include <string>

class SearchAlgorithm {
public:
  virtual SimpleList<int> findPath(const Graph& graph, int startNodeId,
                                   int endNodeId) const = 0;
  virtual std::string getName() const = 0;
  virtual ~SearchAlgorithm() = default;
  
};

// --- Functor para tu algoritmo A* original ---
class AStar_Algorithm : public SearchAlgorithm {
public:
  SimpleList<int> findPath(const Graph& graph, int startNodeId, int endNodeId) const override;
  std::string getName() const override {
    return "A* (A-Star)";
  }

private:
  float calculateHeuristic(const Graph& graph, int nodeId1, int nodeId2) const;
  SimpleList<int> reconstructPath(const SimpleMap<int, int>& cameFrom, int currentId) const;
};

// --- Functor para Breadth-First Search (BFS) ---
class BFS_Algorithm : public SearchAlgorithm {
public:
  SimpleList<int> findPath(const Graph& graph, int startNodeId,
                           int endNodeId) const override;
  std::string getName() const override {
    return "BFS (Breadth-First Search)";
  }
private:
  SimpleList<int> reconstructPath(const SimpleMap<int, int>& cameFrom,
                                  int currentId) const;
};

// --- Functor para Dijkstra ---
class Dijkstra_Algorithm : public SearchAlgorithm {
public:
  SimpleList<int> findPath(const Graph& graph, int startNodeId,
                           int endNodeId) const override;
  std::string getName() const override {
    return "Dijkstra";
  }
private:
  SimpleList<int> reconstructPath(const SimpleMap<int, int>& cameFrom,
                                  int currentId) const;
};

// FUnctor para Best-First-Search 

class BestFirst_Algorithm : public SearchAlgorithm {
public:
  SimpleList<int> findPath(const Graph& graph, int startNodeId, int endNodeId) const override;
  std::string getName() const override {
    return "Best-First Search";
  }
private:
  SimpleList<int> reconstructPath(const SimpleMap<int, int>& cameFrom, int currentId) const;
};


//Functos para DFS

class DFS_Algorithm : public SearchAlgorithm {
public:
    DFS_Algorithm() = default;
    ~DFS_Algorithm() override = default;

    SimpleList<int> findPath(const Graph& graph, int startNodeId, int endNodeId) const override;
    std::string getName() const override { return "Depth-First Search"; }

private:
    SimpleList<int> reconstructPath(const SimpleMap<int, int>& cameFrom, int currentId) const;
};