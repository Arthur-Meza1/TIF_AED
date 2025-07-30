#pragma once

#include "Estructuras_datos/MyVector.h"
#include "Estructuras_datos/list.h"
#include "Node.h"
#include "Graph.h"
#include "Estructuras_datos/queue.h" // Asegúrate de que tu PriorityQueue esté aquí
#include "Estructuras_datos/pair.h"


bool CheckLineRectangleCollisionCustom(raylib::Vector2 p1, raylib::Vector2 p2, raylib::Rectangle rec);


class Dijkstra {
public:
    // Constructor, recibe una referencia constante al grafo
    Dijkstra(const Graph& g);

    // Función principal para encontrar el camino usando Dijkstra
    SimpleList<int> findPath(int startNodeId, int endNodeId);

private:
    const Graph& graph;

    // Vectores para almacenar los datos del algoritmo
    MyVector<float> gScore;      // Camino mas corto a cada nodo
    MyVector<int> cameFrom;      // ID del nodo del camino mas comodo
    MyVector<bool> closedSet;    // Para saber si un nodo ya ha sido evaluado/visitado

    // Función auxiliar para reconstruir el camino una vez que se encuentra el nodo final
    SimpleList<int> reconstructPath(int currentId) const;
};