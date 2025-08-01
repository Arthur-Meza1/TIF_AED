#include "Graph.hpp"
#include "Node.hpp"
#include "Obstacle.hpp"
#include "Pathfinding.hpp"
#include "list.hpp"
#include "raylib-cpp.hpp"
#include "SearchAlgorithm.hpp"
#include "SimpleMap.hpp"
#include "Utils.hpp"
#include <algorithm>
#include <chrono>
#include <memory>
#include <cstring>
#include <iostream>
#include <limits>
#include <chrono>
#include <string>

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 800;
const int GRAPHICAL_NODE_LIMIT = 700; 
const float NODE_GENERATION_AREA_PADDING = 50.0f;
const int MAX_EDGES_PER_NODE_GENERATION = 4;
const float MAX_CONNECTION_DISTANCE_GENERATION =200.0f; 
const int NUM_OBSTACLES = 5;

Graph myGraph;
int startNodeId = -1;
int endNodeId = -1;
SimpleList<int> path;
std::unique_ptr<Pathfinding> myPathfinding;
SimpleMap<std::string, std::unique_ptr<SearchAlgorithm>> algorithms;
const SearchAlgorithm* currentAlgorithm = nullptr;

raylib::Camera2D camera;

bool graphicalMode = true;
// --- Variables Globales Adicionales para Modo Gráfico (Edición de Obstáculos)// ---
bool editingObstacles = false;
raylib::Vector2 startDragPos;
bool isDragging = false;
char obstacleNameInput[64] = "";
bool typingObstacleName = false;
Rectangle nameInputBox = {10, (float)SCREEN_HEIGHT - 40, 200, 30};

// --- Declaraciones de Funciones Auxiliares ---
void InitializeApplication(int numNodes);
void UpdateApplication(float deltaTime);
void DrawApplication();
void CleanupApplication();

// Funciones específicas del modo gráfico
void HandleGraphicalInput();
void DrawGraphicalHUD();
void DrawGraphElements(const Graph& graph, const SimpleList<int>& pathNodes,
                       int startNodeId, int endNodeId);

// Funciones específicas del modo terminal
void RunTerminalMode();
void PrintPathToTerminal(const SimpleList<int>& p);
int GetValidNodeIdFromUser(const std::string& prompt, int maxId);

// --- Implementación de main ---
int main() {
  int numNodes;
  std::cout << "------------------------------------------" << std::endl;
  std::cout << "        RECORRIDOS DE GRAFOS              " << std::endl;
  std::cout << "------------------------------------------" << std::endl;
  std::cout << "Ingrese la cantidad de nodos (se recomienda <= "
            << GRAPHICAL_NODE_LIMIT << " para interfaz grafica): ";
  std::cin >> numNodes;

  // Validación de entrada para numNodes
  if (std::cin.fail() || numNodes <= 0) {
    std::cerr << "Error: Entrada invalida. Por favor, ingrese un numero entero "
                 "positivo."
              << std::endl;
    return 1; 
  }
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');


  algorithms["A*"] = std::make_unique<AStar_Algorithm>();
  algorithms["BFS"] = std::make_unique<BFS_Algorithm>();
  algorithms["BestFirst-Search"] = std::make_unique<BestFirst_Algorithm>();
  algorithms["DFS"] = std::make_unique<DFS_Algorithm>();
  algorithms["Dijkstra"] = std::make_unique<Dijkstra_Algorithm>();


  currentAlgorithm = algorithms["A*"].get();

  InitializeApplication(numNodes);




  // 3. Bucle principal basado en el modo de ejecución
  if (graphicalMode) {
    // Modo Gráfico: Bucle de Raylib
    while (!WindowShouldClose()) {
      UpdateApplication(GetFrameTime()); 
      DrawApplication();                 
    }
  } else {
    RunTerminalMode();
  }

  CleanupApplication();

  return 0;
}


