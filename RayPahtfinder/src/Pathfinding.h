#pragma once
#include "Graph.h"          
#include <map>              
#include <queue>           
#include <limits>           
#include <vector>          
#include <utility>          

// Definición de la clase Pathfinding
class Pathfinding {
public:
    
    Pathfinding(const Graph& graph);
    // Función principal para encontrar la ruta más corta.
    std::list<int> findShortestPath(int startNodeId, int endNodeId) const;

private:
    const Graph& m_graph;

    // Función heurística para el algoritmo A*.
    float calculateHeuristic(int nodeId, int endNodeId) const;
};