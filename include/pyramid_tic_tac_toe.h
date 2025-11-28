#ifndef PYRAMID_TIC_TAC_TOE_H
#define PYRAMID_TIC_TAC_TOE_H

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <map>

using namespace std;

class PyramidTicTacToe {
private:
    static const int ROWS = 3;
    vector<vector<char>> board;
    char currentPlayer;
    bool gameOver;
    char winner;
    int movesCount;

    // CORRECTED winning line definitions for pyramid
    vector<vector<pair<int, int>>> winning_lines = {
        // Horizontal lines (only valid straight lines)
        {{1,0}, {1,1}, {1,2}}, // Middle row - valid straight line

        // Base row horizontal lines (only consecutive positions)
        {{2,0}, {2,1}, {2,2}}, // Base row first triple
        {{2,1}, {2,2}, {2,3}}, // Base row second triple  
        {{2,2}, {2,3}, {2,4}}, // Base row third triple

        // Vertical lines (only valid straight lines)
        {{2,0}, {1,0}, {0,0}}, // Left side - valid straight line
        {{2,2}, {1,1}, {0,0}}, // Center - valid straight line
        {{2,4}, {1,2}, {0,0}}, // Right side - valid straight line

        // Diagonal lines (only valid straight diagonals)
        {{2,0}, {1,1}, {0,0}}, // Left diagonal - valid
        {{2,4}, {1,2}, {0,0}}  // Right diagonal - valid
    };

    // Enhanced evaluation weights for pyramid positions
    map<pair<int, int>, int> position_weights = {
        {{0,0}, 10}, // Top - most valuable

        {{1,0}, 4},  // Middle left
        {{1,1}, 8},  // Middle center - high value
        {{1,2}, 4},  // Middle right

        {{2,0}, 3},  // Base left
        {{2,1}, 5},  // Base left-center
        {{2,2}, 6},  // Base center - high value
        {{2,3}, 5},  // Base right-center
        {{2,4}, 3}   // Base right
    };

    // Enhanced AI helper methods
    bool wouldWin(char player, int row, int col) const {
        vector<vector<char>> testBoard = board;
        if (!isValidPyramidPosition(row, col) || testBoard[row][col] != ' ') return false;

        testBoard[row][col] = player;
        return checkWin(testBoard);
    }

    bool checkWin(const vector<vector<char>>& testBoard) const {
        for (const auto& line : winning_lines) {
            char first = testBoard[line[0].first][line[0].second];
            if (first != ' ' &&
                first == testBoard[line[1].first][line[1].second] &&
                first == testBoard[line[2].first][line[2].second]) {
                return true;
            }
        }
        return false;
    }

    // FIXED: Added checkWin() without parameters that uses current board
    bool checkWin() const {
        return checkWin(board);
    }

    bool isValidPyramidPosition(int row, int col) const {
        if (row < 0 || row >= ROWS) return false;
        switch (row) {
        case 0: return col == 0;
        case 1: return col >= 0 && col < 3;
        case 2: return col >= 0 && col < 5;
        default: return false;
        }
    }

    bool isBoardFull() const {
        for (size_t row = 0; row < board.size(); row++) {
            for (size_t col = 0; col < board[row].size(); col++) {
                if (board[row][col] == ' ') return false;
            }
        }
        return true;
    }

    // ENHANCED EVALUATION FUNCTIONS

    int evaluatePosition(int row, int col, char player) const {
        int score = position_weights.at({ row, col });

        // Check immediate winning opportunities
        if (wouldWin(player, row, col)) {
            score += 1000;
        }

        // Check if this blocks opponent from winning
        char opponent = (player == 'X') ? 'O' : 'X';
        if (wouldWin(opponent, row, col)) {
            score += 500;
        }

        // Evaluate control of key lines
        score += evaluateLineControl(row, col, player) * 10;

        return score;
    }

    int evaluateLineControl(int row, int col, char player) const {
        int control = 0;
        char opponent = (player == 'X') ? 'O' : 'X';

        for (const auto& line : winning_lines) {
            bool onLine = false;
            int playerCount = 0;
            int opponentCount = 0;
            int emptyCount = 0;

            for (const auto& pos : line) {
                if (pos.first == row && pos.second == col) {
                    onLine = true;
                }
                if (board[pos.first][pos.second] == player) playerCount++;
                else if (board[pos.first][pos.second] == opponent) opponentCount++;
                else emptyCount++;
            }

            if (onLine) {
                if (playerCount == 2 && emptyCount == 1) control += 10;
                else if (playerCount == 1 && emptyCount == 2) control += 3;
                else if (opponentCount == 2 && emptyCount == 1) control += 8;
            }
        }

        return control;
    }

