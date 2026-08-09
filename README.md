# pong-

![Project Status](https://img.shields.io/badge/status-active-success)
![Stars](https://img.shields.io/github/stars/ShashwatAryal/pong-?style=flat)
![Forks](https://img.shields.io/github/forks/ShashwatAryal/pong-?style=flat)
![License](https://img.shields.io/github/license/ShashwatAryal/pong-?style=flat)

## Project Title & Description

**pong-** is a classic arcade-style Pong game implemented in C++. This repository contains the source code for a simple yet engaging two-dimensional sports game where players control paddles to hit a ball back and forth, aiming to score points by getting the ball past the opponent's paddle. The project structure suggests an object-oriented approach, likely featuring both a player-controlled paddle and an AI opponent.

## Key Features & Benefits

-   **Classic Pong Gameplay**: Replicates the timeless fun of the original Pong game.
-   **C++ Implementation**: Developed purely in C++, offering a strong foundation for understanding game development concepts.
-   **Object-Oriented Design**: Utilizes classes like `Ball`, `PlayerPaddle`, and `AIPaddle` for clear, modular, and maintainable code.
-   **AI Opponent**: Features an Artificial Intelligence opponent for single-player challenges (implied by `AIPaddle.cpp`).
-   **Simple & Engaging**: Provides a straightforward and enjoyable gaming experience.
-   **Educational Value**: Excellent project for learning C++ game development, graphics programming fundamentals, and basic physics simulation.

## Prerequisites & Dependencies

Before you can build and run this project, you'll need the following installed on your system:

-   **C++ Compiler**: A C++11 (or newer) compatible compiler.
    -   For Windows: [MinGW-w64](http://mingw-w64.org/doku.php) (implied by `libgcc_s_dw2-1.dll`, `libstdc++-6.dll` in the structure).
    -   For Linux/macOS: [g++](https://gcc.gnu.org/) (usually included with developer tools).
-   **`make` utility**: A build automation tool (often included with compilers or developer tools).
-   **Graphics Library (Likely)**: While not explicitly listed in `dll`s, a graphical game like Pong typically requires a graphics library. Common choices include:
    -   [SFML](https://www.sfml-dev.org/) (Simple and Fast Multimedia Library)
    -   [SDL2](https://www.libsdl.org/) (Simple DirectMedia Layer)
    You may need to install the development headers for your chosen graphics library (e.g., `libsfml-dev` on Linux, or SFML SDK on Windows).

## Installation & Setup Instructions

Follow these steps to get the project up and running on your local machine.

### 1. Clone the Repository

First, clone the `pong-` repository to your local machine using Git:

```bash
git clone https://github.com/ShashwatAryal/pong-.git
cd pong-
```

### 2. Build the Project

This project uses a `Makefile` for compilation.

```bash
make
```

This command will compile the C++ source files and link them into an executable. If you encounter errors, ensure your C++ compiler and any necessary graphics library dependencies (and their headers) are correctly installed and configured for your system.

### 3. (Optional) Open in VS Code

If you use Visual Studio Code, you can open the project folder to leverage the provided `.vscode` configurations for development:

```bash
code .
```

The `.vscode/` directory contains configuration files (`c_cpp_properties.json`, `launch.json`, `settings.json`, `tasks.json`) to help with C++ IntelliSense, building, and debugging within VS Code.

## Usage Examples

After successfully building the project, you can run the game from your terminal.

### Running the Game

```bash
./pong-
```

(The exact executable name might vary based on your `Makefile` or OS, e.g., `pong-.exe` on Windows).

### Controls

Once the game is running, typical Pong controls are:

-   **Player Paddle**:
    -   `W` / `Up Arrow`: Move paddle up
    -   `S` / `Down Arrow`: Move paddle down
-   **AI Paddle**: Automatically controlled.

The objective is to hit the ball past your opponent's paddle to score a point.

## Configuration Options

This project does not currently expose external configuration files (e.g., `.ini`, `.json`). Any configurable settings, such as:

-   Game speed
-   Paddle speed
-   Window dimensions
-   Ball speed
-   Scoring limits

are likely hardcoded within the C++ source files (e.g., `main.cpp`, `Ball.cpp`, `AIPaddle.cpp`, `PlayerPaddle.cpp`). To modify these, you would need to edit the relevant source code and recompile the project.

## Contributing Guidelines

We welcome contributions to the `pong-` project! If you'd like to contribute, please follow these guidelines:

1.  **Fork the repository**: Click the "Fork" button at the top right of this page to create your copy.
2.  **Clone your forked repository**:
    ```bash
    git clone https://github.com/YOUR_USERNAME/pong-.git
    cd pong-
    ```
3.  **Create a new branch**: Choose a descriptive name for your branch (e.g., `feature/add-score-display`, `bugfix/paddle-collision`).
    ```bash
    git checkout -b your-feature-branch
    ```
4.  **Make your changes**: Implement your feature, fix a bug, or improve documentation.
5.  **Commit your changes**: Write clear and concise commit messages.
    ```bash
    git add .
    git commit -m "feat: Add score display to game UI"
    ```
6.  **Push to your fork**:
    ```bash
    git push origin your-feature-branch
    ```
7.  **Open a Pull Request**: Go to the original `pong-` repository on GitHub and click the "New pull request" button. Provide a detailed description of your changes.

Please ensure your code adheres to the existing coding style and includes appropriate comments.

## License Information

This project is licensed under the **MIT License**. You are free to use, modify, and distribute this software under the terms specified in the `LICENSE.txt` file.

For the full license text, please see the [LICENSE.txt](LICENSE.txt) file in this repository.

## Acknowledgments

-   **ShashwatAryal**: The owner and primary developer of this project.
-   Inspired by the classic arcade game Pong, originally released by Atari.