#include "Application.h"
#include <iostream>
#include <limits>
#include <chrono>
#include <algorithm>
#include <cstring>

// Constructor: Llama a la inicialización
Application::Application(int numNodes) {
    // Definimos si el modo es gráfico o terminal
    if (numNodes <= GRAPHICAL_NODE_LIMIT) {
        graphicalMode = true;
    } else {
        graphicalMode = false;
    }
    
    // Llama a la lógica de inicialización centralizada
    // Mover la lógica de InitializeApplication aquí
    if (graphicalMode) {
        InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "RayPathFinder - Modo Grafico");
        SetTargetFPS(60);

        camera.target = { (float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2 };
        camera.offset = { (float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2 };
        camera.rotation = 0.0f;
        camera.zoom = 1.0f;

        std::cout << "\nModo Grafico Activado." << std::endl;
    } else {
        std::cout << "\n------------------------------------------" << std::endl;
        std::cout << "MODO TERMINAL ACTIVADO (Nodos: " << numNodes << ")" << std::endl;
        std::cout << "------------------------------------------" << std::endl;
        std::cout << "El dibujo del grafo completo esta deshabilitado para mas de " << GRAPHICAL_NODE_LIMIT << " nodos." << std::endl;
    }

    // Lógica común a ambos modos
    const float NODE_GENERATION_AREA_PADDING = 50.0f;
    const int MAX_EDGES_PER_NODE_GENERATION = 4;
    const float MAX_CONNECTION_DISTANCE_GENERATION = 200.0f;
    myGraph.generateRandomNodes(numNodes, SCREEN_WIDTH, SCREEN_HEIGHT,
                                MAX_EDGES_PER_NODE_GENERATION, MAX_CONNECTION_DISTANCE_GENERATION);

    myGraph.generateRandomObstacles(NUM_OBSTACLES, SCREEN_WIDTH, SCREEN_HEIGHT);
    
    if (!graphicalMode) {
        // ... (lógica para mostrar detalles de obstáculos en terminal)
        std::cout << "\n--- Obstaculos Generados ---" << std::endl;
        if (myGraph.getObstacles().empty()) {
            std::cout << "No se generaron obstaculos." << std::endl;
        } else {
            int obsCount = 0;
            for (const auto& obs : myGraph.getObstacles()) {
                std::cout << "  Obstaculo #" << obsCount++ << " (" << obs.name << "): "
                          << "Pos=(" << obs.rect.x << ", " << obs.rect.y << "), "
                          << "Tam=(" << obs.rect.width << ", " << obs.rect.height << ")" << std::endl;
            }
        }
        std::cout << "----------------------------" << std::endl;
    }

    myPathfinding = new Pathfinding(myGraph);
}

// Destructor: Llama a la limpieza
Application::~Application() {
    if (graphicalMode) {
        CloseWindow();
    }
    if (myPathfinding) {
        delete myPathfinding;
        myPathfinding = nullptr;
    }
    myGraph.clear();
    std::cout << "Recursos liberados. Adios!" << std::endl;
}

void Application::Run() {
    if (graphicalMode) {
        runGraphicalMode();
    } else {
        runTerminalMode();
    }
}

// Lógica de Raylib en modo gráfico
void Application::runGraphicalMode() {
    while (!WindowShouldClose()) {
        updateGraphicalMode(GetFrameTime());
        drawGraphicalMode();
    }
}

// Implementaciones de las funciones de lógica
void Application::updateGraphicalMode(float deltaTime) {
    // ... (lógica de cámara, paneo y zoom) ...
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
        raylib::Vector2 delta = GetMouseDelta();
        delta = delta.Scale(-1.0f / camera.zoom);
        camera.target = (raylib::Vector2)camera.target + delta;
    }

    float wheel = raylib::Mouse::GetWheelMove();
    if (wheel != 0) {
        raylib::Vector2 mouseScreenPos = GetMousePosition();
        raylib::Vector2 mouseWorldPos = GetScreenToWorld2D(mouseScreenPos, camera);

        camera.zoom += wheel * 0.1f;
        if (camera.zoom < 0.1f) camera.zoom = 0.1f;

        raylib::Vector2 newMouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);
        raylib::Vector2 zoomCorrection = newMouseWorldPos - mouseWorldPos;
        camera.target = (raylib::Vector2)camera.target - zoomCorrection;
    }
    
    // Llama al manejo de la entrada gráfica
    handleGraphicalInput();
}

