#pragma once

#include "Estructuras_datos/vector.h"
#include "Estructuras_datos/list.h" 
#include "Node.h"
#include "Graph.h" 
#include <utility>


class Pathfinding {
public:
   
    Pathfinding(const Graph& g);

    
    SimpleList<int> findPath(int startNodeId, int endNodeId);

private:
    const Graph& graph; 

    // Vectores para almacenar los costos g, f, y los predecesores
    Vector<float> gScore;
    Vector<float> fScore;
    Vector<int> cameFrom; // Almacena el ID del nodo precedente en el camino óptimo
    Vector<bool> closedSet; // Para saber si un nodo ya ha sido evaluado

    // Función heurística (distancia euclidiana)
    float calculateHeuristic(int nodeId1, int nodeId2) const;

    
    SimpleList<int> reconstructPath(int currentId) const;
};