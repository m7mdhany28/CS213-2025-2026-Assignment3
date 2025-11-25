#pragma once
#ifndef INFINITY_BOARD_H
#define INFINITY_BOARD_H

#include "BoardGame_Classes.h"
#include <queue>
#include <utility>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Infinity_Board : public Board<char> {
private:
    vector<vector<int>> move_timestamps; // Track when each cell was last marked
    int total_moves;
    int moves_since_last_removal;

public:
    Infinity_Board() : Board(3, 3), total_moves(0), moves_since_last_removal(0) {
        move_timestamps.resize(3, vector<int>(3, -1)); // -1 means empty
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                board[i][j] = 0;
            }
        }
    }

    bool update_board(Move<char>* move) override {
        int x = move->get_x();
        int y = move->get_y();
        char symbol = move->get_symbol();

        if (x < 0 || x >= 3 || y < 0 || y >= 3) return false;
        if (board[x][y] != 0) return false;

        // Remove oldest mark every 3 moves
        if (total_moves >= 3 && (total_moves + 1) % 3 == 0) {
            remove_oldest_mark();
        }

        // Place new mark
        board[x][y] = symbol;
        move_timestamps[x][y] = total_moves;
        total_moves++;
        n_moves++;
        moves_since_last_removal++;

        cout << symbol << " placed at (" << x << "," << y << "). ";
        cout << "Total moves: " << total_moves << "\n";

        return true;
    }

    bool is_win(Player<char>* player) override {
        char symbol = player->get_symbol();

        // Check rows
        for (int row = 0; row < 3; row++) {
            if (board[row][0] == symbol && board[row][1] == symbol && board[row][2] == symbol) {
                cout << "Win detected in row " << row << "\n";
                return true;
            }
        }

        // Check columns
        for (int col = 0; col < 3; col++) {
            if (board[0][col] == symbol && board[1][col] == symbol && board[2][col] == symbol) {
                cout << "Win detected in column " << col << "\n";
                return true;
            }
        }

        // Check diagonals
        if (board[0][0] == symbol && board[1][1] == symbol && board[2][2] == symbol) {
            cout << "Win detected in main diagonal\n";
            return true;
        }
        if (board[0][2] == symbol && board[1][1] == symbol && board[2][0] == symbol) {
            cout << "Win detected in anti-diagonal\n";
            return true;
        }

        return false;
    }

    bool is_lose(Player<char>* player) override {
        return false;
    }

    bool is_draw(Player<char>* player) override {
        // In infinity mode, game continues indefinitely unless someone wins
        return false;
    }

    bool game_is_over(Player<char>* player) override {
        return is_win(player);
    }

    void display_board() {
        cout << "\n   0   1   2\n";
        cout << "  -----------\n";
        for (int i = 0; i < 3; i++) {
            cout << i << "|";
            for (int j = 0; j < 3; j++) {
                cout << " " << (board[i][j] == 0 ? ' ' : board[i][j]) << " |";
            }
            cout << "\n  -----------\n";
        }

        // Debug: Show move timestamps
        cout << "Move timestamps:\n";
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (move_timestamps[i][j] != -1) {
                    cout << "(" << i << "," << j << "): move " << move_timestamps[i][j] << "  ";
                }
            }
        }
        cout << "\n";
    }

    int get_removal_countdown() const {
        return 3 - (total_moves % 3);
    }

    int get_total_moves() const {
        return total_moves;
    }

private:
    void remove_oldest_mark() {
        int oldest_time = total_moves + 1; // Initialize with future value
        pair<int, int> oldest_pos = { -1, -1 };

        // Find the oldest mark on the board
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] != 0 && move_timestamps[i][j] < oldest_time) {
                    oldest_time = move_timestamps[i][j];
                    oldest_pos = { i, j };
                }
            }
        }

        // Remove the oldest mark if found
        if (oldest_pos.first != -1) {
            char removed_symbol = board[oldest_pos.first][oldest_pos.second];
            cout << "=== REMOVING oldest mark " << removed_symbol << " at ("
                << oldest_pos.first << "," << oldest_pos.second
                << ") placed at move " << oldest_time << " ===\n";

            board[oldest_pos.first][oldest_pos.second] = 0;
            move_timestamps[oldest_pos.first][oldest_pos.second] = -1;
            moves_since_last_removal = 0;
        }
        else {
            cout << "No marks to remove (board empty)\n";
        }
    }
};

#endif