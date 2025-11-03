#include "Shape.h"
#include <fstream>
#include <sstream>

Shape::Shape(const std::string& name, const std::vector<std::vector<bool>>& pattern)
    : name(name), pattern(pattern), callback(nullptr) {
    height = pattern.size();
    width = (height > 0) ? pattern[0].size() : 0;
}

bool Shape::getCell(int x, int y) const {
    if (x < 0 || x >= width || y < 0 || y >= height) {
        return false;
    }
    return pattern[y][x];
}

void Shape::setCallback(std::function<void(int, int)> cb) {
    callback = cb;
}

void Shape::triggerCallback(int x, int y) const {
    if (callback) {
        callback(x, y);
    }
}

Shape* Shape::loadFromFile(const char* filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return nullptr;
    }
    
    std::string name;
    std::getline(file, name);
    
    // Trim whitespace from name
    size_t start = name.find_first_not_of(" \t\r\n");
    size_t end = name.find_last_not_of(" \t\r\n");
    if (start != std::string::npos && end != std::string::npos) {
        name = name.substr(start, end - start + 1);
    }
    
    std::vector<std::vector<bool>> pattern;
    std::string line;
    
    while (std::getline(file, line)) {
        // Skip empty lines
        if (line.empty() || line.find_first_not_of(" \t\r\n") == std::string::npos) {
            continue;
        }
        
        std::vector<bool> row;
        for (char c : line) {
            if (c == '0') {
                row.push_back(false);
            } else if (c == '1') {
                row.push_back(true);
            }
            // Ignore other characters (whitespace, etc.)
        }
        
        if (!row.empty()) {
            pattern.push_back(row);
        }
    }
    
    file.close();
    
    if (pattern.empty()) {
        return nullptr;
    }
    
    return new Shape(name, pattern);
}
