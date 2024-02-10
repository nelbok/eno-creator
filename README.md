# Eno Creator

## About

Map editor, wavefront object export
You can easily create something with this application, it might look like Minecraft in a way.
And you can export your work into Wavefront file.

Eno features:
- version
- material
- bounding box
- object

Wavefront features:
- vertex / triangle
- diffuse color

![Interface](sample/preview.png)
![OpenGL preview](sample/preview-opengl.png)

## History

The project started in 2011, it was primarily a presentation software.
Even if now it could be used to make old school video games.
So the interface could look old.

## Requirements

- CMake 3.24+
- C++
- Qt 5.15.2 or Qt6

## Compilation

- cmake -D "CMAKE_PREFIX_PATH=Path to Qt" -B "build" .
- cmake --build build

Path to Qt, example for Windows: C:/Qt/6.6.1/msvc2019_64
