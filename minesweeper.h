#ifndef MINESWEEPER_H
#define MINESWEEPER_H

#include <QMainWindow>
#include <QPushButton>
#include <QGridLayout>
#include <QMessageBox>
#include <vector>
#include <QPixmap>
#include <QLabel>

class Minesweeper : public QMainWindow {
    Q_OBJECT

public:
    // Constructor to initialize the game with specified rows, columns, and mines
    Minesweeper(int rows = 10, int cols = 10, int mines = 10, QWidget *parent = nullptr);
    ~Minesweeper();

private slots:
    void handleButtonClick();  //function to handle left-click on buttons
    void handleRightClick();   //function to handle right-click on buttons
    void restartGame();        //function to handle restarting the game
    void hintFunc();           //function to provide hints

private:
    void createBoard();     //function to create boards
    void placeMines();         //function to place mines in map
    int countNeighboringMines(int row, int col);    //function to count the number of neighboring mines
    void revealCell(int row, int col);  //function to to reveal a cell in map
    void revealEmptyCells(int row, int col);    //function to  reveal empty cells around the clicked cell
    void checkWinCondition(int row, int col);   //function to check if the player has won the game
    void gameOver(bool won);    //function to  handle the game over scenario


    int rows; // Number of rows in the game board
    int cols; // Number of columns in the game board
    int mines; // Number of mines in the game board
    int flags; // Number of flags placed by the player
    int revealedCells; // Number of cells revealed by the player
    bool gameOverFlag; // Flag to indicate if the game is over
    bool hint; // Helper variable to hint function

    QGridLayout *gridLayout; // Layout to arrange the buttons in a grid
    QWidget *centralWidget; // Central widget for the main window

    std::vector<int> hintsList; // List to store coordinates of hint button
    std::vector<std::vector<QPushButton*>> buttons; // 2D vector to store the buttons on the board
    std::vector<std::vector<int>> board;        // 2D vector to store the board state
    std::vector<std::vector<bool>> revealed;    // 2D vector to store which cells have been revealed
    std::vector<std::vector<bool>> flagged;     // 2D vector to store which cells have been flagged
    std::vector<std::vector<int>> revealedN;    // 2D vector to store revealed cells in the hint function


    QPixmap images[9]; // Array to store images for numbers 0-8
    QLabel *scoreLabel; // Label to display the score
};

#endif // MINESWEEPER_H
