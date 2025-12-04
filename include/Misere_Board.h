#pragma once
#ifndef MISERE_BOARD_H
#define MISERE_BOARD_H

#include "BoardGame_Classes.h"
#include <iostream>

using namespace std;

class Misere_Board : public Board<char> {
private:
    // Helper function to check for three in a row
    bool has_three_in_row(char symbol) const {
        // Check rows
        for (int row = 0; row < 3; row++) {
            if (board[row][0] == symbol && board[row][1] == symbol && board[row][2] == symbol)
                return true;
        }

        // Check columns
        for (int col = 0; col < 3; col++) {
            if (board[0][col] == symbol && board[1][col] == symbol && board[2][col] == symbol)
                return true;
        }

        // Check diagonals
        if (board[0][0] == symbol && board[1][1] == symbol && board[2][2] == symbol)
            return true;
        if (board[0][2] == symbol && board[1][1] == symbol && board[2][0] == symbol)
            return true;

        return false;
    }

public:
    Misere_Board() : Board(3, 3) {
        n_moves = 0;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                board[i][j] = 0;
            }
        }
    }

    bool update_board(Move<char>* move) override {
        int row = move->get_x();
        int col = move->get_y();
        char symbol = move->get_symbol();

        if (row >= 0 && row < 3 && col >= 0 && col < 3 && board[row][col] == 0) {
            board[row][col] = symbol;
            n_moves++;

            // Debug: Show board after move
            cout << "After move " << n_moves << ":\n";
            display_board();

            return true;
        }
        return false;
    }

    // Check if player has three in a row (means they LOSE!)
    bool is_win(Player<char>* player) override {
        char symbol = player->get_symbol();
        bool has_line = has_three_in_row(symbol);

        if (has_line) {
            cout << "DEBUG: Player " << symbol << " has three in a row (they LOSE!)\n";
        }
        return has_line;
    }

    // In misere, getting three in a row means you lose
    bool is_lose(Player<char>* player) override {
        return is_win(player);
    }

    // Game is a draw if board is full AND no one has three in a row
    bool is_draw(Player<char>* player) override {
        if (n_moves == 9) {
            bool x_has_line = has_three_in_row('X');
            bool o_has_line = has_three_in_row('O');

            // Draw only if NO ONE has three in a row
            bool is_draw_result = !x_has_line && !o_has_line;

            if (is_draw_result) {
                cout << "DEBUG: Game is a draw (no three-in-a-rows)\n";
            }
            return is_draw_result;
        }
        return false;
    }

    // Game ends when someone gets three in a row OR board is full
    bool game_is_over(Player<char>* player) override {
        bool x_wins = has_three_in_row('X');
        bool o_wins = has_three_in_row('O');
        bool board_full = (n_moves == 9);

        bool game_over = x_wins || o_wins || board_full;

        if (game_over) {
            cout << "DEBUG: Game over - ";
            if (x_wins) cout << "X has three in a row\n";
            else if (o_wins) cout << "O has three in a row\n";
            else if (board_full) cout << "Board is full\n";
        }

        return game_over;
    }

    // Display board helper
    void display_board() const {
        cout << "\n   0   1   2\n";
        cout << "  -----------\n";
        for (int i = 0; i < 3; i++) {
            cout << i << "|";
            for (int j = 0; j < 3; j++) {
                char display_char = (board[i][j] == 0) ? ' ' : board[i][j];
                cout << " " << display_char << " |";
            }
            cout << "\n  -----------\n";
        }
        cout << "Moves: " << n_moves << "\n";
    }

    // Get board state for AI evaluation
    vector<vector<char>> get_board_state() const {
        return board;
    }
};

#endif