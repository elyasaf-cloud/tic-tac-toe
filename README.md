# Tic-tac-toe game

A command-line implementation of the classic Tic-tac-toe game written in **C**

## Features

- Text-based user interface
- Two-player gameplay
- Multiple round support
- Victory tracking across games
- Cross-platform (Windows/Linux)
- Clean screen support for better visibility
- Data is saved from game to game

## How to Play

1. Players enter their names (or use previous data)
2. The board is displayed as a 3×3 grid with numbered squares (1-9)
3. Players take turns placing **X** or **O** by entering a number
4. Win by getting three in a row (horizontal, vertical, or diagonal)
5. Choose to play another round or quit
6. Final winner is determined by total victories
7. Choose whether to save the data for next time

## Controls

- Enter numbers 1-9 to place X/O
- 'q' to quit at any prompt
- 'y'/'n' to continue/end after each game

## Project Structure
- **`main.c`** – Program entry point and games loop
- **`utils.h`** – Header file with function declarations and structures
- **`utils_logic.c`** – Game logic implementation
- **`utils_file.c`** - File handling
- **`utils_UI.c`** - User interface functions
- **`.vscode/tasks.json`** and **`.vscode/launch.json`** – Configuration files for automatic compilation, running, and debugging in **Visual Studio Code**

## Purpose
This project is primarily educational, focusing on:
- Practicing modular programming with multiple files in **C**
- Understanding bit-level board representation (only in the first version)
- Using structures and functions effectively

## Installation

### Quick Launch
For most users, the fastest way to run the game is to use the precompiled executables:
- **Windows**: download `windows_tic-tac-toe.exe` and double-click to start the game
- **Linux**: download `linux_tic-tac-toe`, open a terminal in the download folder, and run:  
  ```bash
  ./linux_tic-tac-toe

### Manual Compilatio
If the quick launch executables do not work on your system, you can compile the program yourself:
1. Download the entire project folder
2. Open a terminal in the project directory
3. Compile the source code with:
    gcc main.c utils_UI.c utils_logic.c utils_file.c -o tic-tac-toe
4. Run the program with:
    ./tic-tac-toe

### Running from VS Code
The repository also includes VS Code configuration files for easy building and debugging:
1. Open the project in Visual Studio Code
2. Use the Run or Debug options to build and start the game automatically

## Screenshot
![App Screenshot](images/screenshot.png)

---

*This README was generated with the help of an AI assistance*
