#include <QApplication>
#include "minesweeper.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv); // Initialize the QApplication with command line arguments
    Minesweeper window; // Create an instance of the Minesweeper game window
    window.show(); // Show the game window
    return app.exec(); // Enter the main event loop
}
