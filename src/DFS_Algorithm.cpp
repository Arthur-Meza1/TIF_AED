#include "SearchAlgorithm.hpp" 
#include "MyVector.hpp" 
#include "SimpleMap.hpp"
#include "Utils.hpp"
#include "Graph.hpp"
#include <limits>

SimpleList<int> DFS_Algorithm::findPath(const Graph& graph, int startNodeId, int endNodeId) const {
    if (startNodeId < 0 || startNodeId >= graph.getNumNodes() || endNodeId < 0 || endNodeId >= graph.getNumNodes()) {
        return {};
    }

    MyVector<int> stack;
    SimpleMap<int, int> cameFrom;
    SimpleMap<int, bool> visited;

    stack.push_back(startNodeId);
    visited.set(startNodeId, true);

    while (!stack.empty()) {
        int currentId = stack.back();
        stack.pop_back();

        if (currentId == endNodeId) {
            return reconstructPath(cameFrom, currentId);
        }

        const auto& neighbors = graph.getAdjacentNodes(currentId);
        // Recorremos los vecinos en orden inverso para que el primer vecino
        // sea el último en entrar en la pila, y por lo tanto, el primero en salir.
        for (int i = neighbors.size() - 1; i >= 0; --i) {
            int neighborId = neighbors[i].first;

            if (!visited.contains(neighborId)) {
                // Chequeo de colisión con obstáculos
                const raylib::Vector2 currentPos = graph.getNode(currentId).position;
                const raylib::Vector2 neighborPos = graph.getNode(neighborId).position;

                bool isBlocked = false;
                for (const auto& obstacle : graph.getObstacles()) {
                    if (Utils::CheckLineRectangleCollision(currentPos, neighborPos, obstacle.rect)) {
                        isBlocked = true;
                        break;
                    }
                }
                if (isBlocked) continue;

                visited.set(neighborId, true);
                cameFrom.set(neighborId, currentId);
                stack.push_back(neighborId);
            }
        }
    }

    return {}; // Camino no encontrado
}

SimpleList<int> DFS_Algorithm::reconstructPath(const SimpleMap<int, int>& cameFrom, int currentId) const {
    MyVector<int> temp;
    
    // Reconstruimos la ruta en orden inverso
    while (cameFrom.contains(currentId)) {
        temp.push_back(currentId);
        currentId = cameFrom.get(currentId);
    }
    temp.push_back(currentId);

    // Llenamos la SimpleList en el orden correcto
    SimpleList<int> totalPath;
    for (int i = temp.size() - 1; i >= 0; --i) {
        totalPath.push_back(temp[i]);
    }
    return totalPath;
}