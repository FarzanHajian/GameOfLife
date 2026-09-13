# Conway's Game of Life

A small C++ implementation of **Conway's Game of Life**, using [Raylib](https://www.raylib.com/) for rendering.

The project is primarily an experiment with C++, Raylib, and efficient representation and rendering of a cellular automaton.

## Overview

The simulation consists of a two-dimensional grid of cells. Each cell has one of two states:

- **Alive**
- **Dead**

The world evolves in discrete generations according to the standard Conway's Game of Life rules.

The current world size is:

- **100 rows**
- **200 columns**

The simulation advances once every **250 ms**.

## Game of Life Rules

For each cell, the number of its eight neighboring cells is counted.

### A live cell

- Dies from **underpopulation** if it has fewer than 2 live neighbors.
- Survives if it has **2 or 3** live neighbors.
- Dies from **overpopulation** if it has more than 3 live neighbors.

### A dead cell

- Becomes alive if it has exactly **3** live neighbors.

All cells are evaluated based on the state of the previous generation.

## Architecture

The project separates the simulation state from rendering.

```text
                 Main Loop
                     │
          ┌──────────┴──────────┐
          │                     │
      Every 250 ms          Every frame
          │                     │
          ▼                     ▼
      World::Next()       DrawTexture()
          │                     │
          ▼                     │
     World state                │
       changes                  │
          │                     │
          ▼                     │
       redraw                   │
          │                     │
          ▼                     │
    RenderTexture2D             │
          │                     │
     ┌────┴─────┐               │
     │          │               │
 DrawGrid()  DrawWorld()        │
     │          │               │
     └────┬─────┘               │
          │                     │
          ▼                     │
      Cached image ─────────────┘
```

The rendered world is cached in a `RenderTexture2D`. The grid and cells are redrawn only when the world changes or the window is resized.

## World Representation

The world is represented using `std::bitset`.

Each bit represents one cell:

Cells are accessed using a flattened index:

```cpp
size_t index = row * COLS + col;
```

## Rendering

Raylib is used for the graphical output.

The application renders into a `RenderTexture2D`:

```cpp
BeginTextureMode(worldTexture);

ClearBackground(RAYWHITE);

DrawGrid();
DrawWorld(world);

EndTextureMode();
```

Live cells are drawn with a horizontal gradient fill (blue → dark blue, via `DrawRectangleGradientH`) rather than a flat color.

The resulting texture is then displayed during normal frames:

```cpp
BeginDrawing();

DrawTextureRec(...);

EndDrawing();
```

This avoids repeatedly drawing every grid line and every cell when the simulation state hasn't changed.

## Timing

Rendering runs at a target of 60 FPS:

```cpp
SetTargetFPS(60);
```

The simulation itself advances every 250 ms:

```cpp
const double UPDATE_INTERVAL = 0.250;
```

These are intentionally independent:

```text
Rendering:    ~60 FPS
Simulation:    4 generations/sec
```

This allows the rendering loop to remain responsive while the simulation advances at a slower, controlled rate.

## Initial State

The simulation currently starts with a small population placed in a **20 × 20 initial area**, seeded at a random location anywhere on the grid.

The intention is to experiment with different initial configurations and find patterns that produce interesting, long-lived simulations rather than quickly dying out or becoming static.

The same random-pattern logic is reused at runtime — see [Controls](#controls) below.

## Controls

- **Space** — pause or resume the simulation.
- **Right-click** on the grid — seed a new random 20 × 20 pattern anchored at the clicked cell (its top-left corner), using the same logic that generates the initial population.
- **A** — toggle Auto Mode. While enabled, a new random 20 × 20 pattern is automatically seeded at a random location every ~200 simulation steps, and an "Auto Mode: ON" indicator is shown on screen.

## Building

The project uses **CMake**. Raylib is vendored in-repo as a git submodule under `external/raylib`, so it must be checked out before configuring.

Example:

```bash
git submodule update --init --recursive
cmake --preset debug [or release or debug-wayland or release-wayland]
cmake --build build
```

## Dependencies

- C++20
- CMake 3.15+
- Raylib 6.x — included as a git submodule (`external/raylib`) and built from source as part of the CMake build, not installed separately

The project has been developed and tested on
- Windows 11 + Visual Studio's C++ toolchain + Ninja + VS Code
- Fedora 44 KDE + GCC + Ninja + VS Code