//------ Funcion para comparar ----------
void CompareAlgorithms(int startNodeId, int endNodeId) {
    if (startNodeId == -1 || endNodeId == -1) {
        std::cout << "ERROR: Por favor, seleccione un nodo de inicio y fin validos." << std::endl;
        return;
    }

    std::cout << "\n--- Comparando la eficiencia de los algoritmos ---\n";
    std::cout << "Buscando del nodo " << startNodeId << " al nodo " << endNodeId << ".\n";

    
    auto& algorithmsMap = myPathfinding->getAlgorithms(); 
    
    for (const auto& pair : algorithmsMap) {
        const std::string& name = pair.key;
        const SearchAlgorithm* algorithm = pair.value.get();
        
        myPathfinding->setAlgorithm(algorithm);

        auto start_time = std::chrono::high_resolution_clock::now();
        SimpleList<int> pathResult = myPathfinding->findPath(startNodeId, endNodeId);
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

        std::cout << "\nAlgoritmo: " << name << std::endl;
        std::cout << "Tiempo de busqueda: " << duration.count() << " us" << std::endl;
        std::cout << "Tamaño de la ruta: " << pathResult.size() << " nodos" << std::endl;
        std::cout << "Memoria usada: " << Utils::GetCurrentMemoryUsageInKB() << " KB" << std::endl;
  
    }
    std::cout << "---------------------------------------------------\n";
}
// --- Implementaciones de Funciones Auxiliares ---