    // ENHANCED MINIMAX WITH ALPHA-BETA PRUNING
    int minimax(int depth, bool isMaximizing, int alpha, int beta, int maxDepth) {
        if (checkWin()) {
            return isMaximizing ? -1000 + depth : 1000 - depth;
        }
        if (isBoardFull() || depth >= maxDepth) {
            return evaluateBoardState();
        }

        if (isMaximizing) {
            int maxEval = -10000;
            for (int row = 0; row < ROWS; row++) {
                int max_col = (row == 0) ? 1 : (row == 1) ? 3 : 5;
                for (int col = 0; col < max_col; col++) {
                    if (board[row][col] == ' ') {
                        board[row][col] = 'O';
                        movesCount++;
                        int eval = minimax(depth + 1, false, alpha, beta, maxDepth);
                        board[row][col] = ' ';
                        movesCount--;
                        maxEval = max(maxEval, eval);
                        alpha = max(alpha, eval);
                        if (beta <= alpha) break;
                    }
                }
            }
            return maxEval;
        }
        else {
            int minEval = 10000;
            for (int row = 0; row < ROWS; row++) {
                int max_col = (row == 0) ? 1 : (row == 1) ? 3 : 5;
                for (int col = 0; col < max_col; col++) {
                    if (board[row][col] == ' ') {
                        board[row][col] = 'X';
                        movesCount++;
                        int eval = minimax(depth + 1, true, alpha, beta, maxDepth);
                        board[row][col] = ' ';
                        movesCount--;
                        minEval = min(minEval, eval);
                        beta = min(beta, eval);
                        if (beta <= alpha) break;
                    }
                }
            }
            return minEval;
        }
    }

    int evaluateBoardState() const {
        int score = 0;

        // Evaluate based on position control
        for (int row = 0; row < ROWS; row++) {
            int max_col = (row == 0) ? 1 : (row == 1) ? 3 : 5;
            for (int col = 0; col < max_col; col++) {
                if (board[row][col] == 'O') {
                    score += position_weights.at({ row, col });
                }
                else if (board[row][col] == 'X') {
                    score -= position_weights.at({ row, col });
                }
            }
        }

        // Evaluate line control
        for (const auto& line : winning_lines) {
            int oCount = 0, xCount = 0, emptyCount = 0;
            for (const auto& pos : line) {
                if (board[pos.first][pos.second] == 'O') oCount++;
                else if (board[pos.first][pos.second] == 'X') xCount++;
                else emptyCount++;
            }

            if (oCount == 2 && emptyCount == 1) score += 20;
            if (xCount == 2 && emptyCount == 1) score -= 20;
            if (oCount == 1 && emptyCount == 2) score += 5;
            if (xCount == 1 && emptyCount == 2) score -= 5;
        }

        return score;
    }

    // ENHANCED MOVE SELECTION
    pair<int, int> findBestMove() {
        // Check for immediate win
        for (int row = 0; row < ROWS; row++) {
            int max_col = (row == 0) ? 1 : (row == 1) ? 3 : 5;
            for (int col = 0; col < max_col; col++) {
                if (board[row][col] == ' ') {
                    if (wouldWin('O', row, col)) {
                        return { row, col };
                    }
                }
            }
        }

        // Check for immediate block
        for (int row = 0; row < ROWS; row++) {
            int max_col = (row == 0) ? 1 : (row == 1) ? 3 : 5;
            for (int col = 0; col < max_col; col++) {
                if (board[row][col] == ' ') {
                    if (wouldWin('X', row, col)) {
                        return { row, col };
                    }
                }
            }
        }

        // Use minimax with adaptive depth
        int bestScore = -10000;
        pair<int, int> bestMove = { -1, -1 };

        int maxDepth = (movesCount < 4) ? 3 : (movesCount < 6) ? 4 : 5;

        for (int row = 0; row < ROWS; row++) {
            int max_col = (row == 0) ? 1 : (row == 1) ? 3 : 5;
            for (int col = 0; col < max_col; col++) {
                if (board[row][col] == ' ') {
                    board[row][col] = 'O';
                    movesCount++;
                    int score = minimax(0, false, -10000, 10000, maxDepth);
                    board[row][col] = ' ';
                    movesCount--;

                    if (score > bestScore) {
                        bestScore = score;
                        bestMove = { row, col };
                    }
                }
            }
        }

        if (bestMove.first == -1) {
            return findHeuristicMove();
        }

        return bestMove;
    }

