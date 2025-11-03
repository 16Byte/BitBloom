#include "ShapeDetector.h"
#include <dirent.h>
#include <sys/stat.h>
#include <cstring>

ShapeDetector::ShapeDetector() {
}

ShapeDetector::~ShapeDetector() {
    for (Shape* shape : shapes) {
        delete shape;
    }
    shapes.clear();
}

void ShapeDetector::loadShapesFromDirectory(const char* directory) {
    DIR* dir = opendir(directory);
    if (!dir) {
        return;
    }
    
    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        // Skip . and ..
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        
        // Build full path
        std::string fullPath = std::string(directory) + "/" + std::string(entry->d_name);
        
        // Check if it's a regular file
        struct stat statbuf;
        if (stat(fullPath.c_str(), &statbuf) == 0 && S_ISREG(statbuf.st_mode)) {
            // Try to load as a shape
            Shape* shape = Shape::loadFromFile(fullPath.c_str());
            if (shape) {
                shapes.push_back(shape);
            }
        }
    }
    
    closedir(dir);
}

void ShapeDetector::addShape(Shape* shape) {
    if (shape) {
        shapes.push_back(shape);
    }
}

bool ShapeDetector::matchesShapeAt(const Grid& grid, const Shape& shape, int x, int y) const {
    // Check if the shape matches at position (x, y)
    // (x, y) is the top-left corner of where we're checking
    
    for (int sy = 0; sy < shape.getHeight(); sy++) {
        for (int sx = 0; sx < shape.getWidth(); sx++) {
            int gridX = x + sx;
            int gridY = y + sy;
            
            bool shapeCell = shape.getCell(sx, sy);
            bool gridCell = grid.getCell(gridX, gridY);
            
            // If they don't match, this isn't the shape
            if (shapeCell != gridCell) {
                return false;
            }
        }
    }
    
    return true;
}

void ShapeDetector::detectAndTrigger(const Grid& grid) {
    // Check every position in the grid for every shape
    for (Shape* shape : shapes) {
        for (int y = 0; y <= grid.getHeight() - shape->getHeight(); y++) {
            for (int x = 0; x <= grid.getWidth() - shape->getWidth(); x++) {
                if (matchesShapeAt(grid, *shape, x, y)) {
                    // Found a match! Trigger callback with center position
                    int centerX = x + shape->getWidth() / 2;
                    int centerY = y + shape->getHeight() / 2;
                    shape->triggerCallback(centerX, centerY);
                }
            }
        }
    }
}