void InitializeApplication(int numNodes) {
  if (numNodes <= GRAPHICAL_NODE_LIMIT) {
    graphicalMode = true;
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "RayPathFinder - Modo Grafico");
    SetTargetFPS(60); 

    // Inicializar cámara 2D de forma segura
    camera.target = {(float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2};
    camera.offset = {(float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    std::cout << "\nModo Grafico Activado." << std::endl;
  } else {
    graphicalMode = false;
    std::cout << "\n------------------------------------------" << std::endl;
    std::cout << "MODO TERMINAL ACTIVADO (Nodos: " << numNodes << ")"
              << std::endl;
    std::cout << "------------------------------------------" << std::endl;
    std::cout << "El dibujo del grafo completo esta deshabilitado para mas de "
              << GRAPHICAL_NODE_LIMIT << " nodos." << std::endl;
  }


  auto start_time_graph = std::chrono::high_resolution_clock::now();
  myGraph.generateRandomNodes(numNodes, SCREEN_WIDTH, SCREEN_HEIGHT,
                              MAX_EDGES_PER_NODE_GENERATION,
                              MAX_CONNECTION_DISTANCE_GENERATION);
  myGraph.generateRandomObstacles(NUM_OBSTACLES, SCREEN_WIDTH, SCREEN_HEIGHT);
  
  auto end_time_graph = std::chrono::high_resolution_clock::now();
  auto duration_graph = std::chrono::duration_cast<std::chrono::milliseconds>(end_time_graph - start_time_graph);

  size_t totalEdges = 0;
  for (int i = 0; i < myGraph.getNumNodes(); ++i) {
    totalEdges += myGraph.getAdjacentNodes(i).size();
  }
  std::cout << "--- Generacion del Grafo Completada ---" << std::endl;
  std::cout << "Numero de nodos generados: " << myGraph.getNumNodes() << std::endl;
  std::cout << "Numero total de entradas de aristas en lista de adyacencia: "
            << totalEdges << std::endl;
  std::cout << "Numero aproximado de aristas unicas (si no dirigido): "
            << totalEdges / 2 << std::endl;
  std::cout << "Tiempo de construccion del grafo: " << duration_graph.count() << " ms" << std::endl;
  std::cout << "Memoria usada despues de la construccion: " << Utils::GetCurrentMemoryUsageInKB() << " KB" << std::endl;


  // Mostrar detalles de obstáculos en terminal si no es modo gráfico
  if (!graphicalMode) {
    std::cout << "\n--- Obstaculos Generados ---" << std::endl;
    if (myGraph.getObstacles().empty()) {
      std::cout << "No se generaron obstaculos." << std::endl;
    } else {
      int obsCount = 0;
      for (const auto& obs : myGraph.getObstacles()) {
        std::cout << "  Obstaculo #" << obsCount++ << " (" << obs.name << "): "
                  << "Pos=(" << obs.rect.x << ", " << obs.rect.y << "), "
                  << "Tam=(" << obs.rect.width << ", " << obs.rect.height << ")"
                  << std::endl;
      }
    }
    std::cout << "----------------------------" << std::endl;
  }
  algorithms.set("A*", std::make_unique<AStar_Algorithm>());
  algorithms.set("BFS", std::make_unique<BFS_Algorithm>());
  algorithms.set("Dijkstra", std::make_unique<Dijkstra_Algorithm>());
  algorithms.set("BestFirst-Search", std::make_unique<BestFirst_Algorithm>());

  
  myPathfinding = std::make_unique<Pathfinding>(myGraph);
  myPathfinding->setAlgorithm(algorithms.get("A*").get());
}

// Actualiza la lógica del juego
void UpdateApplication(float deltaTime) {
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
    raylib::Vector2 delta =
        GetMouseDelta(); 
    delta = delta.Scale(-1.0f / camera.zoom);
    camera.target = (raylib::Vector2)camera.target + delta; 
  }

  // Lógica de zoom de la cámara con la rueda del ratón
  float wheel = raylib::Mouse::GetWheelMove();
  if (wheel != 0) {
    raylib::Vector2 mouseScreenPos =
        GetMousePosition(); 
    raylib::Vector2 mouseWorldPos = GetScreenToWorld2D(
        mouseScreenPos, camera); 

    camera.zoom += wheel * 0.1f;
    if (camera.zoom < 0.1f)
      camera.zoom = 0.1f; 

    raylib::Vector2 newMouseWorldPos =
        GetScreenToWorld2D(GetMousePosition(), camera);
    raylib::Vector2 zoomCorrection =
        newMouseWorldPos - mouseWorldPos; 
    camera.target = (raylib::Vector2)camera.target - zoomCorrection;
  }

  HandleGraphicalInput(); 
}

// Dibuja el estado actual de la aplicación (solo modo gráfico)
void DrawApplication() {
  BeginDrawing(); 
  ClearBackground(RAYWHITE);

  BeginMode2D(camera);

  // --- Dibuja los obstáculos PRIMERO para que no tapen la ruta ---
  for (const auto& obstacle : myGraph.getObstacles()) {
    DrawRectangleRec(obstacle.rect,
                     Fade(obstacle.color, 0.7f));  
    DrawRectangleLinesEx(obstacle.rect, 2, BLACK); 
    DrawText(obstacle.name.c_str(), (int)(obstacle.rect.x + 5),
             (int)(obstacle.rect.y + 5), 10, BLACK);
  }

  // Si estamos arrastrando para dibujar un nuevo obstáculo
  if (editingObstacles && isDragging) {
    raylib::Vector2 mouseWorldPos =
        GetScreenToWorld2D(GetMousePosition(), camera);
    float x1 = std::min(startDragPos.x, mouseWorldPos.x);
    float y1 = std::min(startDragPos.y, mouseWorldPos.y);
    float x2 = std::max(startDragPos.x, mouseWorldPos.x);
    float y2 = std::max(startDragPos.y, mouseWorldPos.y);
    Rectangle currentRect = {x1, y1, x2 - x1, y2 - y1};
    DrawRectangleRec(currentRect,
                     Fade(MAROON, 0.5f)); // Rectángulo que se está dibujando
    DrawRectangleLinesEx(currentRect, 2, WHITE);
  }

  DrawGraphElements(myGraph, path, startNodeId,
                    endNodeId); // Dibuja la ruta y nodos
  EndMode2D();                  // Finalizar el modo 2D

  DrawGraphicalHUD(); 

  // Dibuja la caja de entrada de texto para el nombre del obstáculo 
  if (editingObstacles && typingObstacleName) {
    DrawRectangleRec(nameInputBox, LIGHTGRAY);
    DrawRectangleLinesEx(nameInputBox, 2, BLACK);
    DrawText("Nombre Obstaculo (ENTER para confirmar):",
             (int)nameInputBox.x + 5, (int)nameInputBox.y - 15, 12, DARKGRAY);
    DrawText(obstacleNameInput, (int)nameInputBox.x + 5,
             (int)nameInputBox.y + 8, 15, BLACK);
    // Dibuja un cursor parpadeante
    if (((int)(GetTime() * 2)) % 2 == 0) { 
      DrawText("_",
               (int)nameInputBox.x + 5 + MeasureText(obstacleNameInput, 15),
               (int)nameInputBox.y + 8, 15, BLACK);
    }
  }

  EndDrawing(); // Finalizar el modo de dibujo
}

//----- maneja los externo------
void HandleGraphicalInput() {
  if (IsKeyPressed(KEY_O)) {
    editingObstacles = !editingObstacles;
    if (editingObstacles) {
      std::cout << "Modo de edicion de obstaculos ACTIVADO." << std::endl;
      startNodeId = -1;
      endNodeId = -1;
      path.clear();
    } else {
      std::cout << "Modo de edicion de obstaculos DESACTIVADO." << std::endl;
      typingObstacleName = false;
      obstacleNameInput[0] = '\0'; 
    }
  }

  if (editingObstacles) {
    // Lógica para dibujar y nombrar obstáculos
    raylib::Vector2 mouseWorldPos =
        GetScreenToWorld2D(GetMousePosition(), camera);

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      if (!typingObstacleName) {
        startDragPos = mouseWorldPos;
        isDragging = true;
      }
    } else if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
      if (isDragging) {
        isDragging = false;
        // Calcular el rectángulo final
        float x1 = std::min(startDragPos.x, mouseWorldPos.x);
        float y1 = std::min(startDragPos.y, mouseWorldPos.y);
        float x2 = std::max(startDragPos.x, mouseWorldPos.x);
        float y2 = std::max(startDragPos.y, mouseWorldPos.y);

        Rectangle newRect = {x1, y1, x2 - x1, y2 - y1};
        if (newRect.width > 5 && newRect.height > 5) {
          typingObstacleName = true;
        } else {
          typingObstacleName = false;
          obstacleNameInput[0] = '\0';
        }
      }
    }

    // Lógica de entrada de texto para el nombre del obstáculo
    if (typingObstacleName) {
      int key = GetCharPressed();
      while (key > 0) {
        if (((key >= 32) && (key <= 125)) &&
            (strlen(obstacleNameInput) < sizeof(obstacleNameInput) - 1)) {
          obstacleNameInput[strlen(obstacleNameInput)] = (char)key;
          obstacleNameInput[strlen(obstacleNameInput) + 1] = '\0';
        }
        key = GetCharPressed();
      }

      if (IsKeyPressed(KEY_BACKSPACE)) {
        if (strlen(obstacleNameInput) > 0) {
          obstacleNameInput[strlen(obstacleNameInput) - 1] = '\0';
        }
      }

      if (IsKeyPressed(KEY_ENTER)) {
        if (strlen(obstacleNameInput) > 0) {
          // Añadir el obstáculo con el nombre
          float x1 = std::min(startDragPos.x, mouseWorldPos.x);
          float y1 = std::min(startDragPos.y, mouseWorldPos.y);
          float x2 = std::max(startDragPos.x, mouseWorldPos.x);
          float y2 = std::max(startDragPos.y, mouseWorldPos.y);
          Rectangle finalRect = {x1, y1, x2 - x1, y2 - y1};

          // Añadir el obstáculo con un color aleatorio para distinguirlos
          Color randomColor = {(unsigned char)GetRandomValue(50, 200),
                               (unsigned char)GetRandomValue(50, 200),
                               (unsigned char)GetRandomValue(50, 200), 255};
          myGraph.addObstacle(
              Obstacle(finalRect, std::string(obstacleNameInput), randomColor));
          std::cout << "Obstaculo '" << obstacleNameInput << "' anadido."
                    << std::endl;
        } else {
          std::cout << "Nombre de obstaculo vacio. Obstaculo no anadido."
                    << std::endl;
        }
        typingObstacleName = false;
        obstacleNameInput[0] = '\0'; // Limpiar el buffer
        // Recalcular la ruta si ya hay inicio y fin seleccionados
        if (startNodeId != -1 && endNodeId != -1) {
          path = myPathfinding->findPath(startNodeId, endNodeId);
        }
      }
    }

    
    if (IsKeyPressed(KEY_ESCAPE) && typingObstacleName) {
      typingObstacleName = false;
      obstacleNameInput[0] = '\0';
    }

    // Eliminación de obstáculos con clic derecho 
    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && !isDragging) {
      for (int i = 0; i < myGraph.getObstacles().size(); ++i) {
        if (CheckCollisionPointRec(mouseWorldPos,
                                   myGraph.getObstacles()[i].rect)) {
          std::cout << "Obstaculo '" << myGraph.getObstacles()[i].name
                    << "' eliminado." << std::endl;
          myGraph.removeObstacle(i);
          if (startNodeId != -1 && endNodeId != -1) {
            path = myPathfinding->findPath(startNodeId, endNodeId);
          }
          break; 
        }
      }
    }

  } else { 
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      raylib::Vector2 mouseScreenPos = GetMousePosition();
      raylib::Vector2 mouseWorldPos = GetScreenToWorld2D(
          mouseScreenPos, camera); 
      int clickedNodeId = myGraph.findNodeAtPosition(mouseWorldPos, 8.0f);

      if (clickedNodeId != -1) { 
        if (startNodeId == -1) {
          startNodeId = clickedNodeId;
          endNodeId = -1; 
          path.clear();   
        } else if (endNodeId == -1 && clickedNodeId != startNodeId) {
          endNodeId = clickedNodeId;
          path = myPathfinding->findPath(startNodeId, endNodeId);
        } else {
          startNodeId = clickedNodeId;
          endNodeId = -1;
          path.clear();
        }
      }
    }

    if (IsKeyPressed(KEY_F1)) {
    // Asegúrate de que "A*" exista en tu mapa de algoritmos
      if (algorithms.contains("A*")) {
          myPathfinding->setAlgorithm(algorithms.get("A*").get());
          std::cout << "Algoritmo cambiado a A*." << std::endl;
      }
    }
    if (IsKeyPressed(KEY_F2)) {
      if (algorithms.contains("BFS")) {
          myPathfinding->setAlgorithm(algorithms.get("BFS").get());
          std::cout << "Algoritmo cambiado a BFS." << std::endl;
      }
    }
    if (IsKeyPressed(KEY_F3)) {
      if (algorithms.contains("Dijkstra")) {
          myPathfinding->setAlgorithm(algorithms.get("Dijkstra").get());
            std::cout << "Algoritmo cambiado a Dijkstra." << std::endl;
      }
    }
    
    if(IsKeyPressed(KEY_F5)){
      if (startNodeId != -1 && endNodeId != -1) {
        CompareAlgorithms(startNodeId, endNodeId);
      } else {
        std::cout << "ERROR: Seleccione un nodo de inicio y fin primero." << std::endl;
      }
    }
    if (IsKeyPressed(KEY_F6)) {
      if (algorithms.contains("BestFirst-Search")) {
          myPathfinding->setAlgorithm(algorithms.get("BestFirst-Search").get());
            std::cout << "Algoritmo cambiado a BestFirst-Search." << std::endl;
      }
    }
    if (IsKeyPressed(KEY_F7)) {
      if (algorithms.contains("DFS")) {
          myPathfinding->setAlgorithm(algorithms.get("DFS").get());
            std::cout << "Algoritmo cambiado a DFS" << std::endl;
      }
    }
    if (IsKeyPressed(KEY_R)) {
      if (startNodeId != -1 && endNodeId != -1) {
        path = myPathfinding->findPath(startNodeId, endNodeId);
      }
    }

    if (IsKeyPressed(KEY_C)) { 
      startNodeId = -1;
      endNodeId = -1;
      path.clear();
    }
  }


}

