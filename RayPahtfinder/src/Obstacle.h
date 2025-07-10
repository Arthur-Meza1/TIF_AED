#pragma once

#include "raylib.h"
#include <string>

// Estructura para representar un obstáculo con su forma y nombre
struct Obstacle {
    Rectangle rect;      // La forma del obstáculo
    std::string name;    // El nombre del obstáculo (ej. "Río", "Pared")
    Color color;         // Color para dibujar el obstáculo

    // Constructor por defecto
    Obstacle() : rect({0, 0, 0, 0}), name(""), color(DARKBLUE) {}

    // Constructor con parámetros
    Obstacle(Rectangle r, const std::string& n, Color c = DARKBLUE)
        : rect(r), name(n), color(c) {}
};