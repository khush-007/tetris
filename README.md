# 🧩 Tetris Game in C++

## Contributors

* Rutva Mehta
* Khush Hingrajiya
* Manthan Kanetiya
* Vikas Bhabhor

---

# Overview

A console-based implementation of the classic **Tetris Game** developed using **C++**, **Object-Oriented Programming (OOP)** principles, and the **Windows Console API**.

The game features multiple Tetromino pieces, rotation mechanics, collision detection, line-clearing algorithms, score tracking, and smooth rendering through a custom double-buffered console renderer.

---

# Features

* 🎮 Classic Tetris gameplay
* 🧱 Seven Tetromino shapes (I, O, T, S, Z, J, L)
* 🔄 Piece rotation system
* 🚀 Hard Drop functionality
* 🎯 Collision detection
* 🏆 Score and High Score tracking
* 🎨 Colored Tetromino rendering
* ⌨️ WASD and Arrow Key controls
* 🖥️ Double-buffered console rendering
* 👤 Personalized player profile
* 🔁 Restart and Exit options

---

# OOP Concepts Used

## Classes and Objects

The project is organized into multiple classes:

* `Game`
* `Board`
* `Tetromino`
* `ConsoleRenderer`

---

## Encapsulation

Each class manages its own data and behavior independently.

Examples:

* Board logic is encapsulated inside `Board`
* Rendering logic is encapsulated inside `ConsoleRenderer`
* Tetromino behavior is encapsulated inside `Tetromino`

---

## Abstraction

Complex operations such as rendering, collision detection, and line clearing are hidden behind clean interfaces.

---

## Composition

The `Game` class combines:

* Board
* Renderer
* Tetromino

to build the complete game.

---

# Skills Demonstrated

* C++
* Object-Oriented Programming (OOP)
* Game Development
* Windows API
* Collision Detection
* Real-Time Input Handling
* Console Rendering
* Software Design
* Problem Solving
* Debugging

---

# Technologies Used

* C++
* Standard Template Library (STL)
* Windows API (`windows.h`)
* Console Graphics
* OOP Concepts

---

# Data Structures Used

## Vectors

Used extensively for:

* Tetromino shapes
* Board storage
* Screen buffer management

Examples:

* `vector<vector<int>>`
* `vector<vector<char>>`
* `vector<CHAR_INFO>`

---

## Strings

Used for:

* UI messages
* Player information
* Rendering text

---

## Enumerations

Used internally for managing game logic and controls.

---

# Controls

| Key   | Action       |
| ----- | ------------ |
| ← / A | Move Left    |
| → / D | Move Right   |
| ↓ / S | Move Down    |
| ↑ / W | Rotate Piece |
| Space | Hard Drop    |

---

# Game Mechanics

1. Random Tetromino pieces spawn at the top of the board.
2. Players can move and rotate pieces.
3. Completed horizontal lines are cleared.
4. Each cleared line increases the score.
5. The game ends when a new Tetromino cannot be placed.
6. High scores are tracked during the session.

---

# Tetromino Shapes

The game supports all standard Tetris pieces:

* I Piece
* O Piece
* T Piece
* S Piece
* Z Piece
* J Piece
* L Piece

---

# How to Run

## Compile

```bash
g++ tetris.cpp -o tetris.exe
```

## Run

```bash
./tetris.exe
```

---

# Dependencies

* Windows Operating System
* MinGW / GCC Compiler
* Visual Studio (optional)

---

# Project Architecture

```text
Game
│
├── ConsoleRenderer
│
├── Board
│
└── Tetromino
```

This modular structure improves maintainability and scalability.

---

# Known Limitations

* Windows-only implementation
* High scores are not persisted after program exit
* No difficulty selection
* No sound effects

---

# Future Enhancements

* Persistent High Score Storage
* Difficulty Levels
* Pause/Resume Feature
* Ghost Piece Preview
* Next Piece Preview
* Hold Piece Functionality
* Sound Effects
* Cross-Platform Support
* Improved Graphics

---

# Learning Outcomes

Through this project, we gained practical experience with:

* Object-Oriented Programming
* Game Loop Design
* Collision Detection Algorithms
* Console Rendering Techniques
* Windows API Programming
* Software Architecture
* Real-Time User Input Handling

---

# License

This project is open-source and intended for educational and learning purposes.

---

⭐ If you like this project, consider giving it a star on GitHub!