    pair<int, int> findHeuristicMove() {
        int bestScore = -10000;
        pair<int, int> bestMove = { -1, -1 };

        for (int row = 0; row < ROWS; row++) {
            int max_col = (row == 0) ? 1 : (row == 1) ? 3 : 5;
            for (int col = 0; col < max_col; col++) {
                if (board[row][col] == ' ') {
                    int score = evaluatePosition(row, col, 'O');
                    if (score > bestScore) {
                        bestScore = score;
                        bestMove = { row, col };
                    }
                }
            }
        }

        return bestMove;
    }

    pair<int, int> getRandomMove() const {
        vector<pair<int, int>> availableMoves;
        for (int row = 0; row < ROWS; row++) {
            int max_col = (row == 0) ? 1 : (row == 1) ? 3 : 5;
            for (int col = 0; col < max_col; col++) {
                if (board[row][col] == ' ') {
                    availableMoves.push_back({ row, col });
                }
            }
        }
        if (availableMoves.empty()) return { -1, -1 };
        return availableMoves[rand() % availableMoves.size()];
    }

public:
    PyramidTicTacToe() { reset(); }

    void reset() {
        board.clear();
        board.resize(ROWS);
        board[0] = vector<char>(1, ' ');
        board[1] = vector<char>(3, ' ');
        board[2] = vector<char>(5, ' ');
        currentPlayer = 'X';
        gameOver = false;
        winner = ' ';
        movesCount = 0;
    }

    bool makeMove(int row, int col) {
        if (!isValidMove(row, col)) return false;
        board[row][col] = currentPlayer;
        movesCount++;

        // FIXED: Use the correct checkWin() call
        if (checkWin()) {
            gameOver = true;
            winner = currentPlayer;
        }
        else if (isBoardFull()) {
            gameOver = true;
            winner = ' ';
        }
        else {
            currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
        }
        return true;
    }

    bool isValidMove(int row, int col) const {
        if (gameOver) return false;
        if (!isValidPyramidPosition(row, col)) return false;
        return board[row][col] == ' ';
    }

    pair<int, int> getComputerMove(bool smartAI) {
        return smartAI ? findBestMove() : getRandomMove();
    }

    void displayBoard() const {
        cout << "\n    PYRAMID TIC-TAC-TOE\n";
        cout << "    ==================\n\n";
        cout << "Coordinates:\n";
        cout << "Row 0: [0,0]\n";
        cout << "Row 1: [1,0] [1,1] [1,2]\n";
        cout << "Row 2: [2,0] [2,1] [2,2] [2,3] [2,4]\n\n";
        cout << "Current Board:\n\n";
        cout << string(10, ' ') << "[" << board[0][0] << "]" << endl;
        cout << string(6, ' ');
        for (size_t col = 0; col < board[1].size(); col++) {
            cout << "[" << board[1][col] << "] ";
        }
        cout << endl << "  ";
        for (size_t col = 0; col < board[2].size(); col++) {
            cout << "[" << board[2][col] << "] ";
        }
        cout << endl << endl;
    }

    string getGameState() const {
        if (gameOver) {
            return (winner != ' ') ? "Player " + string(1, winner) + " wins!" : "Game ended in a draw!";
        }
        return "Player " + string(1, currentPlayer) + "'s turn";
    }

    bool isGameOver() const { return gameOver; }
    char getWinner() const { return winner; }
    char getCurrentPlayer() const { return currentPlayer; }
    int getMoveCount() const { return movesCount; }

    // For integration with Simple_UI
    vector<vector<char>> getBoardMatrix() const {
        return board;
    }

    // DEBUG: Print winning lines for verification
    void debugPrintWinningLines() const {
        cout << "DEBUG: Valid Winning Lines:\n";
        for (const auto& line : winning_lines) {
            cout << "Line: ";
            for (const auto& pos : line) {
                cout << "[" << pos.first << "," << pos.second << "] ";
            }
            cout << endl;
        }
    }
};

#endif // PYRAMID_TIC_TAC_TOE_H