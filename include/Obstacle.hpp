#pragma once
#include "raylib-cpp.hpp"
#include "raylib.h"
#include <string>

// Estructura para representar un obstáculo con su forma y nombre
struct Obstacle {
  Rectangle rect;
  std::string name;
  Color color;

  Obstacle() : rect({0, 0, 0, 0}), name(""), color(DARKBLUE) {}

  Obstacle(Rectangle r, const std::string& n, Color c = DARKBLUE) :
      rect(r),
      name(n),
      color(c) {}
};
