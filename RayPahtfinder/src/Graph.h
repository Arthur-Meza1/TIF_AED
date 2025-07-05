#pragma once
#include "Node.h"          // Necesita la definición de Node
#include "Obstacle.h"      // Necesita la definición de RiverConnection
#include <vector>          // Para almacenar nodos y la lista de adyacencia
#include <list>            // Aunque no directamente usada en Graph, útil para la ruta final

// Definición de la clase Graph
class Graph {
public:
    Graph(); // Constructor por defecto

    // Genera los nodos y sus conexiones.
    // numNodes: cantidad de nodos a generar.
    // maxConnectionDistance: distancia máxima para considerar dos nodos conectados.
    // rivers: una lista de RiverConnection que no deben conectarse.
    void generateNodesAndConnections(int numNodes, float maxConnectionDistance, const std::vector<RiverConnection>& rivers);

    // Obtiene los nodos vecinos de un nodo dado su ID.
    // Retorna un vector de pares {ID_vecino, peso_de_la_arista}.
    const std::vector<std::pair<int, float>>& getNeighbors(int nodeId) const;

    // Obtiene la información completa de un nodo dado su ID.
    const Node& getNode(int nodeId) const;

    // Retorna la cantidad total de nodos en el grafo.
    int getNumNodes() const { return m_nodes.size(); }

    // Encuentra el ID del nodo más cercano a una posición específica.
    // Útil para cuando el usuario hace clic en el mapa.
    int getNearestNodeId(raylib::Vector2 position) const;

    // Retorna una referencia constante al vector de todos los nodos.
    const std::vector<Node>& getAllNodes() const { return m_nodes; }

    // Retorna una referencia constante a la lista de conexiones de río (obstáculos).
    const std::vector<RiverConnection>& getRiverConnections() const { return m_rivers; }

private:
    std::vector<Node> m_nodes; // Almacena todos los nodos del grafo

    // Lista de adyacencia:
    // Un vector donde cada índice 'i' representa el Node con ID 'i'.
    // El valor es otro vector de pares {ID_vecino, peso_de_la_arista}.
    std::vector<std::vector<std::pair<int, float>>> m_adjacencyList;

    std::vector<RiverConnection> m_rivers; // Almacena las conexiones de río (obstáculos)

    // Función auxiliar para verificar si una conexión entre dos IDs está bloqueada por un río.
    bool isConnectionBlocked(int id1, int id2) const;
};