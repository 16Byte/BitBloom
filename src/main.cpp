#include "raylib.h"
#include <vector>
#include <deque>
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <fstream>
#include <string>

const int GRID_WIDTH = 80;
const int GRID_HEIGHT = 60;
const int CELL_SIZE = 10;
const int UI_TOP_HEIGHT = 50;
const int UI_BOTTOM_HEIGHT = 40;
const int GRID_OFFSET_Y = UI_TOP_HEIGHT;
const int SCREEN_WIDTH = GRID_WIDTH * CELL_SIZE;
const int SCREEN_HEIGHT = GRID_HEIGHT * CELL_SIZE + UI_TOP_HEIGHT + UI_BOTTOM_HEIGHT;

enum GameState {
    MAIN_MENU,
    GAME,
    PAUSED,
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
    Color buttonColor = isHovered ? WHITE : BLACK;
    Color borderColor = WHITE;
    Color textColor = isHovered ? BLACK : WHITE;
    
    DrawRectangle(x, y, width, height, buttonColor);
    DrawRectangleLines(x, y, width, height, borderColor);
    
    int textWidth = MeasureText(text, 30);
    DrawText(text, x + (width - textWidth) / 2, y + (height - 30) / 2, 30, textColor);
    
    return isHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

bool isMouseOver(int x, int y, int width, int height) {
    Vector2 mousePos = GetMousePosition();
    return mousePos.x >= x && mousePos.x <= x + width &&
           mousePos.y >= y && mousePos.y <= y + height;
}

std::vector<std::string> loadAsciiArt(const char* filename) {
    std::vector<std::string> lines;
    std::ifstream file(filename);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            lines.push_back(line);
        }
        file.close();
    }
    return lines;
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
    float gameTimer = 0.0f;
    float finalTime = 0.0f;
    
    // Load ASCII art logo
    std::vector<std::string> asciiLogo = loadAsciiArt("assets/bitbloom_logo.txt");
    
    while (!WindowShouldClose()) {
        // Update logic
        if (currentState == GAME) {
            HideCursor();
            
            // Allow player to add cells by clicking
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
                Vector2 mousePos = GetMousePosition();
                int x = mousePos.x / CELL_SIZE;
                int y = (mousePos.y - GRID_OFFSET_Y) / CELL_SIZE;
                if (x >= 0 && x < GRID_WIDTH && y >= 0 && y < GRID_HEIGHT) {
                    grid[y][x] = true;
                }
            }
            
            if (IsKeyPressed(KEY_ESCAPE)) {
                currentState = PAUSED;
            }
            
            // Update timer
            gameTimer += GetFrameTime();
            
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
                finalTime = gameTimer;
                currentState = WIN_SCREEN;
            }
        }
        else if (currentState == PAUSED) {
            ShowCursor();
            
            if (IsKeyPressed(KEY_ESCAPE)) {
                currentState = GAME;
            }
        }
        else {
            ShowCursor();
        }
        
        // Render
        BeginDrawing();
        ClearBackground(BLACK);

        // Remove escape as the exit key
        SetExitKey(0);
        
        if (currentState == MAIN_MENU) {
            // Draw ASCII art logo with fixed-width character spacing
            int logoY = 50;
            int fontSize = 16;
            int charSpacing = 10;  // Fixed spacing for monospaced appearance at larger size
            
            for (size_t i = 0; i < asciiLogo.size(); i++) {
                int startX = 50;  // Left-aligned starting position
                for (size_t j = 0; j < asciiLogo[i].length(); j++) {
                    char c[2] = {asciiLogo[i][j], '\0'};
                    DrawText(c, startX + (j * charSpacing), logoY + (i * (fontSize + 3)), fontSize, WHITE);
                }
            }
            
            const char* subtitle = "Eliminate all living cells!";
            int subtitleWidth = MeasureText(subtitle, 20);
            DrawText(subtitle, (SCREEN_WIDTH - subtitleWidth) / 2, 210, 20, LIGHTGRAY);
            
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
                gameTimer = 0.0f;
            }
            
            if (drawButton("Exit", buttonX, 330, buttonWidth, buttonHeight, quitHovered)) {
                CloseWindow();
                return 0;
            }
        }
        else if (currentState == GAME) {
            // Draw top UI bar
            DrawRectangle(0, 0, SCREEN_WIDTH, UI_TOP_HEIGHT, BLACK);
            
            char cellCountText[64];
            snprintf(cellCountText, sizeof(cellCountText), "Alive Cells: %d", aliveCells);
            DrawText(cellCountText, 10, 15, 24, WHITE);
            
            // Draw timer
            int minutes = (int)gameTimer / 60;
            int seconds = (int)gameTimer % 60;
            int milliseconds = (int)((gameTimer - (int)gameTimer) * 100);
            char timerText[32];
            snprintf(timerText, sizeof(timerText), "Time: %02d:%02d.%02d", minutes, seconds, milliseconds);
            int timerWidth = MeasureText(timerText, 24);
            DrawText(timerText, SCREEN_WIDTH / 2 - timerWidth / 2, 15, 24, YELLOW);
            
            // Draw grid with offset
            for (int y = 0; y < GRID_HEIGHT; y++) {
                for (int x = 0; x < GRID_WIDTH; x++) {
                    if (grid[y][x]) {
                        DrawRectangle(x * CELL_SIZE, y * CELL_SIZE + GRID_OFFSET_Y, CELL_SIZE - 1, CELL_SIZE - 1, WHITE);
                    }
                }
            }
            
            // Draw custom crosshair cursor
            Vector2 mousePos = GetMousePosition();
            int hoverX = mousePos.x / CELL_SIZE;
            int hoverY = (mousePos.y - GRID_OFFSET_Y) / CELL_SIZE;
            
            // Only draw cursor if mouse is in grid bounds
            if (mousePos.y >= GRID_OFFSET_Y && mousePos.y < GRID_OFFSET_Y + (GRID_HEIGHT * CELL_SIZE)) {
                // Draw crosshair lines
                int crosshairSize = 8;
                int gap = 3;
                Color cursorColor = (Color){255, 255, 255, 200};
                
                // Horizontal line (left)
                DrawLineEx((Vector2){mousePos.x - crosshairSize, mousePos.y}, 
                          (Vector2){mousePos.x - gap, mousePos.y}, 2, cursorColor);
                // Horizontal line (right)
                DrawLineEx((Vector2){mousePos.x + gap, mousePos.y}, 
                          (Vector2){mousePos.x + crosshairSize, mousePos.y}, 2, cursorColor);
                // Vertical line (top)
                DrawLineEx((Vector2){mousePos.x, mousePos.y - crosshairSize}, 
                          (Vector2){mousePos.x, mousePos.y - gap}, 2, cursorColor);
                // Vertical line (bottom)
                DrawLineEx((Vector2){mousePos.x, mousePos.y + gap}, 
                          (Vector2){mousePos.x, mousePos.y + crosshairSize}, 2, cursorColor);
                
                // Draw center dot
                DrawCircle(mousePos.x, mousePos.y, 1.5f, cursorColor);
                
                // Highlight the cell that will be placed
                if (hoverX >= 0 && hoverX < GRID_WIDTH && hoverY >= 0 && hoverY < GRID_HEIGHT) {
                    if (!grid[hoverY][hoverX]) {
                        DrawRectangleLines(hoverX * CELL_SIZE, hoverY * CELL_SIZE + GRID_OFFSET_Y, 
                                         CELL_SIZE - 1, CELL_SIZE - 1, (Color){255, 255, 255, 120});
                    }
                }
            }
            
            // Draw border around game grid
            DrawRectangleLines(0, GRID_OFFSET_Y, SCREEN_WIDTH, GRID_HEIGHT * CELL_SIZE, WHITE);
            
            // Draw bottom UI bar
            DrawRectangle(0, SCREEN_HEIGHT - UI_BOTTOM_HEIGHT, SCREEN_WIDTH, UI_BOTTOM_HEIGHT, BLACK);
            DrawText("Left Click: Place Cell | ESC: Menu", 10, SCREEN_HEIGHT - 30, 18, GRAY);
        }
        else if (currentState == PAUSED) {
            // Draw the game state in the background
            DrawRectangle(0, 0, SCREEN_WIDTH, UI_TOP_HEIGHT, BLACK);
            
            char cellCountText[64];
            snprintf(cellCountText, sizeof(cellCountText), "Alive Cells: %d", aliveCells);
            DrawText(cellCountText, 10, 15, 24, WHITE);
            
            int minutes = (int)gameTimer / 60;
            int seconds = (int)gameTimer % 60;
            int milliseconds = (int)((gameTimer - (int)gameTimer) * 100);
            char timerText[32];
            snprintf(timerText, sizeof(timerText), "Time: %02d:%02d.%02d", minutes, seconds, milliseconds);
            int timerWidth = MeasureText(timerText, 24);
            DrawText(timerText, SCREEN_WIDTH / 2 - timerWidth / 2, 15, 24, YELLOW);
            
            for (int y = 0; y < GRID_HEIGHT; y++) {
                for (int x = 0; x < GRID_WIDTH; x++) {
                    if (grid[y][x]) {
                        DrawRectangle(x * CELL_SIZE, y * CELL_SIZE + GRID_OFFSET_Y, CELL_SIZE - 1, CELL_SIZE - 1, WHITE);
                    }
                }
            }
            
            DrawRectangleLines(0, GRID_OFFSET_Y, SCREEN_WIDTH, GRID_HEIGHT * CELL_SIZE, WHITE);
            DrawRectangle(0, SCREEN_HEIGHT - UI_BOTTOM_HEIGHT, SCREEN_WIDTH, UI_BOTTOM_HEIGHT, BLACK);
            
            // Draw semi-transparent overlay
            DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (Color){0, 0, 0, 180});
            
            // Draw pause menu
            const char* pauseTitle = "PAUSED";
            int pauseTitleWidth = MeasureText(pauseTitle, 60);
            DrawText(pauseTitle, (SCREEN_WIDTH - pauseTitleWidth) / 2, 120, 60, WHITE);
            
            int buttonWidth = 300;
            int buttonHeight = 60;
            int buttonX = (SCREEN_WIDTH - buttonWidth) / 2;
            
            bool resumeHovered = isMouseOver(buttonX, 220, buttonWidth, buttonHeight);
            bool mainMenuHovered = isMouseOver(buttonX, 300, buttonWidth, buttonHeight);
            bool exitHovered = isMouseOver(buttonX, 380, buttonWidth, buttonHeight);
            
            if (drawButton("Resume (ESC)", buttonX, 220, buttonWidth, buttonHeight, resumeHovered)) {
                currentState = GAME;
            }
            
            if (drawButton("Main Menu", buttonX, 300, buttonWidth, buttonHeight, mainMenuHovered)) {
                currentState = MAIN_MENU;
                clearGrid(grid);
                gameTimer = 0.0f;
            }
            
            if (drawButton("Exit Game", buttonX, 380, buttonWidth, buttonHeight, exitHovered)) {
                break;
            }
        }
        else if (currentState == WIN_SCREEN) {
            const char* winTitle = "YOU WIN!";
            int winTitleWidth = MeasureText(winTitle, 60);
            DrawText(winTitle, (SCREEN_WIDTH - winTitleWidth) / 2, 120, 60, GREEN);
            
            const char* winSubtitle = "All cells eliminated!";
            int winSubtitleWidth = MeasureText(winSubtitle, 30);
            DrawText(winSubtitle, (SCREEN_WIDTH - winSubtitleWidth) / 2, 200, 30, LIGHTGRAY);
            
            // Display final time
            int minutes = (int)finalTime / 60;
            int seconds = (int)finalTime % 60;
            int milliseconds = (int)((finalTime - (int)finalTime) * 100);
            char finalTimeText[64];
            snprintf(finalTimeText, sizeof(finalTimeText), "Your Time: %02d:%02d.%02d", minutes, seconds, milliseconds);
            int finalTimeWidth = MeasureText(finalTimeText, 36);
            DrawText(finalTimeText, (SCREEN_WIDTH - finalTimeWidth) / 2, 250, 36, YELLOW);
            
            int buttonWidth = 300;
            int buttonHeight = 60;
            int buttonX = (SCREEN_WIDTH - buttonWidth) / 2;
            
            bool playAgainHovered = isMouseOver(buttonX, 320, buttonWidth, buttonHeight);
            bool menuHovered = isMouseOver(buttonX, 400, buttonWidth, buttonHeight);
            
            if (drawButton("Play Again", buttonX, 320, buttonWidth, buttonHeight, playAgainHovered)) {
                clearGrid(grid);
                randomSeed(grid, 0.3f);
                currentState = GAME;
                frameCounter = 0;
                gameTimer = 0.0f;
            }
            
            if (drawButton("Main Menu", buttonX, 400, buttonWidth, buttonHeight, menuHovered)) {
                currentState = MAIN_MENU;
                clearGrid(grid);
            }
        }
        
        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}
