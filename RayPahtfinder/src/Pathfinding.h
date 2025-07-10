#pragma once

#include "Node.h"
#include "Graph.h" // Necesario para acceder al grafo y sus nodos/obstáculos
#include <vector>
#include <list>
#include <queue>
#include <utility> // Para std::pair

// Clase que implementa el algoritmo A*
class Pathfinding {
public:
    // Constructor
    Pathfinding(const Graph& g);

    // Función principal para encontrar el camino
    std::list<int> findPath(int startNodeId, int endNodeId);

private:
    const Graph& graph; // Referencia constante al grafo sobre el que se buscará

    // Vectores para almacenar los costos g, f, y los predecesores
    std::vector<float> gScore;
    std::vector<float> fScore;
    std::vector<int> cameFrom; // Almacena el ID del nodo precedente en el camino óptimo
    std::vector<bool> closedSet; // Para saber si un nodo ya ha sido evaluado

    // Función heurística (distancia euclidiana)
    float calculateHeuristic(int nodeId1, int nodeId2) const;

    // Función para reconstruir el camino desde el nodo final
    // ¡¡¡ASEGÚRATE DE QUE ESTA LÍNEA ESTÉ ASÍ EN TU Pathfinding.h!!!
    std::list<int> reconstructPath(int currentId) const; // <--- Añade 'const' aquí
};