# Snake Game in C

A classic Snake game implementation in C using the `ncurses` library for terminal-based graphics.

## Features
- Arrow-key controls for direction.
- Score tracking.
- Game over detection (collision with walls/self).
- Increasing speed as the snake grows.

## Installation
1. Ensure `ncurses` is installed:
   - **Linux**: `sudo apt-get install libncurses5-dev`
   - **macOS**: `brew install ncurses`
2. Compile the code:
   ```bash
   gcc snake.c -o snake -lncurses
