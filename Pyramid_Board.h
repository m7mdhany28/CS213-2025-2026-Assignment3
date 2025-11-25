#pragma once
#ifndef PYRAMID_BOARD_H
#define PYRAMID_BOARD_H

#include "BoardGame_Classes.h"
#include <vector>
#include <iostream>

using namespace std;

class Pyramid_Board : public Board<char> {
private:
    // Track valid pyramid positions
    vector<pair<int, int>> valid_positions;

public:
    Pyramid_Board() : Board(3, 5) {  // 3 rows, 5 columns to accommodate pyramid
        n_moves = 0;

        // Initialize empty board
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 5; j++) {
                board[i][j] = 0;
            }
        }

        // Set up pyramid structure
        setup_pyramid_structure();
    }

    bool update_board(Move<char>* move) override {
        int x = move->get_x();
        int y = move->get_y();
        char symbol = move->get_symbol();

        // Check if position is valid in pyramid
        if (!is_valid_pyramid_position(x, y)) {
            return false;
        }

        if (x >= 0 && x < 3 && y >= 0 && y < 5 && board[x][y] == 0) {
            board[x][y] = symbol;
            n_moves++;
            return true;
        }
        return false;
    }

    bool is_win(Player<char>* player) override {
        char symbol = player->get_symbol();

        // Check all possible winning sequences in the pyramid

        // Horizontal wins
        // Row 2 (base): positions 0,1,2,3,4
        if (board[2][0] == symbol && board[2][1] == symbol && board[2][2] == symbol) return true;
        if (board[2][1] == symbol && board[2][2] == symbol && board[2][3] == symbol) return true;
        if (board[2][2] == symbol && board[2][3] == symbol && board[2][4] == symbol) return true;

        // Row 1 (middle): positions 1,2,3
        if (board[1][1] == symbol && board[1][2] == symbol && board[1][3] == symbol) return true;

        // Vertical wins
        // Left side
        if (board[0][2] == symbol && board[1][1] == symbol && board[2][0] == symbol) return true;
        // Middle
        if (board[0][2] == symbol && board[1][2] == symbol && board[2][2] == symbol) return true;
        // Right side
        if (board[0][2] == symbol && board[1][3] == symbol && board[2][4] == symbol) return true;

        // Diagonal wins
        // Left diagonal
        if (board[2][0] == symbol && board[1][1] == symbol && board[0][2] == symbol) return true;
        if (board[2][1] == symbol && board[1][2] == symbol && board[0][2] == symbol) return true;
        // Right diagonal
        if (board[2][3] == symbol && board[1][2] == symbol && board[0][2] == symbol) return true;
        if (board[2][4] == symbol && board[1][3] == symbol && board[0][2] == symbol) return true;

        // Base diagonals
        if (board[2][0] == symbol && board[2][1] == symbol && board[1][1] == symbol) return true;
        if (board[2][1] == symbol && board[2][2] == symbol && board[1][2] == symbol) return true;
        if (board[2][2] == symbol && board[2][3] == symbol && board[1][2] == symbol) return true;
        if (board[2][3] == symbol && board[2][4] == symbol && board[1][3] == symbol) return true;

        return false;
    }

    bool is_lose(Player<char>* player) override {
        return false;
    }

    bool is_draw(Player<char>* player) override {
        // Check if all valid positions are filled
        for (auto pos : valid_positions) {
            if (board[pos.first][pos.second] == 0) {
                return false;
            }
        }
        return !is_win(player);
    }

    bool game_is_over(Player<char>* player) override {
        return is_win(player) || is_draw(player);
    }

    bool is_valid_pyramid_position(int x, int y) const {
        for (auto pos : valid_positions) {
            if (pos.first == x && pos.second == y) {
                return true;
            }
        }
        return false;
    }

    void display_pyramid() const {
        cout << "\nPyramid Board:\n";
        cout << "        ";
        if (is_valid_pyramid_position(0, 2)) {
            cout << (board[0][2] == 0 ? '.' : board[0][2]);
        }
        cout << "\n";

        cout << "      ";
        for (int j = 1; j <= 3; j++) {
            if (is_valid_pyramid_position(1, j)) {
                cout << (board[1][j] == 0 ? '.' : board[1][j]) << " ";
            }
        }
        cout << "\n";

        cout << "    ";
        for (int j = 0; j < 5; j++) {
            if (is_valid_pyramid_position(2, j)) {
                cout << (board[2][j] == 0 ? '.' : board[2][j]) << " ";
            }
        }
        cout << "\n";

        cout << "Coordinates:\n";
        cout << "        (0,2)\n";
        cout << "      (1,1) (1,2) (1,3)\n";
        cout << "    (2,0) (2,1) (2,2) (2,3) (2,4)\n";
    }

    vector<pair<int, int>> get_valid_positions() const {
        return valid_positions;
    }

private:
    void setup_pyramid_structure() {
        // Pyramid structure:
        // Row 0: 1 position (center top) - position 2
        valid_positions.push_back({ 0, 2 });

        // Row 1: 3 positions (positions 1,2,3)
        valid_positions.push_back({ 1, 1 });
        valid_positions.push_back({ 1, 2 });
        valid_positions.push_back({ 1, 3 });

        // Row 2: 5 positions (positions 0,1,2,3,4)
        valid_positions.push_back({ 2, 0 });
        valid_positions.push_back({ 2, 1 });
        valid_positions.push_back({ 2, 2 });
        valid_positions.push_back({ 2, 3 });
        valid_positions.push_back({ 2, 4 });
    }
};

#endif