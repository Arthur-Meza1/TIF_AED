#include "raylib-cpp.hpp"  
#include "Graph.h"        
#include "Obstacle.h"       
#include "Utils.h"
#include "Pathfinding.h"
#include "Node.h"        
#include <iostream>       
#include <vector>         
#include <list>            
#include <string>         


const int screenWidth = 1280; 
const int screenHeight = 720; 


Graph myGraph;                   
std::vector<RiverConnection> myRivers; 
Pathfinding* myPathfinding = nullptr; 

int startNodeId = -1; 
int endNodeId = -1;   
std::list<int> currentPath; 

// --- Cámara para Zoom y Paneo ---
raylib::Camera2D camera = { }; 
// --- Funciones para Entrada de Usuario (Consola) ---
std::vector<RiverConnection> readRiverConnectionsFromTerminal() {
    std::vector<RiverConnection> rivers;
    int numRivers;

    std::cout << "--- Configuracion de Rios (Conexiones Bloqueadas) ---" << std::endl;
    std::cout << "Cuantos rios (conexiones bloqueadas entre nodos) desea introducir? ";
    std::cin >> numRivers;      
    ClearInputBuffer();         

    for (int i = 0; i < numRivers; ++i) {
        int nodeA_id, nodeB_id;
        std::cout << "Rio " << (i + 1) << ": Bloquear conexion entre nodo ID (A) y nodo ID (B): ";
        std::cin >> nodeA_id >> nodeB_id;  
        ClearInputBuffer();               
        rivers.emplace_back(nodeA_id, nodeB_id); //añade rio
    }
    std::cout << "--- Rios Configurados ---" << std::endl;
    return rivers; // Retorna la lista de ríos
}


// dibuja todos los elementos del grafo: nodos, aristas, ríos, y la ruta encontrada.
void DrawGraphElements(const Graph& graph, const std::list<int>& path, int startId, int endId) {
    // Dibujar aristas (caminos) normales
    for (int i = 0; i < graph.getNumNodes(); ++i) {
        raylib::Vector2 nodePos = graph.getNode(i).position; // Posición del nodo actual
        const auto& neighbors = graph.getNeighbors(i);       // Vecinos del nodo actual
        for (const auto& neighborPair : neighbors) {
            raylib::Vector2 neighborPos = graph.getNode(neighborPair.first).position; // Posición del nodo vecino
            DrawLineEx(nodePos,neighborPos,1.0f, raylib::Color::DarkGray());
        }
    }

    // Dibujar los "ríos" (conexiones bloqueadas)
    for (const auto& river : graph.getRiverConnections()) {
        raylib::Vector2 pos1 = graph.getNode(river.nodeId1).position;
        raylib::Vector2 pos2 = graph.getNode(river.nodeId2).position;
        DrawLineEx(pos1,pos2,3,raylib::Color::Red()); //linea roja los rios(obstculos)
    }

    // Dibujar nodos individuales
    for (int i = 0; i < graph.getNumNodes(); ++i) {
        raylib::Vector2 nodePos = graph.getNode(i).position;
        DrawCircleV(nodePos,3, raylib::Color::Blue()); 
        
        if (camera.zoom > 1.5) { //mucho zoom dibuja ID
             raylib::DrawText(TextFormat("%i", i), (int)nodePos.x + 5, (int)nodePos.y + 5, 10, raylib::Color::Black());
        }
    }

    //dibujar ruta si existe 
    if (!path.empty()) {
        raylib::Vector2 prevPos = graph.getNode(path.front()).position; // Posición del primer nodo en la ruta
        bool first = true;
        for (int nodeId : path) {
            raylib::Vector2 currentPos = graph.getNode(nodeId).position; 
            if (!first) {
                DrawLineEx(prevPos, currentPos, 4, raylib::Color::Green()); 
            }
            prevPos = currentPos; // Actualiza la posición anterior
            first = false;
        }
    }

    // Dibujar los nodos de inicio y fin seleccionados (resaltados)
    if (startId != -1) { // Si hay un nodo de inicio seleccionado
        raylib::Vector2 sNodePos = graph.getNode(startId).position;
        DrawCircleV(sNodePos, 8, raylib::Color::Lime()); 
        raylib::DrawText("Inicio", (int)sNodePos.x + 10, (int)sNodePos.y + 10, 15, raylib::Color::DarkGreen()); 
    }
    if (endId != -1) { // Si hay un nodo de fin seleccionado
        raylib::Vector2 eNodePos = graph.getNode(endId).position;
        DrawCircleV(eNodePos, 8, raylib::Color::Red()); 
        raylib::DrawText("Fin", (int)eNodePos.x + 10, (int)eNodePos.y + 10, 15, raylib::Color::DarkBrown()); 
    }
}

