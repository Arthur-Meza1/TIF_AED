#pragma once
#include "raylib-cpp.hpp" 
#include <iostream>       
#include <limits>
#include <unistd.h>

// Calcula la distancia euclidiana entre dos puntos (Vector2).
namespace Utils {
float GetDistance(raylib::Vector2 p1, raylib::Vector2 p2);

void ClearInputBuffer();
bool CheckLineRectangleCollision(raylib::Vector2 p1, raylib::Vector2 p2,
                                 raylib::Rectangle rec);
size_t GetCurrentMemoryUsageInKB();

} // namespace Utils

