#ifndef GAME_H
#define GAME_H

#include "Grid.h"
#include "GameState.h"
#include "ShapeDetector.h"

class Game {
public:
    Game(int gridWidth, int gridHeight, int cellSize);
    ~Game();
    
    void run();
    
private:
    // Constants
    static const int UI_TOP_HEIGHT = 50;
    static const int UI_BOTTOM_HEIGHT = 40;
    static const int GRID_PADDING_X = 20;
    
    int gridWidth;
    int gridHeight;
    int cellSize;
    int gridOffsetX;
    int gridOffsetY;
    int screenWidth;
    int screenHeight;
    
    Grid* grid;
    ShapeDetector* shapeDetector;
    GameState currentState;
    
    float gameTimer;
    float finalTime;
    int frameCounter;
    
    void handleInput();
    void update();
    void render();
    
    void startNewGame();
    void handleMainMenuInput();
    void handleGameInput();
    void handleWinScreenInput();
    
    void renderMainMenu();
    void renderGame();
    void renderWinScreen();
    
    void setupShapeCallbacks();
};

#endif // GAME_H
