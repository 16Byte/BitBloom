#ifndef SHAPE_H
#define SHAPE_H

#include <string>
#include <vector>
#include <functional>

class Shape {
public:
    Shape(const std::string& name, const std::vector<std::vector<bool>>& pattern);
    
    const std::string& getName() const { return name; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    bool getCell(int x, int y) const;
    
    // Set a callback function to be called when this shape is detected
    void setCallback(std::function<void(int, int)> callback);
    void triggerCallback(int x, int y) const;
    
    // Load shape from file
    static Shape* loadFromFile(const char* filename);
    
private:
    std::string name;
    int width;
    int height;
    std::vector<std::vector<bool>> pattern;
    std::function<void(int, int)> callback;  // Called with center position when detected
};

#endif // SHAPE_H
