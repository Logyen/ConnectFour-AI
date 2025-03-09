#include <iostream>
#include <vector>
#include <limits>

using namespace std;

const int ROWS = 6;
const int COLS = 7;
const char PLAYER_MARKER = 'X';
const char AI_MARKER = 'O';
const char EMPTY = ' ';

// Function to print the board
void printBoard(const vector<vector<char>>& board) {
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            cout << "| " << board[i][j] << " ";
        }
        cout << "|\n";
    }
    cout << "-----------------------------\n  0   1   2   3   4   5   6\n";
}

// Function to check if a player has won
bool checkWin(const vector<vector<char>>& board, char marker) {
    // Check rows
    for (int r = 0; r < ROWS; ++r)
        for (int c = 0; c < COLS - 3; ++c)
            if (board[r][c] == marker && board[r][c + 1] == marker && board[r][c + 2] == marker && board[r][c + 3] == marker)
                return true;

    // Check columns
    for (int r = 0; r < ROWS - 3; ++r)
        for (int c = 0; c < COLS; ++c)
            if (board[r][c] == marker && board[r + 1][c] == marker && board[r + 2][c] == marker && board[r + 3][c] == marker)
                return true;

    // Check main diagonal
    for (int r = 0; r < ROWS - 3; ++r)
        for (int c = 0; c < COLS - 3; ++c)
            if (board[r][c] == marker && board[r + 1][c + 1] == marker && board[r + 2][c + 2] == marker && board[r + 3][c + 3] == marker)
                return true;

    // Check reverse diagonal
    for (int r = 3; r < ROWS; ++r)
        for (int c = 0; c < COLS - 3; ++c)
            if (board[r][c] == marker && board[r - 1][c + 1] == marker && board[r - 2][c + 2] == marker && board[r - 3][c + 3] == marker)
                return true;

    return false;
}

// Function to check if the board is full
bool boardFull(const vector<vector<char>>& board) {
    for (int c = 0; c < COLS; ++c)
        if (board[0][c] == EMPTY)
            return false;
    return true;
}

// Function to drop a piece in a column
bool dropPiece(vector<vector<char>>& board, int col, char marker) {
    if (board[0][col] != EMPTY)
        return false;
    for (int r = ROWS - 1; r >= 0; --r) {
        if (board[r][col] == EMPTY) {
            board[r][col] = marker;
            return true;
        }
    }
    return false;
}

// Minimax function with Alpha-Beta Pruning
int minimax(vector<vector<char>>& board, int depth, bool isMaximizing, int alpha, int beta) {
    if (checkWin(board, AI_MARKER))
        return 100 - depth;
    if (checkWin(board, PLAYER_MARKER))
        return -100 + depth;
    if (boardFull(board))
        return 0;

    if (isMaximizing) {
        int maxScore = -1000;
        for (int c = 0; c < COLS; ++c) {
            vector<vector<char>> tempBoard = board;
            if (dropPiece(tempBoard, c, AI_MARKER)) {
                int score = minimax(tempBoard, depth + 1, false, alpha, beta);
                maxScore = max(maxScore, score);
                alpha = max(alpha, score);
                if (beta <= alpha)
                    break;
            }
        }
        return maxScore;
    } else {
        int minScore = 1000;
        for (int c = 0; c < COLS; ++c) {
            vector<vector<char>> tempBoard = board;
            if (dropPiece(tempBoard, c, PLAYER_MARKER)) {
                int score = minimax(tempBoard, depth + 1, true, alpha, beta);
                minScore = min(minScore, score);
                beta = min(beta, score);
                if (beta <= alpha)
                    break;
            }
        }
        return minScore;
    }
}

// Function to find the best move for AI
int findBestMove(vector<vector<char>>& board) {
    int bestScore = -1000;
    int bestMove = -1;

    for (int c = 0; c < COLS; ++c) {
        vector<vector<char>> tempBoard = board;
        if (dropPiece(tempBoard, c, AI_MARKER)) {
            int score = minimax(tempBoard, 0, false, -1000, 1000);
            if (score > bestScore) {
                bestScore = score;
                bestMove = c;
            }
        }
    }

    return bestMove;
}

// Game loop
void playGame() {
    vector<vector<char>> board(ROWS, vector<char>(COLS, EMPTY));
    cout << "Welcome to Connect Four!\nYou are playing as " << PLAYER_MARKER << " against the AI (" << AI_MARKER << ").\n";
    printBoard(board);
    bool playerTurn = true;

    while (true) {
        int col;
        if (playerTurn) {
            cout << "Enter the column (0-6) to drop your piece: ";
            cin >> col;
            if (col < 0 || col >= COLS || !dropPiece(board, col, PLAYER_MARKER)) {
                cout << "Invalid move! Try again.\n";
                continue;
            }
        } else {
            cout << "AI's turn...\n";
            col = findBestMove(board);
            dropPiece(board, col, AI_MARKER);
            cout << "AI placed its piece in column " << col << endl;
        }

        printBoard(board);

        if (checkWin(board, PLAYER_MARKER)) {
            cout << "You won!\n";
            break;
        } else if (checkWin(board, AI_MARKER)) {
            cout << "AI won!\n";
            break;
        } else if (boardFull(board)) {
            cout << "It's a draw!\n";
            break;
        }

        playerTurn = !playerTurn;
    }
}

// Main function
int main() {
    playGame();
    return 0;
}
