#include "Graph.hpp"
#include "MyVector.hpp"
#include "pair.hpp"
#include <algorithm>
#include <iostream>
#include <limits>
#include <string>

Graph::Graph() : gridCols(0), gridRows(0) {}

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

  for (const auto& edge : adjacencyList[sourceId]) {
    if (edge.first == targetId) {
      return false;
    }
  }

  for (size_t i = 0; i < adjacencyList[sourceId].size(); ++i) {
    if (adjacencyList[sourceId][i].first == targetId) {
      return false; // La arista ya existe, no se añade de nuevo
    }
  }

  // Añadir la arista de source a target
  adjacencyList[sourceId].push_back(Pair<int, float>(targetId, weight));

  adjacencyList[targetId].push_back(Pair<int, float>(sourceId, weight));

  return true;
}

// Obtiene una referencia constante a un nodo por su ID
const Node& Graph::getNode(int id) const {
  if (!isValidNodeId(id)) {
    std::cerr << "Error: Acceso a nodo con ID fuera de rango: " << id
              << std::endl;
    // Retornar un nodo por defecto para evitar un crash inmediato, pero es un
    // error lógico.
    static Node defaultNode(-1, -1.0f, -1.0f);
    return defaultNode;
  }
  return nodes[id];
}

// Obtiene la lista de nodos adyacentes a un nodo dado
const MyVector<Pair<int, float>>& Graph::getAdjacentNodes(int id) const {
  if (!isValidNodeId(id)) {
    // std::cerr << "Error: ID de nodo invalido al obtener adyacentes: " << id
    // << std::endl;
    static const MyVector<Pair<int, float>>
        emptyList; // Lista vacía para IDs inválidos
    return emptyList;
  }
  return adjacencyList[id];
}

// Retorna el número total de nodos
int Graph::getNumNodes() const {
  return nodes.size();
}

// Encuentra el ID del nodo más cercano a una posición dada dentro de un radio
// de clic
int Graph::findNodeAtPosition(const raylib::Vector2& clickPos,
                              float radius) const {
  float radiusSq =
      radius * radius; // Comparar con distancia al cuadrado para evitar sqrt
  for (const auto& node : nodes) {
    // Asegúrate de que el nodo sea válido (no sea un nodo por defecto con ID
    // -1)
    if (node.id != -1 && clickPos.DistanceSqr(node.position) <= radiusSq) {
      return node.id;
    }
  }
  return -1;
}

// Verifica si un ID de nodo es válido
bool Graph::isValidNodeId(int id) const {
  return id >= 0 && id < nodes.size();
}

//-------implementacion optimizada usando cuadricula espacial---------
void Graph::generateRandomNodes(int count, int maxWidth, int maxHeight,
                                int maxEdgesPerNode,
                                float maxConnectionDistance) {
  clear();
  nodes.reserve(count);
  adjacencyList.resize(count);

  gridCols = maxWidth / CellSize + 1;
  gridRows = maxHeight / CellSize + 1;

  // Preparar la grilla
  spatialGrid.resize(gridRows);
  for (int r = 0; r < gridRows; ++r) {
    spatialGrid[r].resize(gridCols);
    for (int c = 0; c < gridCols; ++c) {
      spatialGrid[r][c].reserve(
          10); // Reservar para evitar muchas reasignaciones
    }
  }

  // 1. Generar nodos y distribuir en spatialGrid
  for (int i = 0; i < count; ++i) {
    float x = (float)GetRandomValue(0, maxWidth);
    float y = (float)GetRandomValue(0, maxHeight);

    nodes.emplace_back(i, x, y);

    int cellX = getCellX(x);
    int cellY = getCellY(y);
    if (isValidGridCell(cellX, cellY))
      spatialGrid[cellY][cellX].push_back(i);
  }

  // 2. Conectar nodos evitando duplicados y exceso de aristas
  for (int i = 0; i < count; ++i) {
    const Node& currentNode = nodes[i];
    int currentCellX = getCellX(currentNode.position.x);
    int currentCellY = getCellY(currentNode.position.y);

    int edgesConnected = 0;

    for (int dy = -1; dy <= 1 && edgesConnected < maxEdgesPerNode; ++dy) {
      for (int dx = -1; dx <= 1 && edgesConnected < maxEdgesPerNode; ++dx) {
        int neighborCellX = currentCellX + dx;
        int neighborCellY = currentCellY + dy;

        if (!isValidGridCell(neighborCellX, neighborCellY))
          continue;

        const MyVector<int>& neighborNodes =
            spatialGrid[neighborCellY][neighborCellX];
        for (size_t k = 0;
             k < neighborNodes.size() && edgesConnected < maxEdgesPerNode;
             ++k) {
          int neighborId = neighborNodes[k];

          // Evitar conexiones duplicadas
          if (neighborId <= i)
            continue;

          const Node& neighborNode = nodes[neighborId];
          float dist = currentNode.position.Distance(neighborNode.position);

          if (dist <= maxConnectionDistance) {
            if (addEdge(i, neighborId, dist)) {
              edgesConnected++;
            }
          }
        }
      }
    }
  }

  std::cout << "Grafo generado y conectado con " << nodes.size() << " nodos."
            << std::endl;
}
// --- FUNCIONES PARA OBSTÁCULOS (ahora usando Obstacle struct) ---
void Graph::generateRandomObstacles(int count, int maxWidth, int maxHeight) {
  obstacles.clear();
  obstacles.reserve(count);

  std::cout << "Generando " << count << " obstaculos aleatorios..."
            << std::endl;
  // Nombres predefinidos para los obstáculos aleatorios
  static const std::string obstacleNames_array[] = {
      "Rio",      "Pared",          "Lago",        "Montana",
      "Barranco", "Zona Prohibida", "Bosque Denso"};
  MyVector<std::string> obstacleNames(my_initializer_list<std::string>(
      obstacleNames_array, sizeof(obstacleNames_array) / sizeof(std::string)));

  static const Color obstacleColors_array[] = {BLUE,   DARKGREEN, BROWN, GRAY,
                                               MAROON, VIOLET,    LIME};
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
    std::string name =
        obstacleNames[GetRandomValue(0, obstacleNames.size() - 1)];
    Color color = obstacleColors[GetRandomValue(0, obstacleColors.size() - 1)];

    // Usamos emplace_back para construir el objeto Obstacle directamente en el
    // vector
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

  for (int r = 0; r < gridRows; ++r) {
    for (int c = 0; c < gridCols; ++c) {
      if (r < spatialGrid.size() && c < spatialGrid[r].size()) {
        spatialGrid[r][c].clear();
      }
    }
  }
  spatialGrid.clear();
  gridCols = 0;
  gridRows = 0;
}
size_t Graph::getEstimatedMemoryUsage() const {
  size_t total_memory = 0;

  total_memory += nodes.capacity() * sizeof(Node);

  total_memory += adjacencyList.capacity() * sizeof(MyVector<Pair<int, float>>);
  for (size_t i = 0; i < adjacencyList.size(); ++i) {
    total_memory += adjacencyList[i].capacity() * sizeof(Pair<int, float>);
  }

  total_memory += obstacles.capacity() * sizeof(Obstacle);

  total_memory += spatialGrid.capacity() * sizeof(MyVector<MyVector<int>>);
  for (size_t r = 0; r < spatialGrid.size(); ++r) {
    total_memory += spatialGrid[r].capacity() * sizeof(MyVector<int>);
    for (size_t c = 0; c < spatialGrid[r].size(); ++c) {
      total_memory += spatialGrid[r][c].capacity() * sizeof(int);
    }
  }

  return total_memory;
}

Graph::~Graph() {}
