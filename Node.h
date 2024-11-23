#include "Config.h"

#ifndef NODE_H
#define NODE_H

#include <string>

struct Node {
    char color;       // Candy color: R, G, B, Y
    bool hasStar;     // True if this node contains a star
    int row, col;     // Position in the grid
    Node *up, *down, *left, *right;

    Node(char c = ' ', bool star = false, int r = 0, int col = 0)
        : color(c), hasStar(star), row(r), col(col), up(nullptr), down(nullptr), left(nullptr), right(nullptr) {}
};

// Function prototypes
void initializeGrid(Node* grid[N][N]);
void placeStars(Node* grid[N][N]);
bool ensureValidMoves(Node* grid[N][N]);
bool makeMove(Node* grid[N][N], int row1, int col1, int row2, int col2);
void displayGrid(Node* grid[N][N], int movesRemaining, int score, int starsCollected);
void matchAndReplace(Node* grid[N][N], int& score, int& starsCollected);

#endif
