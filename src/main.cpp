#include "Game.h"

int main() {
    const int GRID_WIDTH = 80;
    const int GRID_HEIGHT = 60;
    const int CELL_SIZE = 10;
    
    Game game(GRID_WIDTH, GRID_HEIGHT, CELL_SIZE);
    game.run();
    
    return 0;
}
