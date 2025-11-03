#ifndef UI_H
#define UI_H

#include "raylib.h"
#include <cstddef>
#include <vector>
#include <string>

class UI {
public:
    static bool drawButton(const char* text, int x, int y, int width, int height, bool isHovered);
    static bool isMouseOver(int x, int y, int width, int height);
    
    static void drawMainMenu(int screenWidth, int screenHeight);
    static void drawGameUI(int aliveCells, float gameTimer, int screenWidth, int screenHeight);
    static void drawWinScreen(float finalTime, int screenWidth, int screenHeight);
    
    static void formatTime(float time, char* buffer, std::size_t bufferSize);
    static std::vector<std::string> loadAsciiArt(const char* filename);
};

#endif // UI_H
