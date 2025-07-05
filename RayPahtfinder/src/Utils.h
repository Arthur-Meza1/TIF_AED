#pragma once
#include "raylib-cpp.hpp" // Para raylib::Vector2
#include <iostream>       // Para std::cin, std::cout
#include <limits>         // Para std::numeric_limits

// Calcula la distancia euclidiana entre dos puntos (Vector2).
float GetDistance(raylib::Vector2 p1, raylib::Vector2 p2);

// Limpia el buffer de entrada estándar (stdin).

void ClearInputBuffer();