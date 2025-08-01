cmake_minimum_required(VERSION 3.11.0)
project(RayPathFinderApp CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COOMANDS ON)

set(RAYLIB_VERSION 5.5)
find_package(raylib ${RAYLIB_VERSION} QUIET)
if (NOT raylib_FOUND)
  include(FetchContent)
  FetchContent_Declare(
    raylib
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
    URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
  )
  FetchContent_GetProperties(raylib)
  if (NOT raylib_POPULATED)
    set(FETCHCONTENT_QUIET NO)
    FetchContent_MakeAvailable(raylib)
  endif()
endif()

set(RAYLIB_CPP_VERSION 5.5.0)
find_package(raylib_cpp ${RAYLIB_CPP_VERSION} QUIET)
if (NOT raylib_cpp_FOUND)
  include(FetchContent)
  FetchContent_Declare(
    raylib_cpp
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
    URL https://github.com/RobLoach/raylib-cpp/archive/refs/tags/v${RAYLIB_CPP_VERSION}.tar.gz 
  )
  FetchContent_GetProperties(raylib_cpp)
  if (NOT raylib_cpp_POPULATED)
    set(FETCHCONTENT_QUIET NO)
    FetchContent_MakeAvailable(raylib_cpp)
  endif()
endif()

set(PROJECT_SOURCES
  src/Graph.cpp
  src/Pathfinding.cpp
  src/Utils.cpp
  main.cpp
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE raylib raylib_cpp)
target_include_directories(${PROJECT_NAME} PRIVATE include data_structures)
