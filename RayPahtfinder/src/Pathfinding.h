#pragma once

#include "Node.h"
#include "Graph.h" 
#include <vector>
#include <list>
#include <queue>
#include <utility>


class Pathfinding {
public:
   
    Pathfinding(const Graph& g);

    
    std::list<int> findPath(int startNodeId, int endNodeId);

private:
    const Graph& graph; 

    // Vectores para almacenar los costos g, f, y los predecesores
    std::vector<float> gScore;
    std::vector<float> fScore;
    std::vector<int> cameFrom; // Almacena el ID del nodo precedente en el camino óptimo
    std::vector<bool> closedSet; // Para saber si un nodo ya ha sido evaluado

    // Función heurística (distancia euclidiana)
    float calculateHeuristic(int nodeId1, int nodeId2) const;

    
    std::list<int> reconstructPath(int currentId) const; 
};