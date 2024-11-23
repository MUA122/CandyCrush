#include "Node.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <set>
#include "Config.h"

using namespace std;

// Initialize the grid with linked nodes
void initializeGrid(Node* grid[N][N]) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            grid[i][j] = new Node(' ', false, i, j);

            if (i > 0) {
                grid[i][j]->up = grid[i - 1][j];
                grid[i - 1][j]->down = grid[i][j];
            }
            if (j > 0) {
                grid[i][j]->left = grid[i][j - 1];
                grid[i][j - 1]->right = grid[i][j];
            }
        }
    }
}

// Place stars randomly on the grid
void placeStars(Node* grid[N][N]) {
    srand(time(0));
    for (int i = 0; i < 5; ++i) {
        int r = rand() % N;
        int c = rand() % N;
        while (grid[r][c]->hasStar) {  // Ensure unique positions
            r = rand() % N;
            c = rand() % N;
        }
        grid[r][c]->hasStar = true;
    }
}

// Assign random colors and ensure at least one valid move exists
bool ensureValidMoves(Node* grid[N][N]) {
    const char colors[] = {'R', 'G', 'B', 'Y'};
    bool validMove = false;

    // Randomly assign colors
    do {
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                grid[i][j]->color = colors[rand() % 4];
            }
        }

        // Check for at least one valid move
        validMove = false;
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                // TODO: correct the check to check that there exists a node with a distance of 2 that can be moved to make a match
                if (j < N - 1 && grid[i][j]->color == grid[i][j + 1]->color) {
                    validMove = true;
                }
                if (i < N - 1 && grid[i][j]->color == grid[i + 1][j]->color) {
                    validMove = true;
                }
            }
        }
    } while (!validMove);

    return validMove;
}

bool makeMove(Node* grid[N][N], int row1, int col1, int row2, int col2) {
    // TODO: Don't move stars
    // Validate that the selected nodes are adjacent
    if (abs(row1 - row2) + abs(col1 - col2) != 1) {
        return false; // Nodes are not adjacent
    }

    // Swap the colors of the two nodes
    char tempColor = grid[row1][col1]->color;
    grid[row1][col1]->color = grid[row2][col2]->color;
    grid[row2][col2]->color = tempColor;

    // TODO: Somehow return the number of the right, left, up, and down matches
    // Check if the move creates a valid match
    auto createsMatch = [](Node* grid[N][N], int row, int col) {
        char color = grid[row][col]->color;
        int countHorizontal = 1, countVertical = 1;

        // Check horizontally
        for (int c = col - 1; c >= 0 && grid[row][c]->color == color; c--) countHorizontal++;
        for (int c = col + 1; c < N && grid[row][c]->color == color; c++) countHorizontal++;

        // Check vertically
        for (int r = row - 1; r >= 0 && grid[r][col]->color == color; r--) countVertical++;
        for (int r = row + 1; r < N && grid[r][col]->color == color; r++) countVertical++;

        // Return true if a match of 3 or more candies is found
        return (countHorizontal >= 3 || countVertical >= 3);
    };

    // TODO: Modify this a little to check up+down+1 >= 3, invalidate their nodes
    //          and check left+right+1 >= 3, invalidate their nodes
    //          and keep shifting down and generating new nodes at the top to make up for the invalidated nodes
    //          Repeat the same process for the two swapped nodes.
    //          Make sure the grid contains at least one valid move
    //         HINT: You can utilize the below matchAndReplace function
    bool validMatch = createsMatch(grid, row1, col1) || createsMatch(grid, row2, col2);

    // TODO: else if up+down+1 < 3 and left+right+1 < 3, re-swap the colors and return false
    if (!validMatch) {
        // Undo the swap if the move is invalid
        grid[row2][col2]->color = grid[row1][col1]->color;
        grid[row1][col1]->color = tempColor;
        return false; // Invalid move
    }

    return true; // Valid move
}

// Display the grid
void displayGrid(Node* grid[N][N], int movesRemaining, int score, int starsCollected) {
    cout << "Moves Remaining: " << movesRemaining
         << " | Score: " << score
         << " | Stars Collected: " << starsCollected << "/5\n";

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (grid[i][j]->hasStar) {
                // TODO: Show the star node as a star without a color
                cout << "[" << grid[i][j]->color << "*] ";
            } else {
                cout << "[" << grid[i][j]->color << "] ";
            }
        }
        cout << "\n";
    }
}

// TODO: Modify this to just remove the matching and shift some nodes down
// Match detection and replacement
void matchAndReplace(Node* grid[N][N], int& score, int& starsCollected) {
    bool matchFound = false;
    set<Node*> toRemove;

    // Horizontal matches
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N-2; ++j) {
            if (grid[i][j]->color == grid[i][j + 1]->color &&
                grid[i][j]->color == grid[i][j + 2]->color) {
                toRemove.insert(grid[i][j]);
                toRemove.insert(grid[i][j + 1]);
                toRemove.insert(grid[i][j + 2]);
                matchFound = true;
            }
        }
    }

    // Vertical matches
    for (int i = 0; i < N-2; ++i) {
        for (int j = 0; j < N; ++j) {
            if (grid[i][j]->color == grid[i + 1][j]->color &&
                grid[i][j]->color == grid[i + 2][j]->color) {
                toRemove.insert(grid[i][j]);
                toRemove.insert(grid[i + 1][j]);
                toRemove.insert(grid[i + 2][j]);
                matchFound = true;
            }
        }
    }

    // Remove matched candies
    for (Node* node : toRemove) {
        if (node->hasStar) {
            starsCollected++;
        }
        node->color = ' ';  // Empty node
    }

    // Refill grid with random colors
    if (matchFound) {
        const char colors[] = {'R', 'G', 'B', 'Y'};
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                if (grid[i][j]->color == ' ') {
                    grid[i][j]->color = colors[rand() % 4];
                }
            }
        }
        score += toRemove.size();
    }
}
