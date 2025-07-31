#pragma once
#include "Estructuras_datos/MyVector.h"
#include "Estructuras_datos/pair.h"
#include "Node.h"
#include "Obstacle.h" 
#include <utility>
#include "raylib-cpp.hpp" 

// Clase que representa el grafo
class Graph {
public:
    Graph();
    ~Graph();
    void addNode(int id, float x, float y);
    bool addEdge(int sourceId, int targetId, float weight);
    const Node& getNode(int id) const;
    const MyVector<Pair<int, float>>& getAdjacentNodes(int id) const;
    int getNumNodes() const;
    int findNodeAtPosition(const raylib::Vector2& clickPos, float radius) const;
    void generateRandomNodes(int count, int maxWidth, int maxHeight, int maxEdgesPerNode, float maxConnectionDistance);

    
    void generateRandomObstacles(int count, int maxWidth, int maxHeight);
    const MyVector<Obstacle>& getObstacles() const; //salida de los obstaculos

    void clear();
    void addObstacle(const Obstacle& obs);
    void removeObstacle(int index); 
    bool areNodesWithinObstacle(int nodeId1, int nodeId2) const;
    size_t getEstimatedMemoryUsage() const;
                                                                

private:
    MyVector<Node> nodes;
    MyVector<MyVector<Pair<int, float>>> adjacencyList;
    bool isValidNodeId(int id) const;
    MyVector<Obstacle> obstacles;

    MyVector<MyVector<MyVector<int>>> spatialGrid;
    int gridCols;
    int gridRows;

    static const int CellSize = 200; // ajustar valor si quieren

    //Metodos axucialres de la cuadricula 
    int getCellX(float x) const { return static_cast<int>(x / CellSize); }
    int getCellY(float y) const { return static_cast<int>(y / CellSize); }
    bool isValidGridCell(int col, int row) const {
        return col >= 0 && col < gridCols && row >= 0 && row < gridRows;
    }
};