#include "Grid.h"
#include "raylib.h"

Grid::Grid(int width, int height) 
    : width(width), height(height),
      cells(height, std::vector<bool>(width, false)),
      nextCells(height, std::vector<bool>(width, false)) {
}

void Grid::update() {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int neighbors = countNeighbors(x, y);
            
            if (cells[y][x]) {
                nextCells[y][x] = (neighbors == 2 || neighbors == 3);
            } else {
                nextCells[y][x] = (neighbors == 3);
            }
        }
    }
    
    cells = nextCells;
}

void Grid::clear() {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            cells[y][x] = false;
        }
    }
}

void Grid::randomSeed(float density) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            cells[y][x] = (GetRandomValue(0, 100) < (int)(density * 100));
        }
    }
}

bool Grid::getCell(int x, int y) const {
    if (x < 0 || x >= width || y < 0 || y >= height) {
        return false;
    }
    return cells[y][x];
}

void Grid::setCell(int x, int y, bool alive) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        cells[y][x] = alive;
    }
}

int Grid::countAliveCells() const {
    int count = 0;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (cells[y][x]) count++;
        }
    }
    return count;
}

int Grid::countNeighbors(int x, int y) const {
    int count = 0;
    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            if (dx == 0 && dy == 0) continue;
            
            int nx = x + dx;
            int ny = y + dy;
            
            if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                if (cells[ny][nx]) count++;
            }
        }
    }
    return count;
}