// Esta función está separada de DrawApplication para mantener la modularidad
void DrawGraphElements(const Graph& graph, const SimpleList<int>& pathNodes,
                       int startNodeId, int endNodeId) {
  if (graph.getNumNodes() <= GRAPHICAL_NODE_LIMIT) {
    // Dibujar todas las aristas
    for (int i = 0; i < graph.getNumNodes(); ++i) {
      const Node& currentNode = graph.getNode(i);
      for (const auto& edge : graph.getAdjacentNodes(i)) {
        const Node& targetNode = graph.getNode(edge.first);
        DrawLineV(currentNode.position, targetNode.position, GRAY);
      }
    }

    // Dibujar todos los nodos
    for (int i = 0; i < graph.getNumNodes(); ++i) {
      const Node& currentNode = graph.getNode(i);
      Color nodeColor = BLUE;
      if (i == startNodeId)
        nodeColor = GREEN;
      else if (i == endNodeId)
        nodeColor = RED;

      DrawCircleV(currentNode.position, 5, nodeColor);
    }
  }

  // --- Dibuja la ruta encontrada ---
  if (!pathNodes.empty()) {
    raylib::Vector2 prevPos;
    bool firstNode = true;
    for (auto it = pathNodes.begin(); it != pathNodes.end(); ++it) {
      int nodeId = *it;
      if (nodeId < 0 || nodeId >= graph.getNumNodes())
        continue; 

      const Node& currentPathNode = graph.getNode(nodeId);
      raylib::Vector2 currentPos = currentPathNode.position;

      if (!firstNode) {
        DrawLineEx(prevPos, currentPos, 4, LIME);
      }
      DrawCircleV(currentPos, 4, LIME);
      prevPos = currentPos;
      firstNode = false;
    }
  }

  // --- Nodo de inicio ---
  if (startNodeId >= 0 && startNodeId < graph.getNumNodes()) {
    DrawCircleV(graph.getNode(startNodeId).position, 8, GREEN);
  }

  // --- Nodo final ---
  if (endNodeId >= 0 && endNodeId < graph.getNumNodes()) {
    DrawCircleV(graph.getNode(endNodeId).position, 8, RED);
  }
}

