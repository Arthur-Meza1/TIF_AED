#pragma once
#include "raylib-cpp.hpp"

struct Node {
  int id;
  raylib::Vector2 position;

  Node(int id, float x, float y) : id(id), position(raylib::Vector2{x, y}) {}

  Node() : id(-1), position(raylib::Vector2(0.0f, 0.0f)) {}
};
