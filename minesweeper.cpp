#include "minesweeper.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPixmap>
#include <QLabel>


// Constructor for the Minesweeper game

Minesweeper::Minesweeper(int rows, int cols, int mines, QWidget *parent)
    : QMainWindow(parent), rows(rows), cols(cols), mines(mines), flags(0), revealedCells(0), gameOverFlag(false) {
    srand(time(0));

    centralWidget = new QWidget(this);
    gridLayout = new QGridLayout();

    // Load images for the cells
    for (int i = 0; i <= 8; ++i) {
        images[i].load(QString(":/%1.png").arg(i));
    }

    createBoard();
    hint=false;
    // Create layouts
    QVBoxLayout *mainLayout = new QVBoxLayout();
    QHBoxLayout *topLayout = new QHBoxLayout();

    // Create hint and restart buttons
    QPushButton *hintButton = new QPushButton("Hint");
    QPushButton *restartButton = new QPushButton("Restart");
    connect(hintButton, &QPushButton::clicked, this, &Minesweeper::hintFunc);
    connect(restartButton, &QPushButton::clicked, this, &Minesweeper::restartGame);

    scoreLabel = new QLabel("Score: 0");
    topLayout->addWidget(scoreLabel);
    topLayout->addWidget(hintButton);

    topLayout->addWidget(restartButton);

    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(gridLayout);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);
}

Minesweeper::~Minesweeper() {}

void Minesweeper::createBoard() {
    board.resize(rows, std::vector<int>(cols, 0)); // Initialize the game board
    revealed.resize(rows, std::vector<bool>(cols, false)); // Initialize revealed cells
    flagged.resize(rows, std::vector<bool>(cols, false)); // Initialize flagged cells
    buttons.resize(rows, std::vector<QPushButton*>(cols, nullptr)); // Initialize buttons

    placeMines(); // Place mines on the board

    gridLayout->setSpacing(0);
    gridLayout->setContentsMargins(0, 0, 0, 0);
    // Create buttons for each cell
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (board[r][c] != -1) {
                board[r][c] = countNeighboringMines(r, c);
            }
            QPushButton *button = new QPushButton();
            buttons[r][c] = button;
            connect(button, &QPushButton::clicked, this, &Minesweeper::handleButtonClick);
            button->setContextMenuPolicy(Qt::CustomContextMenu);
            connect(button, &QPushButton::customContextMenuRequested, this, &Minesweeper::handleRightClick);

            // Set all buttons to empty
            button->setStyleSheet("QPushButton {"
                                  "padding: 0px;"
                                  "margin: 0px;"
                                  "border: none;"
                                  "border-image: url(':/empty.png')"
                                  "}");
            button->setFixedSize(30, 30);
            gridLayout->addWidget(button, r, c);

        }
    }
}

// Function to place mines
void Minesweeper::placeMines() {
    int placedMines = 0;
    while (placedMines < mines) {
        int row = rand() % rows;
        int col = rand() % cols;
        if (board[row][col] != -1) {
            board[row][col] = -1;
            ++placedMines;
        }
    }
}

// Function to count neighboring mines
int Minesweeper::countNeighboringMines(int row, int col) {
    int count = 0;
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            int r = row + i;
            int c = col + j;
            if (r >= 0 && r < rows && c >= 0 && c < cols && board[r][c] == -1) {
                ++count;
            }
        }
    }
    return count;
}

// Function to reveal cell in map
void Minesweeper::revealCell(int row, int col) {
    if (row < 0 || row >= rows || col < 0 || col >= cols || revealed[row][col] || flagged[row][col]) {
        return; // Return if cell is out of bounds or already revealed/flagged
    }
    revealed[row][col] = true;
    ++revealedCells;
    scoreLabel->setText(QString("Score: %1").arg(revealedCells)); // Update score

    if (board[row][col] == -1) {
        // Reveal mine
        buttons[row][col]->setStyleSheet("QPushButton {"
                                         "padding: 0px;"
                                         "margin: 0px;"
                                         "border: none;"
                                         "border-image: url(':/mine.png')"
                                         "}");
        buttons[row][col]->setFixedSize(30, 30);
        gameOver(false); // Game over
    } else if (board[row][col] == 0) {
        // Reveal empty cell
        buttons[row][col]->setStyleSheet("QPushButton {"
                                         "padding: 0px;"
                                         "margin: 0px;"
                                         "border: none;"
                                         "border-image: url(':/0.png')"
                                         "}");
        buttons[row][col]->setFixedSize(30, 30);
        revealEmptyCells(row, col); // Reveal surrounding empty cells
    } else {
        // Reveal number cell
        buttons[row][col]->setStyleSheet("QPushButton {"
                                         "padding: 0px;"
                                         "margin: 0px;"
                                         "border: none;"
                                         "border-image: url(':/" + QString::number(board[row][col]) + ".png');"
                                                                              "}");
        buttons[row][col]->setFixedSize(30, 30);
    }
    checkWinCondition(row, col); // Check if the game is won
}

// Function to recursively reveal surrounding empty cells
void Minesweeper::revealEmptyCells(int row, int col) {
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            int r = row + i;
            int c = col + j;
            revealCell(r, c);
        }
    }
}

// Function to check if the player wins
void Minesweeper::checkWinCondition(int row , int col) {

    if (revealedCells == rows * cols - mines) {
        gameOver(true);
    }
}

