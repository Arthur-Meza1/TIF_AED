cmake_minimum_required(VERSION 3.31.7)
project(RayPathFinderApp CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(raylib CONFIG REQUIRED)
include_directories(src/ventor/raylib-cpp/include)

set(PROJECT_SOURCES
    src/main.cpp
    src/Graph.cpp
    src/Graph.h
    src/Node.h
    src/Obstacle.h
    src/Pathfinding.cpp
    src/Pathfinding.h
    src/Utils.cpp
    src/Utils.h
)

# Crear el ejecutable
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

# 3. Enlazar la librería base raylib a tu ejecutable
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)