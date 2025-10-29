# BitBloom
A game based on Conway's Game of Life with a twist - your goal is to eliminate all living cells!

## How to Play
- The game starts with a randomly seeded grid of living cells
- The simulation runs automatically following Conway's Game of Life rules
- **Left Click**: Place a cell strategically
- **Goal**: Use the simulation rules to eliminate all cells
- **ESC**: Return to main menu

The challenge: You can only ADD cells, not remove them. Use your knowledge of Conway's rules to create patterns that will cause all cells to eventually die out!

## Game Rules
Living cells follow Conway's Game of Life rules:
- A living cell with 2 or 3 neighbors survives
- A dead cell with exactly 3 neighbors becomes alive
- All other cells die or stay dead

Use your understanding of these rules to strategically eliminate all cells!

## Building
```bash
make
```

## Running
```bash
./bin/game_of_life
```
