#include "Graph.h"
#include "Estructuras_datos/MyVector.h"
#include "Estructuras_datos/pair.h"
#include <string>
#include <iostream>  
#include <algorithm> 
#include <limits>    

Graph::Graph() {
    // Constructor: Inicializa las listas vacías
}

// Añade un nodo al grafo. Asume que los IDs se añadirán secuencialmente.
void Graph::addNode(int id, float x, float y) {
    // Si el ID es mayor o igual al tamaño actual, redimensiona los vectores
    if (id >= nodes.size()) {
        nodes.resize(id + 1);
        adjacencyList.resize(id + 1);
    }
    nodes[id] = Node(id, x, y);
}

// Añade una arista (conexión) entre dos nodos con un peso dado
bool Graph::addEdge(int sourceId, int targetId, float weight) {
    if (!isValidNodeId(sourceId) || !isValidNodeId(targetId)) {
        return false;
    }
    if (sourceId == targetId) { 
        return false;
    }

    for(const auto& edge: adjacencyList[sourceId]){
        if(edge.first == targetId){
            return false;
        }
    }
    // Añadir la arista de source a target
    adjacencyList[sourceId].push_back({targetId, weight});
    adjacencyList[targetId].push_back({sourceId, weight});
    

    return true;
}

// Obtiene una referencia constante a un nodo por su ID
const Node& Graph::getNode(int id) const {
    if (!isValidNodeId(id)) {
        // Esto podría lanzar una excepción o retornar un nodo "nulo"
        // Para simplicidad, se asume que siempre se pedirá un ID válido después de isValidNodeId
        // o que el caller maneja el caso.
        // Podrías lanzar: throw std::out_of_range("Node ID out of range");
        std::cerr << "Error: Acceso a nodo con ID fuera de rango: " << id << std::endl;
        // Retornar un nodo por defecto para evitar un crash inmediato, pero es un error lógico.
        static Node defaultNode(-1, -1.0f, -1.0f); // Nodo estático por defecto
        return defaultNode;
    }
    return nodes[id];
}

// Obtiene la lista de nodos adyacentes a un nodo dado
const MyVector<Pair<int, float>>& Graph::getAdjacentNodes(int id) const {
    if (!isValidNodeId(id)) {
        // std::cerr << "Error: ID de nodo invalido al obtener adyacentes: " << id << std::endl;
        static const MyVector<Pair<int, float>> emptyList; // Lista vacía para IDs inválidos
        return emptyList;
    }
    return adjacencyList[id];
}

// Retorna el número total de nodos
int Graph::getNumNodes() const {
    return nodes.size();
}

// Encuentra el ID del nodo más cercano a una posición dada dentro de un radio de clic
int Graph::findNodeAtPosition(const raylib::Vector2& clickPos, float radius) const {
    float radiusSq = radius * radius; // Comparar con distancia al cuadrado para evitar sqrt
    for (const auto& node : nodes) {
        // Asegúrate de que el nodo sea válido (no sea un nodo por defecto con ID -1)
        if (node.id != -1 && clickPos.DistanceSqr(node.position) <= radiusSq) {
            return node.id;
        }
    }
    return -1; // No se encontró ningún nodo en la posición
}

// Verifica si un ID de nodo es válido
bool Graph::isValidNodeId(int id) const {
    return id >= 0 && id < nodes.size();
}