// Dibuja la Interfaz de Usuario (HUD) en modo gráfico
void DrawGraphicalHUD() {
  // Muestra el número total de nodos
  DrawText(TextFormat("Nodos: %i", myGraph.getNumNodes()), 10, 10, 20,
           DARKGRAY);

  if (startNodeId != -1)
    DrawText(TextFormat("Inicio: %i", startNodeId), 10, 30, 20, DARKGRAY);
  if (endNodeId != -1)
    DrawText(TextFormat("Fin: %i", endNodeId), 10, 50, 20, DARKGRAY);

  
  DrawText("Clic IZQ: Seleccionar Inicio/Fin", 10, 80, 15, BLACK);
  DrawText("Clic DER + Arrastrar: Paneo", 10, 100, 15, BLACK);
  DrawText("Rueda mouse: Zoom", 10, 120, 15, BLACK);
  DrawText("R: Recalcular ruta | C: Limpiar seleccion", 10, 140, 15, BLACK);

  // Mensaje si no se encuentra ruta
  if (startNodeId != -1 && endNodeId != -1 && path.empty()) {
    DrawText("No se encontro ruta. (Posiblemente desconectados o sin camino)",
             10, 160, 15, RED);
  }

  
  DrawText("O: Alternar modo edicion obstaculos", 10, 180, 15,
           BLACK); 
  if (editingObstacles) {
    DrawText("En Modo Edicion Obstaculos:", 10, 200, 15, DARKBLUE);
    DrawText("Clic IZQ + Arrastrar: Dibujar obstaculo", 10, 220, 15, DARKBLUE);
    DrawText("Clic DER: Eliminar obstaculo", 10, 240, 15, DARKBLUE);
    DrawText("ESC: Cancelar nombre obstaculo", 10, 260, 15, DARKBLUE);
  }
}

