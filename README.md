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

cmake -S . -B "build" -G "Visual Studio 17 2022" -D "CMAKE_PREFIX_PATH=D:/Qt/5.15.2/msvc2019_64"