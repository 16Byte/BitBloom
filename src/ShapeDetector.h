#ifndef SHAPEDETECTOR_H
#define SHAPEDETECTOR_H

#include "Shape.h"
#include "Grid.h"
#include <vector>
#include <string>

class ShapeDetector {
public:
    ShapeDetector();
    ~ShapeDetector();
    
    // Load all shapes from a directory
    void loadShapesFromDirectory(const char* directory);
    
    // Add a single shape
    void addShape(Shape* shape);
    
    // Check if a specific shape matches at position (x, y) on the grid
    bool matchesShapeAt(const Grid& grid, const Shape& shape, int x, int y) const;
    
    // Detect all shapes in the grid and trigger their callbacks
    void detectAndTrigger(const Grid& grid);
    
    // Get all loaded shapes
    const std::vector<Shape*>& getShapes() const { return shapes; }
    
private:
    std::vector<Shape*> shapes;
};

#endif // SHAPEDETECTOR_H
