#pragma once
#include "MyVector.h"
#include "Node.h"
#include "Obstacle.h" 
#include <vector>
#include <list>
#include <utility>
#include "raylib.h" 

// Clase que representa el grafo
class Graph {
public:
    Graph();
    void addNode(int id, float x, float y);
    bool addEdge(int sourceId, int targetId, float weight);
    const Node& getNode(int id) const;
    const MyVector<std::pair<int, float>>& getAdjacentNodes(int id) const;
    int getNumNodes() const;
    int findNodeAtPosition(const raylib::Vector2& clickPos, float radius) const;
    void generateRandomNodes(int count, int maxWidth, int maxHeight, int maxEdgesPerNode, float maxConnectionDistance);
    void clear();

    
    void generateRandomObstacles(int count, int maxWidth, int maxHeight);
    const MyVector<Obstacle>& getObstacles() const; //salida de los obstaculos

    
    void addObstacle(const Obstacle& obs);
    void removeObstacle(int index); 
    bool areNodesWithinObstacle(int nodeId1, int nodeId2) const; 
                                                                

private:
    MyVector<Node> nodes;
    MyVector<MyVector<std::pair<int, float>>> adjacencyList;
    bool isValidNodeId(int id) const;
    MyVector<Obstacle> obstacles;
};