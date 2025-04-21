# Maze Game

A simple 2D maze game built using C++ and SFML.The game Navigate through the maze, collect points, avoid enemies, and reach the exit to complete each level!

## Features
- **Two challenging levels** with different maze layouts.
- **Player movement** using WASD keys.
- **Enemy AI** that moves randomly around the maze.
- **Score system** for collecting items (*).
- **Save/Load functionality** to continue your progress.
- **Visual feedback** with colored tiles representing different elements.
- **Game stats** showing current level, score, and moves.

## How to Play
1. **Controls**:
   - W: Move Up
   - A: Move Left
   - S: Move Down
   - D: Move Right
   - ESC: Quit Game

2. **Game Elements**:
   - █ (Magenta): Walls (cannot pass through)
   - P (Blue): Player
   - X (Red): Enemy (lose if touched)
   - * (Yellow): Collectible (10 points)
   - E (White): Exit (complete level)

3. **Main Menu**:
   - Press 1: Start New Game
   - Press 2: Continue Saved Game
   - ESC: Quit

## Requirements
- C++ compiler with C++11 support
- SFML library (version 2.5.1 or compatible)
- Arial font file (included with most systems)

## Installation & Running
1. Clone this repository or download the source files.
2. Compile with SFML linked:
   ```bash
   g++ -std=c++11 main.cpp -o maze_game -lsfml-graphics -lsfml-window -lsfml-system
## Game Logic
- **Auto-save**: Progress is saved when you complete a level or lose.  
- **Level progression**: Beat Level 1 to unlock Level 2 (more challenging!).  
- **Enemies**: Touching one (`X`) ends the game immediately—stay sharp!  
- **Collectibles**: Grab all `*` (10 points each) before exiting for max score.  
- **Exits**: Reach `E` to finish a level (bonus 50 points!).  

*tip: The game automatically saves no need to manually save it.  

*Thank you
