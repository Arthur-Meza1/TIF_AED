#pragma once

#include "raylib-cpp.hpp"
#include "Estructuras_datos/list.h"
#include "Graph.h"
#include "Node.h"
#include "Pathfinding.h"
#include "Obstacle.h"    
#include <string>

class Application {
public:
    Application(int numNodes);
    ~Application();
    void Run();

private:
    // Variables de estado del programa
    Graph myGraph;
    Pathfinding* myPathfinding = nullptr;
    int startNodeId = -1;
    int endNodeId = -1;
    SimpleList<int> path;
    raylib::Camera2D camera;
    bool graphicalMode = true;

    // Variables para la edición de obstáculos
    bool editingObstacles = false;
    raylib::Vector2 startDragPos;
    bool isDragging = false;
    char obstacleNameInput[64] = "";
    bool typingObstacleName = false;
    Rectangle nameInputBox = { 10, (float)SCREEN_HEIGHT - 40, 200, 30 };
    
    // Constantes del programa
    const int SCREEN_WIDTH = 1200;
    const int SCREEN_HEIGHT = 800;
    const int GRAPHICAL_NODE_LIMIT = 700;
    const int NUM_OBSTACLES = 5;

    // Métodos para la lógica principal
    void initialize();
    void cleanup();
    
    // Métodos para el modo gráfico
    void runGraphicalMode();
    void updateGraphicalMode(float deltaTime);
    void drawGraphicalMode();
    void handleGraphicalInput();
    void drawGraphicalHUD();
    void drawGraphElements();

    // Métodos para el modo terminal
    void runTerminalMode();
    void printPathToTerminal(const SimpleList<int>& p) const;
    int getValidNodeIdFromUser(const std::string& prompt, int maxId) const;
};