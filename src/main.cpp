// Copyright (c) 2026, FarzanHajian
// Licensed under the BSD 3-Clause License. See LICENSE file in the project root.

#include <raylib.h>
#include <cstdlib>
#include <iostream>
#include "world.h"

const int WORLD_ROWS = 100;
const int WORLD_COLS = 200;
const double UPDATE_INTERVAL = 0.250;  // 250 ms
const int AUTO_MODE_INTERVAL = 200;
const Color GRID_COLOR = LIGHTGRAY;
const Color MAIN_COLOR = BLUE;
const Color SECONDARY_COLOR = DARKBLUE;

bool isPaused = false;
bool isAutoMode = false;

using AppWorld = World<WORLD_ROWS, WORLD_COLS>;

void DrawGrid(float &, float &);
void DrawWorld(AppWorld &, const float &, const float &);

int main() {
    AppWorld world;
    world.Reset();

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 600, "Game of Life");
    MaximizeWindow();

    SetTargetFPS(60);

    RenderTexture2D worldTexture = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
    bool redraw = true;
    double lastUpdate = GetTime();
    int iteration = 0;
    while (!WindowShouldClose()) {
        double now = GetTime();

        if (now - lastUpdate >= UPDATE_INTERVAL) {
            lastUpdate += UPDATE_INTERVAL;
            if (!isPaused) {
                world.Next();
                redraw = true;
            }
        }

        if (IsWindowResized()) {
            UnloadRenderTexture(worldTexture);
            worldTexture = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
            redraw = true;
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
            Vector2 mouse = GetMousePosition();

            int width = GetScreenWidth();
            int height = GetScreenHeight();

            float rowSpan = height / static_cast<float>(WORLD_ROWS);
            float colSpan = width / static_cast<float>(WORLD_COLS);

            int row = static_cast<int>(mouse.y / rowSpan);
            int col = static_cast<int>(mouse.x / colSpan);

            world.AddInitialPattern(row, col);

            redraw = true;
        }

        if (IsKeyReleased(KEY_SPACE)) {
            isPaused = !isPaused;
        }

        if (IsKeyReleased(KEY_A)) {
            isAutoMode = !isAutoMode;
        }

        if (redraw) {
            iteration++;

            if (isAutoMode && !(iteration % AUTO_MODE_INTERVAL)) {
                world.AddInitialPattern();
            }

            float rowSpan, colSpan;
            BeginTextureMode(worldTexture);
            ClearBackground(RAYWHITE);
            DrawGrid(rowSpan, colSpan);
            DrawWorld(world, rowSpan, colSpan);
            EndTextureMode();

            redraw = false;
        }

        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawTextureRec(worldTexture.texture,
                       {0, 0, (float)worldTexture.texture.width, -(float)worldTexture.texture.height},
                       {0, 0},
                       WHITE);

        DrawText(TextFormat("Iteration: %d", iteration), 10, 10, 20, MAIN_COLOR);
        if (isAutoMode) DrawText("Auto Mode: ON", 10, 40, 20, MAIN_COLOR);

        EndDrawing();
    }

    CloseWindow();
}

void DrawGrid(float &rowSpan, float &colSpan) {
    int height = GetScreenHeight();
    int width = GetScreenWidth();

    rowSpan = height / static_cast<float>(WORLD_ROWS);
    colSpan = width / static_cast<float>(WORLD_COLS);

    for (int row = 0; row <= WORLD_ROWS; ++row) {
        int y = static_cast<int>(row * rowSpan);
        DrawLine(0, y, width, y, GRID_COLOR);
    }

    for (int col = 0; col <= WORLD_COLS; ++col) {
        int x = static_cast<int>(col * colSpan);
        DrawLine(x, 0, x, height, GRID_COLOR);
    }
}

void DrawWorld(AppWorld &world, const float &rowSpan, const float &colSpan) {
    const auto &data = world.getWorld();

    for (size_t i = 0; i < data.size(); i++) {
        if (data[i]) {
            auto [quotient, remainder] = std::div(i, WORLD_COLS);
            float x = remainder * colSpan;
            float y = quotient * rowSpan;
            DrawRectangleGradientH(x, y, colSpan + 1, rowSpan + 1, MAIN_COLOR, SECONDARY_COLOR);
        }
    }
}