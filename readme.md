# Snake Game in C++

A classic Snake game built in C++ for the Windows console.

## Features

- Smooth console rendering using Windows API
- Score tracking with final score display on game over
- Progressive difficulty — snake speeds up as your score increases
- Self-collision and wall-collision detection
- Food never spawns on the snake's body

## Requirements

- Windows OS
- C++ compiler (MinGW / MSVC / Visual Studio)

## How to Compile

Using g++ (MinGW):
```bash
g++ snake.cpp -o snake.exe
```

Using MSVC:
```bash
cl snake.cpp
```

## How to Run

```bash
./snake.exe
```

## Controls

| Key | Action |
|-----|--------|
| W | Move Up |
| A | Move Left |
| S | Move Down |
| D | Move Right |

## Gameplay

- Guide the snake to eat food (`o`)
- Each food eaten increases your score by 1 and grows the snake
- The snake speeds up every 5 points
- Game ends if the snake hits a wall or its own body
- Final score is displayed on game over

## Game Speed Scaling

| Score | Speed |
|-------|-------|
| 0 | 100ms per frame (slowest) |
| 5 | 75ms per frame |
| 10+ | 50ms per frame (fastest) |

## Project Structure

```
snake.cpp       — Main source file containing all game logic
README.md       — This file
```

## Classes Overview

- `Point` — Represents a 2D coordinate on the console
- `Snake` — Handles snake body, movement, direction, and growth
- `Board` — Manages game state, food spawning, rendering, and input

## Known Limitations

- Windows only — uses `windows.h`, `conio.h`, and `Sleep()`
- Console window size is detected at startup; resizing during gameplay is not supported
- Maximum snake length is capped at 1000 segments