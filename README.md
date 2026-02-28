# Engine & Burger Time

This repository contains a custom 2D component-based game engine written in C++, and a recreation of the classic 1982 arcade game **Burger Time** built on top of it. 

This project was developed as part of the Programming 4 assignment at Howest Digital Arts and Entertainment (DAE) by Wannes De Tollenaere.

## 🌍 Web
Play the game in a your web browser [here](https://wannesdetollenaere.github.io/2DAE19_DeTollenaere_Wannes_Programming4/)

<img width="400" height="400" alt="image" src="https://github.com/user-attachments/assets/da00eef0-9061-4bdc-ad27-23670a8f3068" />


## 🛠️ Engine Features

The engine is a lightweight but robust 2D game engine built with modern C++ and SDL3. It is designed around flexibility and data-driven architecture.

* **Component-Based Architecture:** Game objects are purely structural containers. Behavior and rendering are defined by attaching specific modular components (`TextureComponent`, `AnimatorComponent`, `RotatorComponent`, `TextComponent`, etc.).
* **Data-Driven Scene Loading:** Levels and scenes are completely constructed via JSON files using the `nlohmann/json` library, allowing for rapid level design and tweaking without needing to recompile C++ code. Supports hierarchical parent-child relationships natively.
* **Cross-Platform:** Uses CMake for cross-platform builds. Includes toolchains for standard desktop builds and WebAssembly (via Emscripten).
* **MORE TO BE ADDED**

## 🍔 The Game: Burger Time

The engine powers a functional clone of Data East's **Burger Time**.

## 💻 Technologies & Libraries

* **Language:** C++20
* **Graphics/Windowing:** [SDL3](https://github.com/libsdl-org/SDL) & [SDL3_ttf](https://github.com/libsdl-org/SDL_ttf)
* **Math:** [GLM](https://github.com/g-truc/glm)
* **UI/Debugging:** [Dear ImGui](https://github.com/ocornut/imgui)
* **Data Parsing:** [nlohmann/json](https://github.com/nlohmann/json)
* **Build System:** CMake

## How to Build

This project uses CMake. You can build it using the command line or an IDE that supports CMake (like Visual Studio 2022, CLion, or VS Code).

### Prerequisites
* CMake (3.20 or higher)
* A C++20 compatible compiler (MSVC, GCC, Clang)
* Git 

### Build Instructions

1. **Clone the repository:**
   ```bash
   git clone https://github.com/wannesdetollenaere/2dae19_detollenaere_wannes_programming4.git
