#pragma once
#ifndef INFINITY_BOARD_H
#define INFINITY_BOARD_H

#include "BoardGame_Classes.h"
#include <queue>
#include <utility>
#include <iostream>

using namespace std;

class Infinity_Board : public Board<char> {
private:
    queue<pair<int, int>> move_history;
    int moves_since_removal;

public:
    Infinity_Board() : Board(3, 3) {
        n_moves = 0;
        moves_since_removal = 0;

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
            move_history.push(make_pair(x, y));
            n_moves++;
            moves_since_removal++;

            if (moves_since_removal >= 3 && n_moves > 3) {
                remove_oldest_move();
                moves_since_removal = 0;
            }
            return true;
        }
        return false;
    }

    bool is_win(Player<char>* player) override {
        char symbol = player->get_symbol();

        for (int i = 0; i < 3; i++) {
            if (board[i][0] == symbol && board[i][1] == symbol && board[i][2] == symbol)
                return true;
        }

        for (int j = 0; j < 3; j++) {
            if (board[0][j] == symbol && board[1][j] == symbol && board[2][j] == symbol)
                return true;
        }

        if (board[0][0] == symbol && board[1][1] == symbol && board[2][2] == symbol)
            return true;
        if (board[0][2] == symbol && board[1][1] == symbol && board[2][0] == symbol)
            return true;

        return false;
    }

    bool is_lose(Player<char>* player) override {
        return false;
    }

    bool is_draw(Player<char>* player) override {
        return n_moves >= 50;
    }

    bool game_is_over(Player<char>* player) override {
        return is_win(player) || is_draw(player);
    }

    int get_moves_since_removal() const {
        return moves_since_removal;
    }

    int get_next_removal_countdown() const {
        return 3 - moves_since_removal;
    }

private:
    void remove_oldest_move() {
        if (!move_history.empty()) {
            auto oldest = move_history.front();
            move_history.pop();

            int x = oldest.first;
            int y = oldest.second;

            cout << "Infinity Effect: The mark at position (" << x << ", " << y << ") has disappeared!\n";
            board[x][y] = 0;
        }
    }
};

#endif