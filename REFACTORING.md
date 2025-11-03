# BitBloom Refactoring

## Overview
The code has been refactored from a single monolithic `main.cpp` file into a clean, modular architecture with separate classes and responsibilities.

## New File Structure

```
src/
├── main_new.cpp        # Clean entry point (10 lines)
├── Game.h/cpp          # Main game orchestration class
├── Grid.h/cpp          # Conway's Game of Life logic
├── UI.h/cpp            # All UI rendering functions
└── GameState.h         # Game state enum
```

## Architecture Benefits

### 1. **Separation of Concerns**
- **Grid class**: Handles all Game of Life logic (rules, neighbors, updates)
- **UI class**: All rendering and visual presentation
- **Game class**: Orchestrates game flow, state management, and user input
- **main.cpp**: Simple entry point that just creates and runs the game

### 2. **Improved Testability**
- Grid logic can be tested independently
- UI rendering separated from game logic
- Each class has a single, clear responsibility

### 3. **Better Maintainability**
- Easy to find and modify specific features
- Clear interfaces between components
- Reduced coupling between different parts

### 4. **Scalability**
- Easy to add new features (e.g., different grid patterns, save/load)
- Simple to extend UI with new screens
- Grid class could support different rule sets

## Class Breakdown

### Grid Class
**Responsibility**: Conway's Game of Life simulation
- `update()` - Apply Game of Life rules
- `countNeighbors()` - Count alive neighbors for a cell
- `setCell()` / `getCell()` - Cell manipulation
- `randomSeed()` - Initialize with random pattern
- `countAliveCells()` - Get total alive cells

### UI Class
**Responsibility**: All visual rendering (static utility class)
- `drawMainMenu()` - Render main menu screen
- `drawGameUI()` - Render game HUD (timer, cell count)
- `drawWinScreen()` - Render victory screen
- `drawButton()` - Reusable button component
- `formatTime()` - Time formatting utility

### Game Class
**Responsibility**: Game flow and state management
- `run()` - Main game loop
- `handleInput()` - Process user input based on state
- `update()` - Update game simulation
- `render()` - Render current state
- `startNewGame()` - Initialize new game session

## Building

The Makefile now supports both versions:

```bash
# Build refactored version (default)
make

# Build old monolithic version
make old

# Build both
make both

# Run refactored version
make run

# Run old version
make run-old
```

## Comparison

### Before (main.cpp): 270 lines, everything in one file
- Game logic mixed with rendering
- Hard to test individual components
- Difficult to navigate and modify

### After: Organized into 5 focused files
- **main_new.cpp**: 10 lines
- **Grid.cpp**: 82 lines
- **UI.cpp**: 80 lines  
- **Game.cpp**: 175 lines
- **Headers**: Clean interfaces

## Future Improvements

With this architecture, it's easy to add:
- **Config class**: For game settings (grid size, speed, density)
- **SaveManager**: Save/load game states
- **PatternLibrary**: Pre-defined interesting patterns
- **SoundManager**: Audio feedback
- **Leaderboard**: Time tracking and Steam integration
- **Different rule sets**: B3/S23 variations

## Migration

Both versions produce identical functionality. The old `main.cpp` is preserved as reference. To switch permanently:

```bash
# Replace old main with new
mv src/main.cpp src/main_old.cpp
mv src/main_new.cpp src/main.cpp

# Update Makefile to only build new version
# (Remove old target references)
```
