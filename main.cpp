#include "Node.h"
#include <iostream>
#include "Config.h"

using namespace std;

int main() {
    Node* grid[N][N];
    int movesRemaining = 20, score = 0, starsCollected = 0;

    initializeGrid(grid);
    placeStars(grid);
    ensureValidMoves(grid);

    while (movesRemaining > 0 && starsCollected < 5) {
        displayGrid(grid, movesRemaining, score, starsCollected);

        int row1, col1, row2, col2;
        cout << "Enter the row and column of the first candy: ";
        cin >> row1 >> col1;
        cout << "Enter the row and column of the second candy: ";
        cin >> row2 >> col2;

        if (makeMove(grid, row1, col1, row2, col2)) {
            matchAndReplace(grid, score, starsCollected);
        } else {
            cout << "Invalid move! Try again.\n";
        }
        movesRemaining--;
    }

    if (starsCollected == 5) {
        cout << "Congratulations! You collected all stars!\n";
    } else {
        cout << "Game Over! You ran out of moves.\n";
    }

    return 0;
}
