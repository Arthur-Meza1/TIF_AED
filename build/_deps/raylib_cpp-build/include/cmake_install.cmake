# Install script for directory: /home/arthurm/Documentos/AED/Pruebas/mas/aed_project/build/_deps/raylib_cpp-src/include

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "0")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set path to fallback-tool for dependency-resolution.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "/home/arthurm/Documentos/AED/Pruebas/mas/aed_project/build/_deps/raylib_cpp-src/include/AudioDevice.hpp"
    "/home/arthurm/Documentos/AED/Pruebas/mas/aed_project/build/_deps/raylib_cpp-src/include/AudioStream.hpp"
    "/home/arthurm/Documentos/AED/Pruebas/mas/aed_project/build/_deps/raylib_cpp-src/include/AutomationEventList.hpp"
    "/home/arthurm/Documentos/AED/Pruebas/mas/aed_project/build/_deps/raylib_cpp-src/include/BoundingBox.hpp"
    "/home/arthurm/Documentos/AED/Pruebas/mas/aed_project/build/_deps/raylib_cpp-src/include/Camera2D.hpp"
    "/home/arthurm/Documentos/AED/Pruebas/mas/aed_project/build/_deps/raylib_cpp-src/include/Camera3D.hpp"
    "/home/arthurm/Documentos/AED/Pruebas/mas/aed_project/build/_deps/raylib_cpp-src/include/Color.hpp"
    "/home/arthurm/Documentos/AED/Pruebas/mas/aed_project/build/_deps/raylib_cpp-src/include/FileData.hpp"
    "/home/arthurm/Documentos/AED/Pruebas/mas/aed_project/build/_deps/raylib_cpp-src/include/FileText.hpp"
    "/home/arthurm/Documentos/AED/Pruebas/mas/aed_project/build/_deps/raylib_cpp-src/include/Font.hpp"
    "/home/arthurm/Documentos/AED/Pruebas/mas/aed_project/build/_deps/raylib_cpp-src/include/Functions.hpp"
    "/home/arthurm/Documentos/AED/Pruebas/mas/aed_project/build/_deps/raylib_cpp-src/include/Gamepad.hpp"
    "/home/arthurm/Documentos/AED/Pruebas/mas/aed_project/build/_deps/raylib_cpp-src/include/Image.hpp"
    "/home/arthurm/Documentos/AED/Pruebas/mas/aed_project/build/_deps/raylib_cpp-src/include/Keyboard.hpp"
    "/home/arthurm/Documentos/AED/Pruebas/mas/aed_project/build/_deps/raylib_cpp-src/include/Material.hpp"
    "/home/arthurm/Documentos/AED/Pruebas/mas/aed_project/build/_deps/raylib_cpp-src/include/Matrix.hpp"
    "/home/arthurm/Documentos/AED/Pruebas/mas/aed_project/build/_deps/raylib_cpp-src/include/Mesh.hpp"
    "/home/arthurm/Documentos/AED/Pruebas/mas/aed_project/build/_deps/raylib_cpp-src/include/MeshUnmanaged.hpp"
    "/home/arthurm/Documentos/AED/Pruebas/mas/aed_project/build/_deps/raylib_cpp-src/include/Model.hpp"
    "/home/arthurm/Documentos/AED/Pruebas/mas/aed_project/build/_deps/raylib_cpp-src/include/ModelAnimation.hpp"
    "/home/arthurm/Documentos/AED/Pruebas/mas/aed_project/build/_deps/raylib_cpp-src/include/Mouse.hpp"
    "/home/arthurm/Documentos/AED/Pruebas/mas/aed_project/build/_deps/raylib_cpp-src/include/Music.hpp"
    "/home/arthurm/Documentos/AED/Pruebas/mas/aed_project/build/_deps/raylib_cpp-src/include/Ray.hpp"
    "/home/arthurm/Documentos/AED/Pruebas/mas/aed_project/build/_deps/raylib_cpp-src/include/RayCollision.hpp"
    "/home/arthurm/Documentos/AED/Pruebas/mas/aed_project/build/_deps/raylib_cpp-src/include/RaylibException.hpp"
    "/home/arthurm/Documentos/AED/Pruebas/mas/aed_project/build/_deps/raylib_cpp-src/include/raylib-cpp-utils.hpp"
    "/home/arthurm/Documentos/AED/Pruebas/mas/aed_project/build/_deps/raylib_cpp-src/include/raylib-cpp.hpp"
    "/home/arthurm/Documentos/AED/Pruebas/mas/aed_project/build/_deps/raylib_cpp-src/include/raylib.hpp"
    "/home/arthurm/Documentos/AED/Pruebas/mas/aed_project/build/_deps/raylib_cpp-src/include/raymath.hpp"
    "/home/arthurm/Documentos/AED/Pruebas/mas/aed_project/build/_deps/raylib_cpp-src/include/Rectangle.hpp"
    "/home/arthurm/Documentos/AED/Pruebas/mas/aed_project/build/_deps/raylib_cpp-src/include/RenderTexture.hpp"
    "/home/arthurm/Documentos/AED/Pruebas/mas/aed_project/build/_deps/raylib_cpp-src/include/ShaderUnmanaged.hpp"
    "/home/arthurm/Documentos/AED/Pruebas/mas/aed_project/build/_deps/raylib_cpp-src/include/Shader.hpp"
    "/home/arthurm/Documentos/AED/Pruebas/mas/aed_project/build/_deps/raylib_cpp-src/include/Sound.hpp"
    "/home/arthurm/Documentos/AED/Pruebas/mas/aed_project/build/_deps/raylib_cpp-src/include/Text.hpp"
    "/home/arthurm/Documentos/AED/Pruebas/mas/aed_project/build/_deps/raylib_cpp-src/include/Texture.hpp"
    "/home/arthurm/Documentos/AED/Pruebas/mas/aed_project/build/_deps/raylib_cpp-src/include/TextureUnmanaged.hpp"
    "/home/arthurm/Documentos/AED/Pruebas/mas/aed_project/build/_deps/raylib_cpp-src/include/Touch.hpp"
    "/home/arthurm/Documentos/AED/Pruebas/mas/aed_project/build/_deps/raylib_cpp-src/include/Vector2.hpp"
    "/home/arthurm/Documentos/AED/Pruebas/mas/aed_project/build/_deps/raylib_cpp-src/include/Vector3.hpp"
    "/home/arthurm/Documentos/AED/Pruebas/mas/aed_project/build/_deps/raylib_cpp-src/include/Vector4.hpp"
    "/home/arthurm/Documentos/AED/Pruebas/mas/aed_project/build/_deps/raylib_cpp-src/include/VrStereoConfig.hpp"
    "/home/arthurm/Documentos/AED/Pruebas/mas/aed_project/build/_deps/raylib_cpp-src/include/Wave.hpp"
    "/home/arthurm/Documentos/AED/Pruebas/mas/aed_project/build/_deps/raylib_cpp-src/include/Window.hpp"
    )
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "/home/arthurm/Documentos/AED/Pruebas/mas/aed_project/build/_deps/raylib_cpp-build/include/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
