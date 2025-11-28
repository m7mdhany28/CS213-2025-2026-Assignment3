#pragma once
#ifndef MISERE_BOARD_H
#define MISERE_BOARD_H

#include "BoardGame_Classes.h"

// The reverse psychology challenge - where winning means losing!
class Misere_Board : public Board<char> {
public:
    Misere_Board() : Board(3, 3) { n_moves = 0; }

    // Place a mark on the board
    bool update_board(Move<char>* move) override {
        int row = move->get_x();
        int col = move->get_y();
        char symbol = move->get_symbol();

        // Make sure the spot is available and valid
        if (row >= 0 && row < 3 && col >= 0 && col < 3 && board[row][col] == 0) {
            board[row][col] = symbol;
            n_moves++;
            return true;
        }
        return false;
    }

    // Check if someone has three in a row (which means they LOSE!)
    bool is_win(Player<char>* player) override {
        char symbol = player->get_symbol();

        // Check all rows for three in a row
        for (int row = 0; row < 3; row++) {
            if (board[row][0] == symbol && board[row][1] == symbol && board[row][2] == symbol) return true;
        }

        // Check all columns for three in a row
        for (int col = 0; col < 3; col++) {
            if (board[0][col] == symbol && board[1][col] == symbol && board[2][col] == symbol) return true;
        }

        // Check both diagonals
        if (board[0][0] == symbol && board[1][1] == symbol && board[2][2] == symbol) return true;
        if (board[0][2] == symbol && board[1][1] == symbol && board[2][0] == symbol) return true;

        return false;  // No three in a row found
    }

    // In misere, getting three in a row means you lose
    bool is_lose(Player<char>* player) override {
        return is_win(player);  // Winning the normal way means losing here!
    }

    // Game is a draw if board fills up with no three-in-a-rows
    bool is_draw(Player<char>* player) override {
        return (n_moves == 9) && !is_win(player);
    }

    // Game ends when someone gets three in a row or board is full
    bool game_is_over(Player<char>* player) override {
        return is_win(player) || is_draw(player);
    }
};

#endif