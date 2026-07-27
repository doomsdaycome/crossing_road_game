# 🐸 Crossing Road Game

![C++](https://img.shields.io/badge/C++-17-blue.svg)
![SFML](https://img.shields.io/badge/SFML-3.0-green.svg)
![CMake](https://img.shields.io/badge/CMake-Build-orange.svg)
![License](https://img.shields.io/badge/License-MIT-brightgreen.svg)

An object-oriented, continuous-scrolling arcade game inspired by Crossy Road. Developed in C++ using the Simple and Fast Multimedia Library (SFML), this project emphasizes scalable software architecture, clean code principles, and efficient memory management.

## ✨ Features
* **Dual Game Modes:** 
  * *Classic Mode:* Pre-designed levels loaded dynamically from JSON files.
  * *Endless Mode:* Procedurally generated lanes with increasing difficulty and adaptive speed.
* **Robust Save/Load System:** State serialization using `nlohmann/json` to save and resume game progress.
* **Optimized Rendering:** Culling system that dynamically removes off-screen entities to prevent memory leaks and maintain high FPS.

## 🏗️ Software Architecture & Design Patterns
This project is structured with Enterprise-level Game Development standards in mind:
* **State Machine Pattern:** Decouples game phases (`PlayState`, `MenuState`, `GameOverState`) for seamless transitions.
* **Separation of Concerns (Clean Architecture):**
  * `gameplay/`: Core mechanics isolated into dedicated managers (`LaneManager`, `CollisionSystem`, `ScoreManager`).
  * `services/`: Low-level system operations (`ResourceManager`, `LevelRepository`) handling I/O and Asset Caching.
  * `data/`: Pure Data Transfer Objects (DTOs) like `GameSnapshot` to eliminate circular dependencies.
* **Singleton Pattern:** Global `ResourceManager` to ensure textures and fonts are loaded into memory exactly once.

## 📂 Project Structure
```text
CROSSING_ROAD_GAME/
├── asset/              # Textures, Fonts, and JSON level data
├── include/            # Header files (.hpp)
│   ├── core/           # Game loop and global configurations
│   ├── data/           # DTOs (LevelData, GameSnapshot)
│   ├── entities/       # Player, Monster, Lane objects
│   ├── gameplay/       # Collision, Lane, and Score logic
│   ├── services/       # Resource & File I/O Managers
│   └── states/         # Game screens (Menu, Play, Settings)
├── source/             # Implementation files (.cpp)
└── CMakeLists.txt      # CMake build configuration
```

## 🚀 Getting Started
Prerequisites
* C++ Compiler: Supports C++17 standard or higher.
* CMake: Version 3.10 or higher.
* SFML: Version 3.0.x (Ensure SFML is installed and discoverable by CMake).

Build Instructions (Windows / Linux)
1. Clone the repository:
```bash
git clone https://github.com/YinceIuCode/Crossing-Road-Game.git
cd crossing-road-game
```
2. Generate build files using CMake:
```bash
mkdir build
cd build
cmake ..
```
3. Compile the project:
```bash
cmake --build .
```
4. Run the executable generated in the build or bin directory. (Note: Ensure the asset/ folder is in the same directory as the executable).

## 📜 License
This project is licensed under the MIT License - see the LICENSE file for details.

## 👨‍💻 Author
Hoàng Đức Vinh
Undergraduate Student (Information Technology)
*University of Science, VNU-HCM (HCMUS)*
