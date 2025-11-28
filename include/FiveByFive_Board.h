#pragma once
#ifndef FIVEBYFIVE_BOARD_H
#define FIVEBYFIVE_BOARD_H

#include "BoardGame_Classes.h"
#include <vector>
#include <iostream>

using namespace std;

class FiveByFive_Board : public Board<char> {
public:
    FiveByFive_Board() : Board(5, 5) {
        n_moves = 0;
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                board[i][j] = 0;
            }
        }
    }

    bool update_board(Move<char>* move) override {
        int x = move->get_x();
        int y = move->get_y();
        char symbol = move->get_symbol();

        if (x < 0 || x >= 5 || y < 0 || y >= 5) return false;
        if (board[x][y] != 0) return false;

        board[x][y] = symbol;
        n_moves++;
        return true;
    }

    bool is_win(Player<char>* player) override {
        return false; // In 5x5, win is determined by sequence count at the end
    }

    bool is_lose(Player<char>* player) override {
        return false;
    }

    bool is_draw(Player<char>* player) override {
        return n_moves >= 24; // Game ends when only 1 square remains empty
    }

    bool game_is_over(Player<char>* player) override {
        return n_moves >= 24; // 25 squares - 1 empty = 24 moves
    }

    void display_board() {
        cout << "\n   0   1   2   3   4\n";
        cout << "  -------------------\n";
        for (int i = 0; i < 5; i++) {
            cout << i << "|";
            for (int j = 0; j < 5; j++) {
                cout << " " << (board[i][j] == 0 ? ' ' : board[i][j]) << " |";
            }
            cout << "\n  -------------------\n";
        }
        cout << "Moves: " << n_moves << "/24\n"; // Show progress
    }

    vector<vector<char>> get_board_matrix() const {
        vector<vector<char>> matrix(5, vector<char>(5));
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                matrix[i][j] = board[i][j];
            }
        }
        return matrix;
    }

    // CORRECTED sequence counting
    int count_player_sequences(char symbol) const {
        int count = 0;

        // Check horizontal sequences
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == symbol &&
                    board[i][j + 1] == symbol &&
                    board[i][j + 2] == symbol) {
                    count++;
                    // Debug output
                    cout << "Horizontal sequence for " << symbol << " at row " << i
                        << " columns " << j << "-" << j + 2 << "\n";
                }
            }
        }

        // Check vertical sequences
        for (int j = 0; j < 5; j++) {
            for (int i = 0; i < 3; i++) {
                if (board[i][j] == symbol &&
                    board[i + 1][j] == symbol &&
                    board[i + 2][j] == symbol) {
                    count++;
                    cout << "Vertical sequence for " << symbol << " at column " << j
                        << " rows " << i << "-" << i + 2 << "\n";
                }
            }
        }

        // Check diagonal sequences (top-left to bottom-right)
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == symbol &&
                    board[i + 1][j + 1] == symbol &&
                    board[i + 2][j + 2] == symbol) {
                    count++;
                    cout << "Diagonal sequence for " << symbol << " at (" << i << "," << j
                        << ") to (" << i + 2 << "," << j + 2 << ")\n";
                }
            }
        }

        // Check diagonal sequences (top-right to bottom-left)
        for (int i = 0; i < 3; i++) {
            for (int j = 2; j < 5; j++) {
                if (board[i][j] == symbol &&
                    board[i + 1][j - 1] == symbol &&
                    board[i + 2][j - 2] == symbol) {
                    count++;
                    cout << "Anti-diagonal sequence for " << symbol << " at (" << i << "," << j
                        << ") to (" << i + 2 << "," << j - 2 << ")\n";
                }
            }
        }

        return count;
    }

    // Helper function for AI to check if move would create a sequence
    bool would_create_sequence(int x, int y, char symbol) const {
        // Create a test board
        vector<vector<char>> test_board(5, vector<char>(5));
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                test_board[i][j] = board[i][j];
            }
        }

        if (test_board[x][y] != 0) return false;
        test_board[x][y] = symbol;

        // Check horizontal
        for (int start_col = max(0, y - 2); start_col <= min(2, y); start_col++) {
            if (test_board[x][start_col] == symbol &&
                test_board[x][start_col + 1] == symbol &&
                test_board[x][start_col + 2] == symbol) {
                return true;
            }
        }

        // Check vertical
        for (int start_row = max(0, x - 2); start_row <= min(2, x); start_row++) {
            if (test_board[start_row][y] == symbol &&
                test_board[start_row + 1][y] == symbol &&
                test_board[start_row + 2][y] == symbol) {
                return true;
            }
        }

        // Check diagonals
        for (int offset = -2; offset <= 0; offset++) {
            int start_row = x + offset;
            int start_col = y + offset;
            if (start_row >= 0 && start_row <= 2 && start_col >= 0 && start_col <= 2) {
                if (test_board[start_row][start_col] == symbol &&
                    test_board[start_row + 1][start_col + 1] == symbol &&
                    test_board[start_row + 2][start_col + 2] == symbol) {
                    return true;
                }
            }
        }

        for (int offset = -2; offset <= 0; offset++) {
            int start_row = x + offset;
            int start_col = y - offset;
            if (start_row >= 0 && start_row <= 2 && start_col >= 2 && start_col < 5) {
                if (test_board[start_row][start_col] == symbol &&
                    test_board[start_row + 1][start_col - 1] == symbol &&
                    test_board[start_row + 2][start_col - 2] == symbol) {
                    return true;
                }
            }
        }

        return false;
    }
};

#endif