#include "Pathfinding.h"
#include <algorithm>
#include "raylib.h"     // Necessary for Rectangle, Vector2, GetRandomValue, etc.
#include "raymath.h"    // Explicitly include for CheckCollisionLineRec (good practice)
#include <limits>       // For std::numeric_limits

// Constructor del Pathfinding, recibe una referencia constante al grafo
Pathfinding::Pathfinding(const Graph& g) : graph(g) {
    // Inicializa los vectores de costos y padres con tamaños apropiados
    gScore.resize(graph.getNumNodes(), std::numeric_limits<float>::infinity());
    fScore.resize(graph.getNumNodes(), std::numeric_limits<float>::infinity());
    cameFrom.resize(graph.getNumNodes(), -1); // -1 indica sin padre
    closedSet.resize(graph.getNumNodes(), false);
}

// Calcula la distancia euclidiana entre dos nodos (heurística)
float Pathfinding::calculateHeuristic(int nodeId1, int nodeId2) const {
    const Node& n1 = graph.getNode(nodeId1);
    const Node& n2 = graph.getNode(nodeId2);
    // Vector2Distance es una función de raymath.h, compatible con raylib-cpp
    return Vector2Distance(n1.position, n2.position);
}
bool CheckLineRectangleCollisionCustom(raylib::Vector2 p1, raylib::Vector2 p2, raylib::Rectangle rec) {
    // Comprueba si alguno de los puntos finales de la línea está dentro del rectángulo
    if (CheckCollisionPointRec(p1, rec) || CheckCollisionPointRec(p2, rec)) {
        return true;
    }

    // Comprueba si la línea interseca con cualquiera de los cuatro bordes del rectángulo
    // Borde 1: Borde superior (rec.x, rec.y) a (rec.x + rec.width, rec.y)
    if (CheckCollisionLines(p1, p2, {rec.x, rec.y}, {rec.x + rec.width, rec.y}, nullptr)) return true;
    // Borde 2: Borde inferior (rec.x, rec.y + rec.height) a (rec.x + rec.width, rec.y + rec.height)
    if (CheckCollisionLines(p1, p2, {rec.x, rec.y + rec.height}, {rec.x + rec.width, rec.y + rec.height}, nullptr)) return true;
    // Borde 3: Borde izquierdo (rec.x, rec.y) a (rec.x, rec.y + rec.height)
    if (CheckCollisionLines(p1, p2, {rec.x, rec.y}, {rec.x, rec.y + rec.height}, nullptr)) return true;
    // Borde 4: Borde derecho (rec.x + rec.width, rec.y) a (rec.x + rec.width, rec.y + rec.height)
    if (CheckCollisionLines(p1, p2, {rec.x + rec.width, rec.y}, {rec.x + rec.width, rec.y + rec.height}, nullptr)) return true;

    return false;
}
// Implementación del algoritmo A*
std::list<int> Pathfinding::findPath(int startNodeId, int endNodeId) {
    // Validar IDs de nodos
    if (startNodeId < 0 || startNodeId >= graph.getNumNodes() ||
        endNodeId < 0 || endNodeId >= graph.getNumNodes()) {
        // std::cerr << "Error: ID de nodo de inicio/fin invalido." << std::endl;
        return {}; // Retorna lista vacía si los IDs son inválidos
    }

    // Reiniciar estructuras para una nueva búsqueda
    std::fill(gScore.begin(), gScore.end(), std::numeric_limits<float>::infinity());
    std::fill(fScore.begin(), fScore.end(), std::numeric_limits<float>::infinity());
    std::fill(cameFrom.begin(), cameFrom.end(), -1);
    std::fill(closedSet.begin(), closedSet.end(), false);

    std::priority_queue<std::pair<float, int>, std::vector<std::pair<float, int>>, std::greater<std::pair<float, int>>> openSet;

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

        // Get the current node's position ONCE per current node to avoid recalculation
        const raylib::Vector2 currentPos = graph.getNode(currentId).position;

        const auto& neighbors = graph.getAdjacentNodes(currentId); // Corrected: iterate over graph.getAdjacentNodes()

        for (const auto& neighborPair : neighbors) {
            int neighborId = neighborPair.first;
            float edgeCost = neighborPair.second;

            if (closedSet[neighborId]) {
                continue;
            }

            // --- VERIFICACIÓN DE OBSTÁCULOS (ahora usando obstacle.rect) ---
            const raylib::Vector2 neighborPos = graph.getNode(neighborId).position;
            bool isBlocked = false;
            for (const auto& obstacle : graph.getObstacles()) {
                // ¡¡¡CORRECCIÓN CLAVE AQUÍ: USA ::CheckCollisionLineRec O SOLO CheckCollisionLineRec!!!
                // NO uses raylib::CheckCollisionLineRec
                if (CheckLineRectangleCollisionCustom(currentPos, neighborPos, obstacle.rect)) {
                    isBlocked = true;
                     break;
                }
            }
            if (isBlocked) {
                continue;
            }
            // --- FIN DE LA VERIFICACIÓN DE OBSTÁCULOS ---

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

    return {}; // Ruta no encontrada
}

// Reconstruye el camino desde el nodo final usando el mapa cameFrom
// FIX: The 'const' keyword must match the declaration in the header.
std::list<int> Pathfinding::reconstructPath(int currentId) const {
    std::list<int> totalPath;
    while (currentId != -1) {
        totalPath.push_front(currentId);
        currentId = cameFrom[currentId];
    }
    return totalPath;
}