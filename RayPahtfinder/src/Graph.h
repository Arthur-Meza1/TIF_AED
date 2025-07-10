#pragma once

#include "Node.h"
#include "Obstacle.h" // Incluir el nuevo archivo Obstacle.h
#include <vector>
#include <list>
#include <utility>
#include "raylib.h" // Para Rectangle y CheckCollisionLineRec

// Clase que representa el grafo
class Graph {
public:
    Graph();
    void addNode(int id, float x, float y);
    bool addEdge(int sourceId, int targetId, float weight);
    const Node& getNode(int id) const;
    const std::vector<std::pair<int, float>>& getAdjacentNodes(int id) const;
    int getNumNodes() const;
    int findNodeAtPosition(const raylib::Vector2& clickPos, float radius) const;
    void generateRandomNodes(int count, int maxWidth, int maxHeight, int maxEdgesPerNode, float maxConnectionDistance);
    void clear();

    // --- MIEMBROS PARA OBSTÁCULOS (ahora con la nueva estructura Obstacle) ---
    void generateRandomObstacles(int count, int maxWidth, int maxHeight);
    const std::vector<Obstacle>& getObstacles() const; // Getter para los obstáculos

    // --- NUEVAS FUNCIONES PARA AÑADIR OBSTÁCULOS MANUALMENTE ---
    void addObstacle(const Obstacle& obs);
    void removeObstacle(int index); // Para eliminar por índice, si fuera necesario
    bool areNodesWithinObstacle(int nodeId1, int nodeId2) const; // Verifica si ambos nodos están dentro de un mismo obstáculo
                                                                // Útil para evitar aristas entre nodos si ambos ya están bloqueados

private:
    std::vector<Node> nodes;
    std::vector<std::vector<std::pair<int, float>>> adjacencyList;
    std::vector<std::vector<bool>> connected;
    bool isValidNodeId(int id) const;

    // --- ALMACENAMIENTO DE OBSTÁCULOS (ahora de tipo Obstacle) ---
    std::vector<Obstacle> obstacles;
};