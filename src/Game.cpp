#include "Game.h"
#include "UI.h"
#include "raylib.h"
#include <ctime>

Game::Game(int gridWidth, int gridHeight, int cellSize) 
    : gridWidth(gridWidth), gridHeight(gridHeight), cellSize(cellSize),
      gridOffsetX(GRID_PADDING_X), gridOffsetY(UI_TOP_HEIGHT),
      currentState(MAIN_MENU), gameTimer(0.0f), finalTime(0.0f), frameCounter(0) {
    
    screenWidth = gridWidth * cellSize + (GRID_PADDING_X * 2);
    screenHeight = gridHeight * cellSize + UI_TOP_HEIGHT + UI_BOTTOM_HEIGHT;
    
    grid = new Grid(gridWidth, gridHeight);
    
    SetRandomSeed(time(NULL));
}

Game::~Game() {
    delete grid;
}

void Game::run() {
    SetExitKey(0);
    InitWindow(screenWidth, screenHeight, "BitBloom");
    SetTargetFPS(60);
    
    while (!WindowShouldClose()) {
        handleInput();
        update();
        render();
    }
    
    CloseWindow();
}

void Game::handleInput() {
    switch (currentState) {
        case MAIN_MENU:
            handleMainMenuInput();
            break;
        case GAME:
            handleGameInput();
            break;
        case WIN_SCREEN:
            handleWinScreenInput();
            break;
    }
}

void Game::update() {
    if (currentState == GAME) {
        // Update timer
        gameTimer += GetFrameTime();
        
        // Update simulation
        frameCounter++;
        if (frameCounter >= 6) {
            grid->update();
            frameCounter = 0;
        }
        
        // Check win condition
        if (grid->countAliveCells() == 0) {
            finalTime = gameTimer;
            currentState = WIN_SCREEN;
        }
    }
}

void Game::render() {
    BeginDrawing();
    ClearBackground(BLACK);
    
    switch (currentState) {
        case MAIN_MENU:
            renderMainMenu();
            break;
        case GAME:
            renderGame();
            break;
        case WIN_SCREEN:
            renderWinScreen();
            break;
    }
    
    EndDrawing();
}

void Game::startNewGame() {
    grid->clear();
    grid->randomSeed(0.3f);
    currentState = GAME;
    frameCounter = 0;
    gameTimer = 0.0f;
}

void Game::handleMainMenuInput() {
    int buttonWidth = 300;
    int buttonHeight = 60;
    int buttonX = (screenWidth - buttonWidth) / 2;
    
    bool playHovered = UI::isMouseOver(buttonX, 250, buttonWidth, buttonHeight);
    bool quitHovered = UI::isMouseOver(buttonX, 330, buttonWidth, buttonHeight);
    
    if (UI::drawButton("Play", buttonX, 250, buttonWidth, buttonHeight, playHovered)) {
        startNewGame();
    }
    
    if (UI::drawButton("Exit", buttonX, 330, buttonWidth, buttonHeight, quitHovered)) {
        CloseWindow();
        exit(0);
    }
}

void Game::handleGameInput() {
    // Allow player to add cells by clicking
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePos = GetMousePosition();
        int x = (mousePos.x - gridOffsetX) / cellSize;
        int y = (mousePos.y - gridOffsetY) / cellSize;
        if (x >= 0 && x < gridWidth && y >= 0 && y < gridHeight) {
            grid->setCell(x, y, true);
        }
    }
    
    if (IsKeyPressed(KEY_ESCAPE)) {
        currentState = MAIN_MENU;
        grid->clear();
        gameTimer = 0.0f;
    }
}

void Game::handleWinScreenInput() {
    int buttonWidth = 300;
    int buttonHeight = 60;
    int buttonX = (screenWidth - buttonWidth) / 2;
    
    bool playAgainHovered = UI::isMouseOver(buttonX, 320, buttonWidth, buttonHeight);
    bool menuHovered = UI::isMouseOver(buttonX, 400, buttonWidth, buttonHeight);
    
    if (UI::drawButton("Play Again", buttonX, 320, buttonWidth, buttonHeight, playAgainHovered)) {
        startNewGame();
    }
    
    if (UI::drawButton("Main Menu", buttonX, 400, buttonWidth, buttonHeight, menuHovered)) {
        currentState = MAIN_MENU;
        grid->clear();
    }
}

void Game::renderMainMenu() {
    UI::drawMainMenu(screenWidth, screenHeight);
    // Buttons are drawn in handleMainMenuInput
}

void Game::renderGame() {
    // Draw top UI bar
    UI::drawGameUI(grid->countAliveCells(), gameTimer, screenWidth, screenHeight);
    
    // Draw grid with offset
    for (int y = 0; y < gridHeight; y++) {
        for (int x = 0; x < gridWidth; x++) {
            if (grid->getCell(x, y)) {
                DrawRectangle(x * cellSize + gridOffsetX, y * cellSize + gridOffsetY, 
                            cellSize - 1, cellSize - 1, GREEN);
            }
        }
    }
    
    // Draw border around game grid
    DrawRectangleLines(gridOffsetX, gridOffsetY, gridWidth * cellSize, gridHeight * cellSize, WHITE);
}

void Game::renderWinScreen() {
    UI::drawWinScreen(finalTime, screenWidth, screenHeight);
    // Buttons are drawn in handleWinScreenInput
}
