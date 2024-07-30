#  1. Overview 
This Minesweeper game is a Qt-based C++ application that mimics the classic Minesweeper 
game. The objective is to clear a grid of hidden mines without detonating any of them. The game 
features a custom grid size and mines and a user-friendly graphical interface. 

**Features:**
* Custom grid size and number of mines. 
* Flagging functionality to mark suspected mines. 
* Reset button to start a new game. 

**Prerequisites**
* Qt 5 or later 
* C++14 or later 
* Cmake 

# 2. Program Interface  
This program is designed to run on Qt Creator. You can start the game by just pressing 
the run button on Qt Creator. By using the quit button that is given by the operation system itself 
you can quit the game, by pressing the restart button you can restart the game and by pressing the 
hint button you can see the suggested cells that the game gives to you.  
You can click the matrix in the interface to reveal the grid. If the grid is a mine the game 
finishes and a Game Over text pops up. Also if you reveal all the cells without clicking a mine 
you win the game. In the left upper side, you can see your score which is dependent on the cells you 
reveal. 

# 3. Program Structure  
This program has three files, two Cpp files (main.cpp, minesweeper.cpp) and a header file 
(minesweeper.h). main.cpp file has the main function to run the program. main.cpp program runs 
the file. minesweeper.h is to connect main.cpp and minesweeper.cpp and includes the names of 
the functions.  

* Minesweeper() : initializes the game with a given grid size. 
* void handleButtonClick(): handles the left click on buttons, this can reveal the button 
* void handleRightClick(): handles the right click on buttons, flags the given cell if unflagged 
* and unflags the given cell if flagged 
* void restartGame(): initializes the game again *
* void hintFunc(): gives hints and turns the cell block to be green and if it is green reveals the 
* cell 
* void createBoard(): creates the board 
* void placeMines(): place the mines when initializing the board 
* int countNeighboringMines(int row, int col): gives the mine count of the neighbouring cells 
* void revealCell(int row, int col): reveals the cell and gives the right image 
* void revealEmptyCells(int row, int col): this is a helper function that reveals cells recursively 
* if needed 
* void checkWinCondition(int row, int col): checks if the game is won 
* void gameOver(bool won): checks if the game is over(won or lost)

* **For a detailed explanation of the program please read the description.pdf.**

