# Minesweeper UE plugins - play Minesweeper inside Unreal Editor

This repository serves as a project container for the Minesweeper Unreal Engine plugin. The main plugin source code is located in the ["Plugins/Minesweeper"](https://github.com/zZnghialamZz/MinesweeperUE/tree/main/Plugins/Minesweeper) directory

## UE Version
- **Unreal Engine 5.6.1**

## Installation
1. Install UE 5.6.1
2. Clone this repository
3. Open the `MineSweeperHolder.uproject` file
4. The project will automatically build all source files on first launch

## Usage
Once the project is loaded, launch Minesweeper by clicking the plugin icon in the Unreal Editor:

<img width="1171" height="253" alt="MinesweeperLogo" src="https://github.com/user-attachments/assets/715fb88e-ddc9-4ffe-a3aa-c9d2de63b6a0" />

## Demo

Here is a video demo of the Plugins:

https://github.com/user-attachments/assets/8b894367-8d3e-463d-8144-a9cf7ce65516

## Development Notes
### Use of AI/LLMs
During development, I used AI tools for specific auxiliary tasks rather than direct code generation:

**In the learning Phase:**

I used Claude AI (via Cursor IDE integration) to understand Minesweeper game rules and mechanics since I don't know much about that game.
Via its recommendation, I also go to minesweeper.online to play and gain practical understanding

**In Code Refinement Phase:**

I also asked AI assistance for code refactoring and architectural improvements.
For example, I usually code a bunch of spaghetti code to make it works first then I asked it to restructure the code as I wanted => it helped me split the code and in some cases, create new files to re-structure them nicely, of course that I need to carefully check but it does help this process move quicker than the normal way.

I feel like AI tools proved effective for structural reorganization tasks, though all generated code need to be reviewed and validation carefully (especially with C++ UE5 code, some LLMs like GPT or Copilot still use a lot of legacy UE4 code for unreal C++ project, so if I want to develop on UE5 project I usually stick with Cursor + Claude4 and re-check with EpicGame latest documents).
This approach leveraged AI strengths in code organization and refactoring while I can maintain direct control over core game logic implementation and code quality overall.
