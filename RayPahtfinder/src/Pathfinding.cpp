#include "Pathfinding.h"
#include "Utils.h"
#include <functional> 


Pathfinding::Pathfinding(const Graph& graph) : m_graph(graph) {}

//hueristica 
float Pathfinding::calculateHeuristic(int nodeId, int endNodeId) const {
    return GetDistance(m_graph.getNode(nodeId).position, m_graph.getNode(endNodeId).position);
}

// Implementación del algoritmo A*
std::list<int> Pathfinding::findShortestPath(int startNodeId, int endNodeId) const {
    std::list<int> path; 
    // Casos base: si no hay inicio/fin o si son el mismo nodo
    if (startNodeId == -1 || endNodeId == -1) return path;
    if (startNodeId == endNodeId) {
        path.push_back(startNodeId);
        return path;
    }

    // costo del camino más barato desde el nodo inicial hasta n.
    std::map<int, float> gScore;
    // costo total estimado desde inical hasta nodo objetivo  (gScore[n] + heurística).
    std::map<int, float> fScore;
    // cameFrom[n]: El predecesor de n en el camino más barato encontrado hasta ahora.
    std::map<int, int> cameFrom;


    std::priority_queue<std::pair<float, int>, std::vector<std::pair<float, int>>, std::greater<std::pair<float, int>>> openSet;

    //valores infinitos 
    for (int i = 0; i < m_graph.getNumNodes(); ++i) {
        gScore[i] = std::numeric_limits<float>::max();
        fScore[i] = std::numeric_limits<float>::max();
    }

    // costo inicil 0
    gScore[startNodeId] = 0;
    // El fScore inicial es solo la heurística desde el inicio al fin.
    fScore[startNodeId] = calculateHeuristic(startNodeId, endNodeId);
    // Añade el nodo inicial al openSet
    openSet.push({fScore[startNodeId], startNodeId});

    //principal
    while (!openSet.empty()) {
        int currentId = openSet.top().second; // Obtiene el nodo con el fScore más bajo
        openSet.pop();                        // Lo retira del openSet

        // Si el nodo actual es el objetivo, hemos encontrado el camino
        if (currentId == endNodeId) {
            // Reconstruir la ruta usandos el mapa cameFrom
            int temp = endNodeId;
            while (cameFrom.count(temp)) { // Mientras el nodo actual tenga un predecesor
                path.push_front(temp);     // Añade el nodo al inicio de la lista de la ruta
                temp = cameFrom[temp];     // Mueve al predecesor
            }
            path.push_front(startNodeId); // Añade el nodo inicial al inicio de la ruta
            return path; // Retorna la ruta encontrada
        }

        // Explora los vecinos del nodo actual
        const auto& neighbors = m_graph.getNeighbors(currentId);
        for (const auto& neighborPair : neighbors) {
            int neighborId = neighborPair.first;    // ID del vecino
            float weight = neighborPair.second;     // Costo de la arista al vecino

            // Calcula el costo tentativo desde el inicio al vecino pasando por el nodo actual
            float tentative_gScore = gScore[currentId] + weight;

            // Si este camino es mejor que cualquier otro encontrado hasta ahora para este vecino
            if (tentative_gScore < gScore[neighborId]) {
                cameFrom[neighborId] = currentId;        // Actualiza el predecesor del vecino
                gScore[neighborId] = tentative_gScore;   // Actualiza el gScore del vecino
                // Actualiza el fScore del vecino (gScore + heurística)
                fScore[neighborId] = gScore[neighborId] + calculateHeuristic(neighborId, endNodeId);
                openSet.push({fScore[neighborId], neighborId}); // Añade o actualiza el vecino en el openSet
            }
        }
    }

    return path; 
}