#pragma once
#ifndef MISERE_BOARD_H
#define MISERE_BOARD_H

#include "BoardGame_Classes.h"

class Misere_Board : public Board<char> {
public:
    Misere_Board() : Board(3, 3) {
        n_moves = 0;
        // Initialize board with zeros
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

        if (x >= 0 && x < 3 && y >= 0 && y < 3 && board[x][y] == 0) {
            board[x][y] = symbol;
            n_moves++;
            return true;
        }
        return false;
    }

    bool is_win(Player<char>* player) override {
        char symbol = player->get_symbol();

        // Check rows, columns, diagonals for 3 in a row
        for (int i = 0; i < 3; i++) {
            if (board[i][0] == symbol && board[i][1] == symbol && board[i][2] == symbol) return true;
            if (board[0][i] == symbol && board[1][i] == symbol && board[2][i] == symbol) return true;
        }
        if (board[0][0] == symbol && board[1][1] == symbol && board[2][2] == symbol) return true;
        if (board[0][2] == symbol && board[1][1] == symbol && board[2][0] == symbol) return true;

        return false;
    }

    bool is_lose(Player<char>* player) override {
        return is_win(player);
    }

    bool is_draw(Player<char>* player) override {
        return (n_moves == 9) && !is_win(player);
    }

    bool game_is_over(Player<char>* player) override {
        return is_win(player) || is_draw(player);
    }

    // Helper method to get valid moves (useful for AI)
    vector<pair<int, int>> get_valid_moves() {
        vector<pair<int, int>> moves;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == 0) {
                    moves.push_back({ i, j });
                }
            }
        }
        return moves;
    }
};

#endif