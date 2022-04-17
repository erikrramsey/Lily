# Lily

A 3D game engine for Windows built on top of OpenGL and SDL.

![](https://github.com/erikrramsey/Lily/blob/master/screenshot1.jpg)

Models used under CC license: https://skfb.ly/orPGW https://skfb.ly/otpMF 
## Features

- Scene editor gui designed to load and display 3D models, and arrange them in a hierarchy.
- An Entity Component System built from scratch designed to promote data locality.

## Planned Features

- Scripting support
- Scene serialization
- Scene editor and runtime being separated
- More advanced graphics rendering technique suppoprt

# Build Instructions

## Visual Studio 2019
- Open powershell or cmd from the Lily directory and run the following commands:
```
git submodule update
cd vendor/imgui
git checkout docking
```

- Download [C++ CMake Tools for Visual Studio](https://docs.microsoft.com/en-us/cpp/build/cmake-projects-in-visual-studio?view=msvc-170)
- Open project folder in Visual Studio
- Right click cmakelists and click "Configure Lily Application"
