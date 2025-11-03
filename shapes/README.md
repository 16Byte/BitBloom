# Shape Detection System

The shape detection system allows you to define patterns that the game will recognize and react to during gameplay.

## Creating New Shapes

Shapes are defined in text files in the `shapes/` directory.

### File Format:
```
shape_name
pattern_line_1
pattern_line_2
...
```

- **First line**: Name of the shape (used for callbacks)
- **Remaining lines**: Pattern using `0` (dead cell) and `1` (alive cell)
- Files can be any size (size-agnostic detection)

### Example: Diamond
**File**: `shapes/diamond.txt`
```
diamond
010
101
010
```

### Example: Glider
**File**: `shapes/glider.txt`
```
glider
010
001
111
```

### Example: Blinker
**File**: `shapes/blinker.txt`
```
blinker
111
```

## How It Works

1. **Loading**: All `.txt` files in `shapes/` directory are loaded at game start
2. **Detection**: After each grid update, the system scans for matching patterns
3. **Callbacks**: When a shape is detected, its callback function is triggered
4. **Position**: Callback receives the center position (x, y) of the detected shape

## Adding Callbacks

Edit the `setupShapeCallbacks()` function in `Game.cpp`:

```cpp
if (shapeName == "your_shape_name") {
    shape->setCallback([](int x, int y) {
        // Your code here - play sound, show effect, etc.
        PlaySound(mySound);
    });
}
```

## Use Cases

- **Audio Feedback**: Play sounds when specific patterns appear
- **Visual Effects**: Highlight or animate detected patterns
- **Scoring**: Award points for creating certain patterns
- **Achievements**: Track if player creates specific configurations
- **Tutorials**: Detect when player successfully recreates example patterns

## Performance Notes

- Detection runs once per simulation step (every 6 frames)
- Only scans when grid is actively simulating
- Efficient pattern matching algorithm
- Multiple instances of the same shape can be detected simultaneously

## Example Shapes to Create

- **Block** (2x2 still life)
- **Beehive** (stable pattern)
- **Toad** (oscillator)
- **Pulsar** (period-3 oscillator)
- **Spaceship** (moving patterns)
- **Gun** (generates gliders)
- **Self-destructing patterns** (consume themselves)
