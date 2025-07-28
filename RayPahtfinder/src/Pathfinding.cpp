#include "Pathfinding.h"
#include <algorithm>        
#include <limits>
#include "Estructuras_datos/queue.h"
#include "Estructuras_datos/MyVector.h"      
#include "Estructuras_datos/pair.h"      

// Constructor del Pathfinding, recibe una referencia constante al grafo
Pathfinding::Pathfinding(const Graph& g) : graph(g) {
    int n = g.getNumNodes();
    gScore.resize(n, std::numeric_limits<float>::infinity());
    fScore.resize(n, std::numeric_limits<float>::infinity());
    cameFrom.resize(n, -1);
    closedSet.resize(n, false);
}



// Calcula la distancia euclidiana entre dos nodos (heurística)
float Pathfinding::calculateHeuristic(int nodeId1, int nodeId2) const {
    const Node& n1 = graph.getNode(nodeId1);
    const Node& n2 = graph.getNode(nodeId2);
    return Vector2Distance(n1.position, n2.position);
}
bool CheckLineRectangleCollisionCustom(raylib::Vector2 p1, raylib::Vector2 p2, raylib::Rectangle rec) {
    if (CheckCollisionPointRec(p1, rec) || CheckCollisionPointRec(p2, rec)) {
        return true;
    }
    if (CheckCollisionLines(p1, p2, {rec.x, rec.y}, {rec.x + rec.width, rec.y}, nullptr)) return true;
    if (CheckCollisionLines(p1, p2, {rec.x, rec.y + rec.height}, {rec.x + rec.width, rec.y + rec.height}, nullptr)) return true;
    if (CheckCollisionLines(p1, p2, {rec.x, rec.y}, {rec.x, rec.y + rec.height}, nullptr)) return true;
    if (CheckCollisionLines(p1, p2, {rec.x + rec.width, rec.y}, {rec.x + rec.width, rec.y + rec.height}, nullptr)) return true;

    return false;
}
// Implementación del algoritmo A*
SimpleList<int> Pathfinding::findPath(int startNodeId, int endNodeId) {
    // Validar IDs de nodos
    if (startNodeId < 0 || startNodeId >= gScore.size() ||
        endNodeId < 0 || endNodeId >= gScore.size()) {
       
        return {}; 
    }

    // Reiniciar estructuras para una nueva búsqueda
    gScore.resize(gScore.size(), std::numeric_limits<float>::infinity());
    fScore.resize(gScore.size(), std::numeric_limits<float>::infinity());
    cameFrom.resize(gScore.size(), -1);
    closedSet.resize(gScore.size(), false);


    PriorityQueue<std::pair<float, int>> openSet;


    gScore[startNodeId] = 0;
    fScore[startNodeId] = calculateHeuristic(startNodeId, endNodeId);
    openSet.push({fScore[startNodeId], startNodeId});

    while (!openSet.empty()) {
        int currentId = openSet.top().second;
        openSet.pop();

        if (currentId == endNodeId) {
            return reconstructPath(currentId);
        }

        if (closedSet[currentId]) {
            continue;
        }

        closedSet[currentId] = true;

        
        const raylib::Vector2 currentPos = graph.getNode(currentId).position;

        const auto& neighbors = graph.getAdjacentNodes(currentId); 

        for (const auto& neighborPair : neighbors) {
            int neighborId = neighborPair.first;
            float edgeCost = neighborPair.second;

            if (closedSet[neighborId]) {
                continue;
            }

            //verificacion de obstaculos 
            const raylib::Vector2 neighborPos = graph.getNode(neighborId).position;
            bool isBlocked = false;
            for (const auto& obstacle : graph.getObstacles()) {
                if (CheckLineRectangleCollisionCustom(currentPos, neighborPos, obstacle.rect)) {
                    isBlocked = true;
                     break;
                }
            }
            if (isBlocked) {
                continue;
            }
            // Resto de la lógica A*
            float tentative_gScore = gScore[currentId] + edgeCost;

            if (tentative_gScore < gScore[neighborId]) {
                cameFrom[neighborId] = currentId;
                gScore[neighborId] = tentative_gScore;
                fScore[neighborId] = gScore[neighborId] + calculateHeuristic(neighborId, endNodeId);
                openSet.push({fScore[neighborId], neighborId});
            }
        }
    }

    return SimpleList<int>(); //no encontrado
}

//reconstruccion del camino
SimpleList<int> Pathfinding::reconstructPath(int currentId) const {
    // Usamos tu Vector en lugar de std::vector
    MyVector<int> temp;        // Empieza vacío
    while (currentId != -1) {
        // Aumentar tamaño de temp en 1 y añadir el valor
        int oldSize = temp.size();
        temp.resize(oldSize + 1, 0);
        temp[oldSize] = currentId;

        currentId = cameFrom[currentId];
    }

    // Convertimos en orden inverso para usar push_back
    SimpleList<int> totalPath;
    for (int i = temp.size() - 1; i >= 0; --i) {
        totalPath.push_back(temp[i]);
    }

    return totalPath;
}