void Application::drawGraphicalMode() {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    BeginMode2D(camera);

    // Dibuja los obstáculos
    for (const auto& obstacle : myGraph.getObstacles()) {
        DrawRectangleRec(obstacle.rect, Fade(obstacle.color, 0.7f));
        DrawRectangleLinesEx(obstacle.rect, 2, BLACK);
        DrawText(obstacle.name.c_str(), (int)(obstacle.rect.x + 5), (int)(obstacle.rect.y + 5), 10, BLACK);
    }
    
    // Si se está dibujando un obstáculo...
    if (editingObstacles && isDragging) {
        raylib::Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);
        float x1 = std::min(startDragPos.x, mouseWorldPos.x);
        float y1 = std::min(startDragPos.y, mouseWorldPos.y);
        float x2 = std::max(startDragPos.x, mouseWorldPos.x);
        float y2 = std::max(startDragPos.y, mouseWorldPos.y);
        Rectangle currentRect = { x1, y1, x2 - x1, y2 - y1 };
        DrawRectangleRec(currentRect, Fade(MAROON, 0.5f));
        DrawRectangleLinesEx(currentRect, 2, WHITE);
    }

    // Dibuja el grafo y la ruta
    drawGraphElements();
    EndMode2D();
    
    // Dibuja el HUD
    drawGraphicalHUD();

    // Dibuja la caja de texto para el nombre del obstáculo
    if (editingObstacles && typingObstacleName) {
        DrawRectangleRec(nameInputBox, LIGHTGRAY);
        DrawRectangleLinesEx(nameInputBox, 2, BLACK);
        DrawText("Nombre Obstaculo (ENTER para confirmar):", (int)nameInputBox.x + 5, (int)nameInputBox.y - 15, 12, DARKGRAY);
        DrawText(obstacleNameInput, (int)nameInputBox.x + 5, (int)nameInputBox.y + 8, 15, BLACK);
        if (((int)(GetTime() * 2)) % 2 == 0) {
            DrawText("_", (int)nameInputBox.x + 5 + MeasureText(obstacleNameInput, 15), (int)nameInputBox.y + 8, 15, BLACK);
        }
    }

    EndDrawing();
}