// --- Lógica del Modo Terminal ---
void RunTerminalMode() {
  std::string command;
  int sNode = -1, eNode = -1;


  std::cout << "Algoritmos disponibles: ";
  bool first = true;
  for (const auto& pair : algorithms) {
    if (!first) std::cout << ", ";
    std::cout << pair.key;
    first = false;
  }
  std::cout << std::endl;
  std::string chosenAlgorithmName = "A*";

  do {
    std::cout << "\n------------------------------------------" << std::endl;
    std::cout << "Comandos: 'buscar', 'info', 'salir' 'set-algorithm' 'comparar'" << std::endl;
    std::cout << "Ingrese comando: ";
    std::cin >> command;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(),
                    '\n'); 

    if (command == "buscar") {
      sNode = GetValidNodeIdFromUser("Ingrese ID del nodo de inicio: ",
                                     myGraph.getNumNodes() - 1);
      if (sNode == -1)
        continue; 

      eNode = GetValidNodeIdFromUser("Ingrese ID del nodo final: ",
                                     myGraph.getNumNodes() - 1);
      if (eNode == -1)
        continue;

      if (sNode == eNode) {
        std::cout
            << "El nodo de inicio y fin son el mismo. No hay ruta que buscar."
            << std::endl;
        continue;
      }

      std::cout << "Buscando ruta de " << sNode << " a " << eNode << "..."
                << std::endl;
      auto startTime = std::chrono::high_resolution_clock::now();
      SimpleList<int> terminalPath = myPathfinding->findPath(sNode, eNode);
      auto endTime = std::chrono::high_resolution_clock::now();
      std::chrono::duration<double> elapsed = endTime - startTime;

      PrintPathToTerminal(terminalPath);
      std::cout << "Tiempo de busqueda: " << elapsed.count() << " segundos."
                << std::endl;

    }else if (command == "set-algorithm"){
      std::cout << "Algoritmos disponibles: ";
      first = true;
      for (const auto& pair : algorithms) {
        if (!first) std::cout << ", ";
        std::cout << pair.key;
        first = false;
      }
      std::cout << "\nElija un algoritmo: ";
      std::cin >> chosenAlgorithmName;
      if (algorithms.find(chosenAlgorithmName) == algorithms.end()) {
        std::cout << "Algoritmo no valido. Usando " << currentAlgorithm->getName() << " por defecto." << std::endl;
        chosenAlgorithmName = "A*"; 
      }
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }else if (command == "info") {
      std::cout << "Nodos totales en el grafo: " << myGraph.getNumNodes()
                << std::endl;
      // Mostrar detalles de obstáculos en terminal
      std::cout << "\n--- Obstaculos Actuales ---" << std::endl;
      if (myGraph.getObstacles().empty()) {
        std::cout << "No hay obstaculos definidos." << std::endl;
      } else {
        int obsCount = 0;
        for (const auto& obs : myGraph.getObstacles()) {
          std::cout << "  Obstaculo #" << obsCount++ << " (" << obs.name
                    << "): "
                    << "Pos=(" << obs.rect.x << ", " << obs.rect.y << "), "
                    << "Tam=(" << obs.rect.width << ", " << obs.rect.height
                    << ")" << std::endl;
        }
      }
      std::cout << "----------------------------" << std::endl;
    }else if (command == "comparar") {
      int sNode = GetValidNodeIdFromUser("ID de nodo de inicio: ", myGraph.getNumNodes() - 1);
      int eNode = GetValidNodeIdFromUser("ID de nodo de fin: ", myGraph.getNumNodes() - 1);

      if (sNode != -1 && eNode != -1) {
        CompareAlgorithms(sNode, eNode);
      }
    }else if (command != "salir") {
      std::cout << "Comando desconocido. Intente 'buscar', 'info', o 'salir'."
                << std::endl;
    }

  } while (command != "salir");
  std::cout << "Saliendo del modo terminal." << std::endl;
}

