#include "raylib.h"
#include <vector>
#include <deque>
#include <cstdlib>
#include <ctime>
#include <cstdio>

const int GRID_WIDTH = 80;
const int GRID_HEIGHT = 60;
const int CELL_SIZE = 10;
const int SCREEN_WIDTH = GRID_WIDTH * CELL_SIZE;
const int SCREEN_HEIGHT = GRID_HEIGHT * CELL_SIZE;

enum GameState {
    MAIN_MENU,
    GAME,
    WIN_SCREEN
};

int countNeighbors(const std::vector<std::vector<bool>>& grid, int x, int y) {
    int count = 0;
    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            if (dx == 0 && dy == 0) continue;
            
            int nx = x + dx;
            int ny = y + dy;
            
            if (nx >= 0 && nx < GRID_WIDTH && ny >= 0 && ny < GRID_HEIGHT) {
                if (grid[ny][nx]) count++;
            }
        }
    }
    return count;
}

void updateGrid(std::vector<std::vector<bool>>& grid, std::vector<std::vector<bool>>& nextGrid) {
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            int neighbors = countNeighbors(grid, x, y);
            
            if (grid[y][x]) {
                nextGrid[y][x] = (neighbors == 2 || neighbors == 3);
            } else {
                nextGrid[y][x] = (neighbors == 3);
            }
        }
    }
    
    grid = nextGrid;
}

void randomSeed(std::vector<std::vector<bool>>& grid, float density = 0.3f) {
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            grid[y][x] = (GetRandomValue(0, 100) < (int)(density * 100));
        }
    }
}

void clearGrid(std::vector<std::vector<bool>>& grid) {
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            grid[y][x] = false;
        }
    }
}

int countAliveCells(const std::vector<std::vector<bool>>& grid) {
    int count = 0;
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            if (grid[y][x]) count++;
        }
    }
    return count;
}

bool drawButton(const char* text, int x, int y, int width, int height, bool isHovered) {
    Color buttonColor = isHovered ? DARKGRAY : GRAY;
    DrawRectangle(x, y, width, height, buttonColor);
    DrawRectangleLines(x, y, width, height, WHITE);
    
    int textWidth = MeasureText(text, 30);
    DrawText(text, x + (width - textWidth) / 2, y + (height - 30) / 2, 30, WHITE);
    
    return isHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

bool isMouseOver(int x, int y, int width, int height) {
    Vector2 mousePos = GetMousePosition();
    return mousePos.x >= x && mousePos.x <= x + width &&
           mousePos.y >= y && mousePos.y <= y + height;
}

int main() {
    SetExitKey(0);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "BitBloom");
    SetTargetFPS(60);
    
    srand(time(NULL));
    SetRandomSeed(time(NULL));
    
    std::vector<std::vector<bool>> grid(GRID_HEIGHT, std::vector<bool>(GRID_WIDTH, false));
    std::vector<std::vector<bool>> nextGrid(GRID_HEIGHT, std::vector<bool>(GRID_WIDTH, false));
    
    GameState currentState = MAIN_MENU;
    int frameCounter = 0;
    int aliveCells = 0;
    
    while (!WindowShouldClose()) {
        // Update logic
        if (currentState == GAME) {
            // Allow player to add cells by clicking
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
                Vector2 mousePos = GetMousePosition();
                int x = mousePos.x / CELL_SIZE;
                int y = mousePos.y / CELL_SIZE;
                if (x >= 0 && x < GRID_WIDTH && y >= 0 && y < GRID_HEIGHT) {
                    grid[y][x] = true;
                }
            }
            
            if (IsKeyPressed(KEY_ESCAPE)) {
                currentState = MAIN_MENU;
                clearGrid(grid);
            }
            
            // Update simulation
            frameCounter++;
            if (frameCounter >= 6) {
                updateGrid(grid, nextGrid);
                frameCounter = 0;
            }
            
            // Count alive cells
            aliveCells = countAliveCells(grid);
            
            // Check win condition
            if (aliveCells == 0) {
                currentState = WIN_SCREEN;
            }
        }
        
        // Render
        BeginDrawing();
        ClearBackground(BLACK);
        
        if (currentState == MAIN_MENU) {
            const char* title = "BitBloom";
            int titleWidth = MeasureText(title, 60);
            DrawText(title, (SCREEN_WIDTH - titleWidth) / 2, 100, 60, GREEN);
            
            const char* subtitle = "Eliminate all living cells!";
            int subtitleWidth = MeasureText(subtitle, 20);
            DrawText(subtitle, (SCREEN_WIDTH - subtitleWidth) / 2, 180, 20, LIGHTGRAY);
            
            int buttonWidth = 300;
            int buttonHeight = 60;
            int buttonX = (SCREEN_WIDTH - buttonWidth) / 2;
            
            bool playHovered = isMouseOver(buttonX, 250, buttonWidth, buttonHeight);
            bool quitHovered = isMouseOver(buttonX, 330, buttonWidth, buttonHeight);
            
            if (drawButton("Play", buttonX, 250, buttonWidth, buttonHeight, playHovered)) {
                clearGrid(grid);
                randomSeed(grid, 0.3f);
                currentState = GAME;
                frameCounter = 0;
            }
            
            if (drawButton("Exit", buttonX, 330, buttonWidth, buttonHeight, quitHovered)) {
                CloseWindow();
                return 0;
            }
        }
        else if (currentState == GAME) {
            // Draw grid
            for (int y = 0; y < GRID_HEIGHT; y++) {
                for (int x = 0; x < GRID_WIDTH; x++) {
                    if (grid[y][x]) {
                        DrawRectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE - 1, CELL_SIZE - 1, GREEN);
                    }
                }
            }
            
            // Draw UI
            DrawRectangle(0, 0, SCREEN_WIDTH, 40, Fade(BLACK, 0.7f));
            
            char cellCountText[64];
            snprintf(cellCountText, sizeof(cellCountText), "Alive Cells: %d", aliveCells);
            DrawText(cellCountText, 10, 10, 24, WHITE);
            
            DrawText("Goal: Eliminate all cells!", SCREEN_WIDTH - 250, 10, 20, YELLOW);
            
            DrawText("Left Click: Place Cell | ESC: Menu", 10, SCREEN_HEIGHT - 30, 18, GRAY);
        }
        else if (currentState == WIN_SCREEN) {
            const char* winTitle = "YOU WIN!";
            int winTitleWidth = MeasureText(winTitle, 60);
            DrawText(winTitle, (SCREEN_WIDTH - winTitleWidth) / 2, 150, 60, GREEN);
            
            const char* winSubtitle = "All cells eliminated!";
            int winSubtitleWidth = MeasureText(winSubtitle, 30);
            DrawText(winSubtitle, (SCREEN_WIDTH - winSubtitleWidth) / 2, 230, 30, LIGHTGRAY);
            
            int buttonWidth = 300;
            int buttonHeight = 60;
            int buttonX = (SCREEN_WIDTH - buttonWidth) / 2;
            
            bool playAgainHovered = isMouseOver(buttonX, 300, buttonWidth, buttonHeight);
            bool menuHovered = isMouseOver(buttonX, 380, buttonWidth, buttonHeight);
            
            if (drawButton("Play Again", buttonX, 300, buttonWidth, buttonHeight, playAgainHovered)) {
                clearGrid(grid);
                randomSeed(grid, 0.3f);
                currentState = GAME;
                frameCounter = 0;
            }
            
            if (drawButton("Main Menu", buttonX, 380, buttonWidth, buttonHeight, menuHovered)) {
                currentState = MAIN_MENU;
                clearGrid(grid);
            }
        }
        
        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}
