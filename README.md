# Eno Creator

## Introduction

Wavefront map editor
You can easily create something with this application, it might look like Minecraft in a way.
And you can export your work into Wavefront file.

Features:
- vertex
- faces
- color

Not supported:
- normals
- textures
- complex material

![Interface](sample/preview.png)
![OpenGL preview](sample/preview-opengl.png)

## History & Requirements

The project began in 2011, so many features look old.
Updated in 2022 with:
- CMake 3.5
- C++17
- VS 2022
- Qt 5.15.2

Warning: Might not working on macOS or Linux.

## Compilation

Example with Qt5 and Visual Studio 2022 on Windows:
- cmake -S . -B "build5" -G "Visual Studio 17 2022" -D "CMAKE_PREFIX_PATH=D:/Qt/5.15.2/msvc2019_64"
- cmake --build build5

Example with Qt6 and Visual Studio 2022 on Windows:
- cmake -S . -B "build6" -G "Visual Studio 17 2022" -D "CMAKE_PREFIX_PATH=D:/Qt/6.6.1/msvc2019_64"
- cmake --build build6

Example on Linux when you already have either Qt5 or Qt6 installed:
- cmake -S . -B "build"
- cmake --build build