void Application::handleGraphicalInput() {
    // Tecla 'O' para alternar edición
    if (IsKeyPressed(KEY_O)) {
        // ... (lógica de activación/desactivación del modo de edición) ...
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
        raylib::Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);
        
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !typingObstacleName) {
            startDragPos = mouseWorldPos;
            isDragging = true;
        } else if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && isDragging) {
            isDragging = false;
            float x1 = std::min(startDragPos.x, mouseWorldPos.x);
            float y1 = std::min(startDragPos.y, mouseWorldPos.y);
            float x2 = std::max(startDragPos.x, mouseWorldPos.x);
            float y2 = std::max(startDragPos.y, mouseWorldPos.y);
            Rectangle newRect = { x1, y1, x2 - x1, y2 - y1 };

            if (newRect.width > 5 && newRect.height > 5) {
                typingObstacleName = true;
            } else {
                typingObstacleName = false;
                obstacleNameInput[0] = '\0';
            }
        }
        // ... (lógica de entrada de texto y eliminación de obstáculos) ...
        if (typingObstacleName) {
            int key = GetCharPressed();
            while (key > 0) {
                if (((key >= 32) && (key <= 125)) && (strlen(obstacleNameInput) < sizeof(obstacleNameInput) - 1)) {
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
                    float x1 = std::min(startDragPos.x, mouseWorldPos.x);
                    float y1 = std::min(startDragPos.y, mouseWorldPos.y);
                    float x2 = std::max(startDragPos.x, mouseWorldPos.x);
                    float y2 = std::max(startDragPos.y, mouseWorldPos.y);
                    Rectangle finalRect = { x1, y1, x2 - x1, y2 - y1 };

                    Color randomColor = { (unsigned char)GetRandomValue(50, 200), (unsigned char)GetRandomValue(50, 200), (unsigned char)GetRandomValue(50, 200), 255 };
                    myGraph.addObstacle(Obstacle(finalRect, std::string(obstacleNameInput), randomColor));
                    std::cout << "Obstaculo '" << obstacleNameInput << "' anadido." << std::endl;
                } else {
                    std::cout << "Nombre de obstaculo vacio. Obstaculo no anadido." << std::endl;
                }
                typingObstacleName = false;
                obstacleNameInput[0] = '\0';
                if (startNodeId != -1 && endNodeId != -1) {
                    path = myPathfinding->findPath(startNodeId, endNodeId);
                }
            }
        }

        if (IsKeyPressed(KEY_ESCAPE) && typingObstacleName) {
            typingObstacleName = false;
            obstacleNameInput[0] = '\0';
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && !isDragging) {
             for (int i = 0; i < myGraph.getObstacles().size(); ++i) {
                if (CheckCollisionPointRec(mouseWorldPos, myGraph.getObstacles()[i].rect)) {
                    std::cout << "Obstaculo '" << myGraph.getObstacles()[i].name << "' eliminado." << std::endl;
                    myGraph.removeObstacle(i);
                    if (startNodeId != -1 && endNodeId != -1) {
                        path = myPathfinding->findPath(startNodeId, endNodeId);
                    }
                    break;
                }
            }
        }
    } else { // Lógica para seleccionar nodos
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            raylib::Vector2 mouseScreenPos = GetMousePosition();
            raylib::Vector2 mouseWorldPos = GetScreenToWorld2D(mouseScreenPos, camera);
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

void Application::drawGraphElements() {
    if (myGraph.getNumNodes() <= GRAPHICAL_NODE_LIMIT) {
        // Dibujar aristas y nodos
        for (int i = 0; i < myGraph.getNumNodes(); ++i) {
            const Node& currentNode = myGraph.getNode(i);
            for (const auto& edge : myGraph.getAdjacentNodes(i)) {
                const Node& targetNode = myGraph.getNode(edge.first);
                DrawLineV(currentNode.position, targetNode.position, GRAY);
            }
        }

        for (int i = 0; i < myGraph.getNumNodes(); ++i) {
            const Node& currentNode = myGraph.getNode(i);
            Color nodeColor = BLUE;
            if (i == startNodeId) nodeColor = GREEN;
            else if (i == endNodeId) nodeColor = RED;
            DrawCircleV(currentNode.position, 5, nodeColor);
        }
    }

    // Dibuja la ruta encontrada
    if (!path.empty()) {
        raylib::Vector2 prevPos;
        bool firstNode = true;
        for (auto it = path.begin(); it != path.end(); ++it) {
            int nodeId = *it;
            if (nodeId < 0 || nodeId >= myGraph.getNumNodes()) continue;

            const Node& currentPathNode = myGraph.getNode(nodeId);
            raylib::Vector2 currentPos = currentPathNode.position;

            if (!firstNode) {
                DrawLineEx(prevPos, currentPos, 4, LIME);
            }
            DrawCircleV(currentPos, 4, LIME);
            prevPos = currentPos;
            firstNode = false;
        }
    }

    // Dibuja los nodos de inicio y fin con un círculo más grande
    if (startNodeId >= 0 && startNodeId < myGraph.getNumNodes()) {
        DrawCircleV(myGraph.getNode(startNodeId).position, 8, GREEN);
    }
    if (endNodeId >= 0 && endNodeId < myGraph.getNumNodes()) {
        DrawCircleV(myGraph.getNode(endNodeId).position, 8, RED);
    }
}

void Application::drawGraphicalHUD() {
    // Dibuja el HUD con texto
    DrawText(TextFormat("Nodos: %i", myGraph.getNumNodes()), 10, 10, 20, DARKGRAY);
    if (startNodeId != -1) DrawText(TextFormat("Inicio: %i", startNodeId), 10, 30, 20, DARKGRAY);
    if (endNodeId != -1) DrawText(TextFormat("Fin: %i", endNodeId), 10, 50, 20, DARKGRAY);
    DrawText("Clic IZQ: Seleccionar Inicio/Fin", 10, 80, 15, BLACK);
    DrawText("Clic DER + Arrastrar: Paneo", 10, 100, 15, BLACK);
    DrawText("Rueda mouse: Zoom", 10, 120, 15, BLACK);
    DrawText("R: Recalcular ruta | C: Limpiar seleccion", 10, 140, 15, BLACK);

    if (startNodeId != -1 && endNodeId != -1 && path.empty()) {
        DrawText("No se encontro ruta. (Posiblemente desconectados o sin camino)", 10, 160, 15, RED);
    }
    
    DrawText("O: Alternar modo edicion obstaculos", 10, 180, 15, BLACK);
    if (editingObstacles) {
        DrawText("En Modo Edicion Obstaculos:", 10, 200, 15, DARKBLUE);
        DrawText("Clic IZQ + Arrastrar: Dibujar obstaculo", 10, 220, 15, DARKBLUE);
        DrawText("Clic DER: Eliminar obstaculo", 10, 240, 15, DARKBLUE);
        DrawText("ESC: Cancelar nombre obstaculo", 10, 260, 15, DARKBLUE);
    }
}

// Lógica del modo terminal
void Application::runTerminalMode() {
    std::string command;
    int sNode = -1, eNode = -1;

    do {
        std::cout << "\n------------------------------------------" << std::endl;
        std::cout << "Comandos: 'buscar', 'info', 'salir'" << std::endl;
        std::cout << "Ingrese comando: ";
        std::cin >> command;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (command == "buscar") {
            sNode = getValidNodeIdFromUser("Ingrese ID del nodo de inicio: ", myGraph.getNumNodes() - 1);
            if (sNode == -1) continue;

            eNode = getValidNodeIdFromUser("Ingrese ID del nodo final: ", myGraph.getNumNodes() - 1);
            if (eNode == -1) continue;

            if (sNode == eNode) {
                 std::cout << "El nodo de inicio y fin son el mismo. No hay ruta que buscar." << std::endl;
                 continue;
            }

            std::cout << "Buscando ruta de " << sNode << " a " << eNode << "..." << std::endl;
            auto startTime = std::chrono::high_resolution_clock::now();
            path = myPathfinding->findPath(sNode, eNode);
            auto endTime = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = endTime - startTime;

            printPathToTerminal(path);
            std::cout << "Tiempo de busqueda: " << elapsed.count() << " segundos." << std::endl;

        } else if (command == "info") {
            // ... (lógica para mostrar información de nodos y obstáculos) ...
            std::cout << "Nodos totales en el grafo: " << myGraph.getNumNodes() << std::endl;
            std::cout << "\n--- Obstaculos Actuales ---" << std::endl;
            if (myGraph.getObstacles().empty()) {
                std::cout << "No hay obstaculos definidos." << std::endl;
            } else {
                int obsCount = 0;
                for (const auto& obs : myGraph.getObstacles()) {
                    std::cout << "  Obstaculo #" << obsCount++ << " (" << obs.name << "): "
                              << "Pos=(" << obs.rect.x << ", " << obs.rect.y << "), "
                              << "Tam=(" << obs.rect.width << ", " << obs.rect.height << ")" << std::endl;
                }
            }
            std::cout << "----------------------------" << std::endl;
        } else if (command != "salir") {
            std::cout << "Comando desconocido. Intente 'buscar', 'info', o 'salir'." << std::endl;
        }

    } while (command != "salir");
    std::cout << "Saliendo del modo terminal." << std::endl;
}

int Application::getValidNodeIdFromUser(const std::string& prompt, int maxId) const {
    int nodeId;
    std::cout << prompt;
    std::cin >> nodeId;
    if (std::cin.fail()) {
        std::cerr << "Error: Entrada invalida. Por favor, ingrese un numero entero." << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return -1;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (nodeId < 0 || nodeId > maxId) {
        std::cerr << "Error: ID de nodo fuera de rango. Rango valido: 0 a " << maxId << std::endl;
        return -1;
    }
    return nodeId;
}

void Application::printPathToTerminal(const SimpleList<int>& p) const {
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