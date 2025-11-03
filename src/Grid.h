#ifndef GRID_H
#define GRID_H

#include <vector>

class Grid {
public:
    Grid(int width, int height);
    
    void update();
    void clear();
    void randomSeed(float density = 0.3f);
    
    bool getCell(int x, int y) const;
    void setCell(int x, int y, bool alive);
    
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    int countAliveCells() const;
    
private:
    int width;
    int height;
    std::vector<std::vector<bool>> cells;
    std::vector<std::vector<bool>> nextCells;
    
    int countNeighbors(int x, int y) const;
};

#endif // GRID_H