// Function to get to game to finish state
void Minesweeper::gameOver(bool won) {
    gameOverFlag = true;

    // Reveal all mines when the game is over
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if ( board[r][c] == -1) {
                buttons[r][c]->setStyleSheet("QPushButton {"
                                             "padding: 0px;"
                                             "margin: 0px;"
                                             "border: none;"
                                             "border-image: url(':/mine.png')"
                                             "}");
                buttons[r][c]->setFixedSize(30, 30);
            }
        }
    }
    QString message = won ? "You win!" : "You lose!";
    QMessageBox::information(this, "minesweeper", message);
}
// Function to handle left-click events on buttons
void Minesweeper::handleButtonClick() {
    if (gameOverFlag) {
        return;
    }
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (button) {
        int row = -1, col = -1;
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; c++) {
                if (buttons[r][c] == button) {
                    row = r;
                    col = c;
                    break;
                }
            }
            if (row != -1) {
                break;
            }
        }
        if (row != -1 && col != -1) {
            revealCell(row, col);
        }
    }
}
// Function to handle right-click events for flagging
void Minesweeper::handleRightClick() {
    if (gameOverFlag) {
        return;
    }
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (button) {
        int row = -1, col = -1;
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                if (buttons[r][c] == button) {
                    row = r;
                    col = c;
                    break;
                }
            }
            if (row != -1) {
                break;
            }
        }
        if (row != -1 && col != -1) {
            if (!revealed[row][col]) {
                if(flagged[row][col]==false){
                    flagged[row][col] = !flagged[row][col];
                    button->setStyleSheet("QPushButton {"
                                          "padding: 0px;"
                                          "margin: 0px;"
                                          "border: none;"
                                          "border-image: url(':/flag.png')"
                                          "}");
                    button->setFixedSize(30, 30);
                }
                else{
                    flagged[row][col] = !flagged[row][col];
                    button->setStyleSheet("QPushButton {"
                                          "padding: 0px;"
                                          "margin: 0px;"
                                          "border: none;"
                                          "border-image: url(':/empty.png')"
                                          "}");
                    button->setFixedSize(30, 30);
                }
            }
        }
    }
}

// Function to restart the game
void Minesweeper::restartGame() {
    hint = false;
    gameOverFlag = false;
    revealedCells = 0;
    flags = 0;
    scoreLabel->setText("Score: 0");
    board.clear();
    revealed.clear();
    flagged.clear();
    createBoard();
}

// Function to handle hint logic and reveal a hint cell
void Minesweeper:: hintFunc(){
    // If hint is already active, reveal the hinted cell
    if(hint){
        revealCell(hintsList[0],hintsList[1]);
        hint=false;
        return;
    }
    revealedN.resize(rows, std::vector(cols, 0)); // Initialize the hint board
    int countMine = 0;
    int temp = -1;

    // Iterate until no new mines are found
    while(temp != countMine){
        temp = countMine;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (revealed[i][j]) {
                    int count = countNeighboringMines(i, j);
                    int nCount = 0;
                    for (int k = -1; k <= 1; k++) {
                        for (int l = -1; l <= 1; l++) {
                            int r = i + k;
                            int c = j + l;
                            if (r > -1 && r < rows && l > -1 && c < cols && c > -1 && !revealed[r][c]) {
                                if(revealedN[r][c] != 1){
                                    revealedN[r][c] = -1;
                                    nCount++;
                                }else if(revealedN[r][c] == 1){
                                    count--;
                                }
                            }
                        }
                    }
                    if(nCount == count){
                        countMine += count;
                        for (int k = -1; k <= 1; k++) {
                            for (int l = -1; l <= 1; l++) {
                                int r = i + k;
                                int c = j + l;
                                if (r > -1 && r < rows && l > -1 && c < cols && c > -1 && !revealed[r][c]) {
                                    revealedN[r][c] = 1;
                                }
                            }
                        }
                    }
                }
            }
        }
    }



    int hintr = -1;
    int hintc = -1;
    // Find a cell to provide as a hint
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if(revealed[i][j]){
                int count = countNeighboringMines(i, j);
                bool interCount = false;
                for (int k = -1; k <= 1; k++) {
                    for (int l = -1; l <= 1; l++) {
                        int r = i + k;
                        int c = j + l;
                        if (r > -1 && r < rows && l > -1 && c < cols && c > -1 && !revealed[r][c]) {
                            if(!interCount && revealedN[r][c] != 1){
                                if(board[r][c] == -1){
                                    continue;
                                }
                                interCount = true;
                                hintr = r;
                                hintc = c;
                            }else if(revealedN[r][c] == 1){
                                count--;
                            }
                        }
                    }
                }
                // Highlight the hint cell
                if(count == 0 && interCount){
                    buttons[hintr][hintc]->setStyleSheet("QPushButton {"
                                                                 "padding: 0px;"
                                                                 "margin: 0px;"
                                                                 "border: none;"
                                                                 "border-image: url(':/hint.png')"
                                                                 "}");
                    buttons[hintr][hintc]->setFixedSize(30, 30);
                    hintsList={hintr,hintc}; // Store the hint cell coordinates
                    hint = true;
                    return;
                }
            }
        }
    }
    hint = false;
}

