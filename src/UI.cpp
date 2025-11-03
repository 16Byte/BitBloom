#include "UI.h"
#include <cstdio>
#include <fstream>
#include <vector>
#include <string>

bool UI::drawButton(const char* text, int x, int y, int width, int height, bool isHovered) {
    Color buttonColor = isHovered ? WHITE : BLACK;
    Color borderColor = WHITE;
    Color textColor = isHovered ? BLACK : WHITE;
    
    DrawRectangle(x, y, width, height, buttonColor);
    DrawRectangleLines(x, y, width, height, borderColor);
    
    int textWidth = MeasureText(text, 30);
    DrawText(text, x + (width - textWidth) / 2, y + (height - 30) / 2, 30, textColor);
    
    return isHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

bool UI::isMouseOver(int x, int y, int width, int height) {
    Vector2 mousePos = GetMousePosition();
    return mousePos.x >= x && mousePos.x <= x + width &&
           mousePos.y >= y && mousePos.y <= y + height;
}

std::vector<std::string> UI::loadAsciiArt(const char* filename) {
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

void UI::drawMainMenu(int screenWidth, int screenHeight) {
    (void)screenHeight; // Unused
    
    // Try to load ASCII art logo, fall back to text if not found
    static std::vector<std::string> asciiLogo = loadAsciiArt("assets/bitbloom_logo.txt");
    
    if (!asciiLogo.empty()) {
        // Draw ASCII art logo
        int startY = 50;
        int fontSize = 10;
        for (size_t i = 0; i < asciiLogo.size(); i++) {
            int textWidth = MeasureText(asciiLogo[i].c_str(), fontSize);
            DrawText(asciiLogo[i].c_str(), (screenWidth - textWidth) / 2, startY + (i * fontSize), fontSize, GREEN);
        }
    } else {
        // Fallback to simple text title
        const char* title = "BitBloom";
        int titleWidth = MeasureText(title, 60);
        DrawText(title, (screenWidth - titleWidth) / 2, 100, 60, GREEN);
    }
    
    const char* subtitle = "Eliminate all living cells!";
    int subtitleWidth = MeasureText(subtitle, 20);
    DrawText(subtitle, (screenWidth - subtitleWidth) / 2, 180, 20, LIGHTGRAY);
}

void UI::drawGameUI(int aliveCells, float gameTimer, int screenWidth, int screenHeight) {
    const int UI_TOP_HEIGHT = 50;
    const int UI_BOTTOM_HEIGHT = 40;
    
    // Draw top UI bar
    DrawRectangle(0, 0, screenWidth, UI_TOP_HEIGHT, BLACK);
    
    char cellCountText[64];
    snprintf(cellCountText, sizeof(cellCountText), "Alive Cells: %d", aliveCells);
    DrawText(cellCountText, 10, 15, 24, WHITE);
    
    // Draw timer
    char timerText[32];
    formatTime(gameTimer, timerText, sizeof(timerText));
    int timerWidth = MeasureText(timerText, 24);
    DrawText(timerText, screenWidth / 2 - timerWidth / 2, 15, 24, YELLOW);
    
    // Draw bottom UI bar
    DrawRectangle(0, screenHeight - UI_BOTTOM_HEIGHT, screenWidth, UI_BOTTOM_HEIGHT, BLACK);
    DrawText("Left Click: Place Cell | ESC: Menu", 10, screenHeight - 30, 18, GRAY);
}

void UI::drawWinScreen(float finalTime, int screenWidth, int screenHeight) {
    (void)screenHeight; // Unused
    
    const char* winTitle = "YOU WIN!";
    int winTitleWidth = MeasureText(winTitle, 60);
    DrawText(winTitle, (screenWidth - winTitleWidth) / 2, 120, 60, GREEN);
    
    const char* winSubtitle = "All cells eliminated!";
    int winSubtitleWidth = MeasureText(winSubtitle, 30);
    DrawText(winSubtitle, (screenWidth - winSubtitleWidth) / 2, 200, 30, LIGHTGRAY);
    
    // Display final time
    char finalTimeText[64];
    formatTime(finalTime, finalTimeText, sizeof(finalTimeText));
    char displayText[80];
    snprintf(displayText, sizeof(displayText), "Your Time: %s", finalTimeText);
    int finalTimeWidth = MeasureText(displayText, 36);
    DrawText(displayText, (screenWidth - finalTimeWidth) / 2, 250, 36, YELLOW);
}

void UI::formatTime(float time, char* buffer, std::size_t bufferSize) {
    int minutes = (int)time / 60;
    int seconds = (int)time % 60;
    int milliseconds = (int)((time - (int)time) * 100);
    snprintf(buffer, bufferSize, "%02d:%02d.%02d", minutes, seconds, milliseconds);
}
