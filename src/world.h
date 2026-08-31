// Copyright (c) 2026, FarzanHajian
// Licensed under the BSD 3-Clause License. See LICENSE file in the project root.

#ifndef _WORLD_H_
#define _WORLD_H_

#include <array>
#include <bitset>
#include <cstdlib>
#include <random>

template <int ROWS, int COLS>
class World {
   public:
    using World_t = std::bitset<ROWS * COLS>;
    using Neighbors_t = std::array<int, 8>;

   private:
    World_t _world;
    std::mt19937 _rnd;
    std::vector<int> toBeAdded;
    std::vector<int> toBeRemoved;

   public:
    World() {
        static_assert(ROWS >= 50, "The minimum row count is 50");
        static_assert(COLS >= 100, "The minimum column count is 100");

        std::random_device dev;
        _rnd = std::mt19937(dev());

        toBeAdded.reserve(50);
        toBeRemoved.reserve(50);
    }

    World_t getWorld() {
        return _world;
    }

    void Reset() {
        _world.reset();
        AddInitialPattern();
    }

    void Next() {
        toBeAdded.clear();
        toBeRemoved.clear();
        int neighbors[8] = {-1};

        const int TOP_LEFT = 0;
        const int TOP_RIGHT = COLS - 1;
        const int BOTTOM_LEFT = COLS * ROWS - COLS;
        const int BOTTOM_RIGHT = COLS * ROWS - 1;
        const int FIRST_ROW = 0;
        const int LAST_ROW = ROWS - 1;
        const int FIRST_COL = 0;
        const int LAST_COL = COLS - 1;

        int row = 0, col = 0;
        int above = -COLS, below = COLS;
        for (size_t cell = 0; cell < _world.size(); cell++) {
            if (cell == TOP_LEFT) {
                neighbors[4] = 1;
                neighbors[6] = COLS;
                neighbors[7] = COLS + 1;
            } else if (cell == TOP_RIGHT) {
                neighbors[3] = TOP_RIGHT - 1;
                neighbors[5] = TOP_RIGHT + COLS - 1;
                neighbors[6] = TOP_RIGHT + COLS;
            } else if (cell == BOTTOM_LEFT) {
                neighbors[1] = BOTTOM_LEFT - COLS;
                neighbors[2] = BOTTOM_LEFT - COLS + 1;
                neighbors[4] = BOTTOM_LEFT + 1;
            } else if (cell == BOTTOM_RIGHT) {
                neighbors[0] = BOTTOM_RIGHT - COLS - 1;
                neighbors[1] = BOTTOM_RIGHT - COLS;
                neighbors[3] = BOTTOM_RIGHT - 1;
            } else if (row == FIRST_ROW) {
                neighbors[3] = cell - 1;
                neighbors[4] = cell + 1;
                neighbors[5] = below - 1;
                neighbors[6] = below;
                neighbors[7] = below + 1;
            } else if (row == LAST_ROW) {
                neighbors[0] = above - 1;
                neighbors[1] = above;
                neighbors[2] = above + 1;
                neighbors[3] = cell - 1;
                neighbors[4] = cell + 1;
            } else if (col == FIRST_COL) {
                neighbors[1] = above;
                neighbors[2] = above + 1;
                neighbors[4] = cell + 1;
                neighbors[6] = below;
                neighbors[7] = below + 1;
            } else if (col == LAST_COL) {
                neighbors[0] = above - 1;
                neighbors[1] = above;
                neighbors[3] = cell - 1;
                neighbors[5] = below - 1;
                neighbors[6] = below;
            } else {
                neighbors[0] = above - 1;
                neighbors[1] = above;
                neighbors[2] = above + 1;
                neighbors[3] = cell - 1;
                neighbors[4] = cell + 1;
                neighbors[5] = below - 1;
                neighbors[6] = below;
                neighbors[7] = below + 1;
            }

            int neighborCount = 0;
            for (const int n : neighbors) {
                if (n == -1) continue;
                neighborCount += _world.test(n) ? 1 : 0;
            }

            if (_world.test(cell)) {
                if (neighborCount < 2 || neighborCount > 3) toBeRemoved.push_back(cell);
            } else {
                if (neighborCount == 3) toBeAdded.push_back(cell);
            }

            above++;
            below++;

            col++;
            if (col == COLS) {
                col = 0;
                row++;
            }
        }

        for (const int cell : toBeAdded)
            _world.set(cell);
        for (const int cell : toBeRemoved)
            _world.reset(cell);
    }

   private:
    void AddInitialPattern() {
        int initialPatternSize = 20;
        int cellCount = initialPatternSize * initialPatternSize;
        int xPadding = COLS / 4;
        int yPadding = ROWS / 4;
        std::uniform_int_distribution<int> rowDist(yPadding, ROWS - yPadding - 1);
        std::uniform_int_distribution<int> colDist(xPadding, COLS - xPadding - 1);
        std::uniform_int_distribution<int> countDist(cellCount / 4, cellCount / 2);
        std::uniform_int_distribution<int> cellDist(0, cellCount - 1);

        int startingRow = rowDist(_rnd);
        int startingCol = colDist(_rnd);
        int startingCount = countDist(_rnd);
        int startingPoint = ((startingRow - 1) * COLS) + startingCol;
        for (int i = 0; i < startingCount; i++) {
            while (true) {
                int cell = cellDist(_rnd);
                auto [quotient, remainder] = std::div(cell, initialPatternSize);
                int offset = (remainder == 0 ? initialPatternSize : remainder);
                int index = startingPoint + (quotient * COLS) + offset;
                if (!_world.test(index)) {
                    _world.set(index);
                    break;
                }
            }
        }
    }
};

#endif  // _WORLD_H_
