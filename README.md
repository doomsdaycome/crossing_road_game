# Crossing Castle Game

![C++](https://img.shields.io/badge/C++-17-blue.svg)
![SFML](https://img.shields.io/badge/SFML-3.0-green.svg)
![CMake](https://img.shields.io/badge/CMake-Build-orange.svg)
![License](https://img.shields.io/badge/License-MIT-brightgreen.svg)

An object-oriented, continuous-scrolling arcade game inspired by Crossy Road. Developed in C++ using the Simple and Fast Multimedia Library (SFML), this project emphasizes scalable software architecture, clean code principles, and efficient memory management.

1. [Introduction](#introduction)
2. [Installation](#installation)
3. [Execution](#execution)
4. [Troubleshoot](#troubleshoot)

## Introduction

Crossing Castle Game is an object-oriented, continuous-scrolling arcade game inspired by the mechanics of Crossy Road. Developed using modern C++17 and Simple and Fast Multimedia Library (SFML 3.0), the project serves as a showcase of scalable software architecture, modular software design, clean code principles, and low-overhead memory management.

### Key Features
* **Dual Gameplay Modes:**
  * **Classic Mode:** Structured, pre-designed levels loaded dynamically from JSON file configurations.
  * **Endless Mode:** Procedurally generated terrain and lanes with progressive difficulty scaling and adaptive movement speeds.
* **Save/Load & State Persistence:** State serialization powered by `nlohmann/json`, allowing players to save ongoing progress and resume at any time.
* **Viewport Culling & Performance Optimization:** Dynamic entity tracking and culling systems that immediately clean up off-screen objects to eliminate memory leaks and sustain high frame rates.
* **Audio & Visual Feedback:** Dynamic asset loading with sound effects, ambient background audio, and smooth visual animations.

### System Architecture & Design Patterns
The application follows enterprise-level game architecture patterns to maximize maintainability and component decoupling:
* **State Machine Pattern:** Manages game transitions across discrete runtime states such as `MenuState`, `PlayState`, `PauseState`, `SettingsState`, and `GameOverState`.
* **Clean Architecture & Separation of Concerns:**
  * `gameplay/`: Encapsulates core mechanics through dedicated management entities (`LaneManager`, `CollisionSystem`, `ScoreManager`).
  * `services/`: Provides low-level infrastructure operations (`ResourceManager`, `LevelRepository`) handling I/O, file loading, and asset caching.
  * `data/`: Contains pure Data Transfer Objects (DTOs) such as `GameSnapshot` and `LevelData` to eliminate circular dependency chains.
* **Singleton Pattern:** Enforces a single global instance for `ResourceManager`, guaranteeing textures, sounds, and fonts are loaded into memory exactly once.

### Project Structure
```text
CROSSING_ROAD_GAME/
├── asset/              # Textures, audio files, fonts, and JSON level data
├── include/            # Header files (.hpp)
│   ├── core/           # Game loop, window management, and global configurations
│   ├── data/           # Data Transfer Objects (LevelData, GameSnapshot)
│   ├── entities/       # Player, Monster, Lane, and Obstacle object models
│   ├── gameplay/       # Collision detection, lane processing, and scoring logic
│   ├── services/       # Resource loading and file I/O operations
│   └── states/         # Game screens (Menu, Play, Pause, Settings, GameOver)
├── source/             # Implementation files (.cpp)
├── CMakeLists.txt      # CMake build configuration script
└── README.md           # Project documentation
```

---

## Installation

### Prerequisites
* **C++ Compiler:** Supporting C++20 standard or higher (GCC, Clang, or MSVC).
* **CMake:** Version 3.10 or higher.
* **SFML:** Version 3.0.x (Must be installed and discoverable by CMake).

### Manual Installation

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

### Visual Studio Code Setup
If you are using Visual Studio Code, you can build and run the project using the official **CMake Tools** extension:

1. Open VS Code and install the **CMake Tools** extension (`ms-vscode.cmake-tools`).
2. Open the project folder in VS Code (`File -> Open Folder...`).
3. Select your C++ compiler kit when prompted by CMake Tools.
4. Click **Build** on the bottom status bar, or press `F7` to build the project.

### Pre-built Executable
For users who do not wish to compile from source:
1. Navigate to the **Releases** section on the official GitHub repository.
2. Download the latest release package for your operating system.
3. Extract the ZIP package to a local directory.

## Execution

### Running from build directory
After building from source using CLI:
```bash
./bin/main
```
*Note: Ensure that the `asset/` folder is placed in the same execution directory as the executable file so all textures and media can be resolved correctly at startup.*

### Running from Visual Studio Code
Press `Shift + F5` or click the **Play / Debug** icon on the VS Code status bar to run the executable directly.

## Troubleshoot

### Common Issues & Solutions

1. **Asset Loading Failures (`Failed to load image/font...`)**
   * **Cause:** The working directory during execution is set incorrectly, causing relative paths to `asset/` to fail.
   * **Solution:** Ensure the `asset/` directory is copied into the binary output folder (e.g., `build/bin/`), or explicitly set the execution path from your terminal to the root directory where `asset/` resides.

2. **SFML Library Not Found During CMake Generation**
   * **Cause:** System environment cannot locate SFML 3.0 installation paths.
   * **Solution:** Specify the location of SFML manually when running CMake:
     ```bash
     cmake -DSFML_DIR="/path/to/sfml/lib/cmake/SFML" ..
     ```

3. **Missing Dynamic Link Libraries (.dll on Windows or .so on Linux)**
   * **Cause:** SFML shared libraries are not present in system PATH or binary folder.
   * **Solution:** Copy required runtime binaries (`sfml-graphics-3.dll`, `sfml-window-3.dll`, `sfml-system-3.dll`, `sfml-audio-3.dll`) into the same directory as the executable file.

## Authors & Contributions

### Authors
* **Huỳnh Gia Đạt**
* **Võ Lê Hoàng**
* **Phạm Bạch Đăng Khoa**
* **Nguyễn Quốc Tuấn**
* **Hoàng Đức Vinh**

### Contributions
This repository is a strict academic project. External pull requests, third-party contributions, or modifications by external developers are strictly prohibited and will be rejected per instructions from our course mentor and lecturer.