// --- Función Principal (El punto de entrada del programa) ---
int main() {
    
    myRivers = readRiverConnectionsFromTerminal();

    
    raylib::Window window(screenWidth, screenHeight, "Raylib Pathfinder");
    window.SetTargetFPS(60); 

    
    camera.target = { (float)screenWidth / 2, (float)screenHeight / 2 };
    camera.offset = { (float)screenWidth / 2, (float)screenHeight / 2 };
    camera.rotation = 0.0f; 
    camera.zoom = 1.0f;     

    
    myGraph.generateNodesAndConnections(500, 70.0f, myRivers);

    // 5. Inicializar el objeto Pathfinding con el grafo creado.
    myPathfinding = new Pathfinding(myGraph);

   
    while (!window.ShouldClose()) { 

        
        if (raylib::Mouse::IsButtonDown(MOUSE_BUTTON_RIGHT)) {
            raylib::Vector2 delta = raylib::Mouse::GetDelta(); 
            delta = delta.Scale(-1.0f / camera.zoom);          
            camera.target = (raylib::Vector2)camera.target + delta;             
        }

        // Zoom del mapa (con la rueda del mouse)
        float wheel = raylib::Mouse::GetWheelMove(); //valor de rueda 
        if (wheel != 0) {
            
            raylib::Vector2 mouseWorldPos = GetScreenToWorld2D(raylib::Mouse::GetPosition(), camera);

            camera.zoom += wheel * 0.1f * camera.zoom; 
            //limites del zoom
            if (camera.zoom < 0.1f) camera.zoom = 0.1f; 
            if (camera.zoom > 10.0f) camera.zoom = 10.0f; 

            
            camera.offset = raylib::Mouse::GetPosition() - (mouseWorldPos * camera.zoom);
        }

        //eleccion de nodos
        if (raylib::Mouse::IsButtonPressed(MOUSE_BUTTON_LEFT)) {
            raylib::Vector2 mouseScreenPos = raylib::Mouse::GetPosition(); //pixeles             
            raylib::Vector2 mouseWorldPos = GetScreenToWorld2D(mouseScreenPos, camera);
            int clickedNodeId = myGraph.getNearestNodeId(mouseWorldPos);

            if (clickedNodeId != -1) { 
                if (startNodeId == -1) { 
                    startNodeId = clickedNodeId; 
                    endNodeId = -1;              
                    currentPath.clear();         
                } else if (endNodeId == -1 && clickedNodeId != startNodeId) { 
                    endNodeId = clickedNodeId;    
                    
                    currentPath = myPathfinding->findShortestPath(startNodeId, endNodeId); //calcular ruta
                } else { //si hay otro click sera el nuevo inicio
                    startNodeId = clickedNodeId;
                    endNodeId = -1;
                    currentPath.clear();
                }
            }
        }

        //recalcular ruta 
        if (raylib::Keyboard::IsKeyPressed(KEY_R)) {
             if (startNodeId != -1 && endNodeId != -1) {
                currentPath = myPathfinding->findShortestPath(startNodeId, endNodeId);
            }
        }
        //limpiar
        if (raylib::Keyboard::IsKeyPressed(KEY_C)) {
            startNodeId = -1;
            endNodeId = -1;
            currentPath.clear();
        }


        
        window.BeginDrawing(); 

        ClearBackground(raylib::Color::RayWhite()); 

        
        BeginMode2D(camera);
        DrawGraphElements(myGraph, currentPath, startNodeId, endNodeId); 
        EndMode2D(); 

        
        raylib::DrawText(TextFormat("Nodos: %i", myGraph.getNumNodes()), 10, 10, 20, raylib::Color::DarkGray());
        if (startNodeId != -1) raylib::DrawText(TextFormat("Inicio: %i", startNodeId), 10, 30, 20, raylib::Color::DarkGray());
        if (endNodeId != -1) raylib::DrawText(TextFormat("Fin: %i", endNodeId), 10, 50, 20, raylib::Color::DarkGray());
        raylib::DrawText("Clic IZQ: Seleccionar Inicio/Fin", 10, 80, 15, raylib::Color::Black());
        raylib::DrawText("Clic DER + Arrastrar: Paneo", 10, 100, 15, raylib::Color::Black());
        raylib::DrawText("Rueda mouse: Zoom", 10, 120, 15, raylib::Color::Black());
        raylib::DrawText("R: Recalcular ruta | C: Limpiar seleccion", 10, 140, 15, raylib::Color::Black());


        window.EndDrawing(); 
    }

    // --- Limpieza ---
    delete myPathfinding; 
    window.Close();       

    return 0; 
}