// Genera un grafo aleatorio con nodos y conexiones
void Graph::generateRandomNodes(int count, int maxWidth, int maxHeight, int maxEdgesPerNode, float maxConnectionDistance) {
    clear(); // Limpia cualquier grafo existente
    nodes.reserve(count); // Reserva espacio para los nodos
    adjacencyList.resize(count); // Redimensiona la lista de adyacencia

    // 1. Generar Nodos con posiciones aleatorias
    for (int i = 0; i < count; ++i) {
        // Genera posiciones dentro de los límites de la pantalla/área definida
        float x = (float)GetRandomValue(0, maxWidth);
        float y = (float)GetRandomValue(0, maxHeight);

        nodes.emplace_back(i,x,y);
    }

    // 2. Conectar Nodos
    // Una estrategia simple: Conectar cada nodo a algunos vecinos aleatorios dentro de un radio.
    for (int i = 0; i < count; ++i) {
        raylib::Vector2 p1 = nodes[i].position;
        int connectionsMade = 0;

        // Intentar conectar con otros nodos
        for (int j = 0; j < count && connectionsMade < maxEdgesPerNode; ++j) {
            if (i == j) continue; // No conectar a sí mismo

            raylib::Vector2 p2 = nodes[j].position;
            float distance = p1.Distance(p2); // Distancia euclidiana

            if (distance < maxConnectionDistance) { // Conectar solo si están lo suficientemente cerca
                if (addEdge(i, j, distance)) { // addEdge ya maneja duplicados
                    connectionsMade++;
                }
            }
        }
        // Si no se hicieron suficientes conexiones cercanas, intentar con nodos aleatorios
        while (connectionsMade < maxEdgesPerNode) {
            int targetNodeId = GetRandomValue(0, count - 1);
            if (targetNodeId == i) continue;

            raylib::Vector2 p2 = nodes[targetNodeId].position;
            float distance = p1.Distance(p2);
            if (addEdge(i, targetNodeId, distance)) {
                connectionsMade++;
            }
        }
    }
    std::cout << "Grafo generado y conectado con " << nodes.size() << " nodos." << std::endl;



}
// --- FUNCIONES PARA OBSTÁCULOS (ahora usando Obstacle struct) ---
void Graph::generateRandomObstacles(int count, int maxWidth, int maxHeight) {
    obstacles.clear();
    obstacles.reserve(count);

    std::cout << "Generando " << count << " obstaculos aleatorios..." << std::endl;
    // Nombres predefinidos para los obstáculos aleatorios
    static const std::string obstacleNames_array[] = {
    "Rio", "Pared", "Lago", "Montana", "Barranco", "Zona Prohibida", "Bosque Denso"
    };
    MyVector<std::string> obstacleNames(my_initializer_list<std::string>(
        obstacleNames_array, sizeof(obstacleNames_array) / sizeof(std::string)));

    static const Color obstacleColors_array[] = {
        BLUE, DARKGREEN, BROWN, GRAY, MAROON, VIOLET, LIME
    };
    MyVector<Color> obstacleColors(my_initializer_list<Color>(
        obstacleColors_array, sizeof(obstacleColors_array) / sizeof(Color)));

    for (int i = 0; i < count; ++i) {
        int minSize = 30;
        int maxSize = 150;
        int width = GetRandomValue(minSize, maxSize);
        int height = GetRandomValue(minSize, maxSize);

        int x = GetRandomValue(0, maxWidth - width);
        int y = GetRandomValue(0, maxHeight - height);

        Rectangle rect = {(float)x, (float)y, (float)width, (float)height};
        std::string name = obstacleNames[GetRandomValue(0, obstacleNames.size() - 1)];
        Color color = obstacleColors[GetRandomValue(0, obstacleColors.size() - 1)];

        // Usamos emplace_back para construir el objeto Obstacle directamente en el vector
        obstacles.emplace_back(rect, name, color);
    }
    std::cout << "Obstaculos aleatorios generados." << std::endl;
}

const MyVector<Obstacle>& Graph::getObstacles() const {
    return obstacles;
}

void Graph::addObstacle(const Obstacle& obs) {
    obstacles.push_back(obs);
}

void Graph::removeObstacle(int index) {
    if (index >= 0 && index < obstacles.size()) {
        obstacles.erase(obstacles.begin() + index);
    }
}

void Graph::clear() {
    nodes.clear();
    adjacencyList.clear();
}
