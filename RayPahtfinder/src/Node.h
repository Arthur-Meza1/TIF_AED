#pragma once 
#include "raylib-cpp.hpp" 

struct Node {
    int id;                   
    raylib::Vector2 position; 

    Node(int id, float x, float y) : id(id), position(::Vector2{x, y}) {}
};