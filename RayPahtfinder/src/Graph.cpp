#include "Graph.h"
#include "Utils.h" // Para usar GetDistance()
#include <random>    // Para generar números aleatorios (posiciones de nodos)
#include <limits>    // Para std::numeric_limits

// Constructor por defecto
Graph::Graph() {}

// Implementación de la función para verificar si una conexión está bloqueada.
bool Graph::isConnectionBlocked(int id1, int id2) const {
    for (const auto& river : m_rivers) { // Itera sobre todos los "ríos" definidos
        if (river.connects(id1, id2)) { // Usa el método 'connects' de RiverConnection
            return true; // La conexión está bloqueada
        }
    }
    return false; // La conexión no está bloqueada
}

// Implementación de la función para generar nodos y sus conexiones.
void Graph::generateNodesAndConnections(int numNodes, float maxConnectionDistance, const std::vector<RiverConnection>& rivers) {
    m_nodes.clear();         // Limpia nodos anteriores
    m_adjacencyList.clear(); // Limpia conexiones anteriores
    m_rivers = rivers;       // Copia la lista de "ríos" proporcionada

    // Configuración para la generación de números aleatorios para las posiciones de los nodos
    std::random_device rd;   // Semilla aleatoria
    std::mt19937 gen(rd());  // Generador de números pseudoaleatorios
    // Distribuciones para X e Y dentro de un rango (ej. 50 a 1280-50 para X, 50 a 720-50 para Y)
    // Esto asegura que los nodos no se generen pegados a los bordes de la ventana.
    std::uniform_real_distribution<> distribX(50, 1280 - 50);
    std::uniform_real_distribution<> distribY(50, 720 - 50);

    // 1. Generar los nodos
    for (int i = 0; i < numNodes; ++i) {
        // Crea un nuevo Node con ID 'i' y posición aleatoria
        m_nodes.emplace_back(i, static_cast<float>(distribX(gen)), static_cast<float>(distribY(gen)));
    }

    // 2. Redimensionar la lista de adyacencia para que tenga espacio para todos los nodos
    m_adjacencyList.resize(numNodes);

    // 3. Crear las conexiones (aristas) entre los nodos
    // Itera sobre todos los pares posibles de nodos
    for (int i = 0; i < numNodes; ++i) {
        for (int j = i + 1; j < numNodes; ++j) { // 'j' empieza en 'i+1' para evitar duplicados y conexiones de un nodo consigo mismo
            // Calcula la distancia euclidiana entre los nodos i y j
            float dist = GetDistance(m_nodes[i].position, m_nodes[j].position);

            // Si la distancia es menor o igual a la distancia máxima permitida para una conexión
            if (dist <= maxConnectionDistance) {
                // ¡Punto clave! Verifica si esta posible conexión está bloqueada por un "río"
                if (!isConnectionBlocked(m_nodes[i].id, m_nodes[j].id)) {
                    // Si no está bloqueada, añade la arista (conexión) a la lista de adyacencia
                    // Se añade en ambos sentidos porque el grafo es no dirigido (se puede ir de A a B y de B a A)
                    m_adjacencyList[i].push_back({j, dist}); // Conexión de i a j con peso 'dist'
                    m_adjacencyList[j].push_back({i, dist}); // Conexión de j a i con peso 'dist'
                }
            }
        }
    }
}

// Retorna los vecinos de un nodo dado su ID
const std::vector<std::pair<int, float>>& Graph::getNeighbors(int nodeId) const {
    // .at() proporciona comprobación de límites, lanzando una excepción si el ID es inválido
    return m_adjacencyList.at(nodeId);
}

// Retorna la información de un nodo dado su ID
const Node& Graph::getNode(int nodeId) const {
    return m_nodes.at(nodeId);
}

// Encuentra el nodo más cercano a una posición dada
int Graph::getNearestNodeId(raylib::Vector2 position) const {
    if (m_nodes.empty()) return -1; // Si no hay nodos, no hay uno cercano

    int nearestId = -1;
    float minDistanceSq = std::numeric_limits<float>::max(); // Usamos distancia al cuadrado para eficiencia (evita sqrt)

    for (const auto& node : m_nodes) {
        // Calcula la distancia al cuadrado entre la posición dada y la posición del nodo actual
        float distSq = position.DistanceSqr(node.position);
        if (distSq < minDistanceSq) {
            minDistanceSq = distSq; // Actualiza la distancia mínima
            nearestId = node.id;    // Actualiza el ID del nodo más cercano
        }
    }
    // Opcional: Si el clic del mouse está demasiado lejos de cualquier nodo,
    // se considera que no se seleccionó ningún nodo.
    if (minDistanceSq > 50 * 50) return -1; // Si la distancia al cuadrado es mayor que (50*50) = 2500, ignora el clic.
    return nearestId;
}