// obtener un ID de nodo válido del usuario en modo terminal
int GetValidNodeIdFromUser(const std::string& prompt, int maxId) {
  int nodeId;
  std::cout << prompt;
  std::cin >> nodeId;
  if (std::cin.fail()) {
    std::cerr << "Error: Entrada invalida. Por favor, ingrese un numero entero."
              << std::endl;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return -1; 
  }
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

  if (nodeId < 0 || nodeId > maxId) {
    std::cerr << "Error: ID de nodo fuera de rango. Rango valido: 0 a " << maxId
              << std::endl;
    return -1; 
  }
  return nodeId;
}

// Imprime la ruta al terminal
void PrintPathToTerminal(const SimpleList<int>& p) {
  if (!p.empty()) {
    std::cout << "Ruta encontrada: ";
    bool first = true;
    for (int nodeId : p) {
      if (!first) {
        std::cout << " -> ";
      }
      std::cout << nodeId;
      first = false;
    }
    std::cout << std::endl;
  } else {
    std::cout << "No se encontro ruta." << std::endl;
  }
}


void CleanupApplication() {
  if (graphicalMode) {
    CloseWindow(); 
  }
  if (myPathfinding) {
    myPathfinding.reset();
    myPathfinding = nullptr;
  }
  myGraph.clear(); 
  std::cout << "Recursos liberados. Adios!" << std::endl;
}
