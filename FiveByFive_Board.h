#pragma once
#ifndef FIVEBYFIVE_BOARD_H
#define FIVEBYFIVE_BOARD_H

#include "BoardGame_Classes.h"
#include <vector>
#include <utility>
#include <algorithm>

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

        if (x >= 0 && x < 5 && y >= 0 && y < 5 && board[x][y] == 0) {
            board[x][y] = symbol;
            n_moves++;
            return true;
        }
        return false;
    }

    bool is_win(Player<char>* player) override {
        
        return false;
    }

    bool is_lose(Player<char>* player) override {
        return false; 
    }

    bool is_draw(Player<char>* player) override {
   
        return n_moves == 24;
    }

    bool game_is_over(Player<char>* player) override {
        return is_draw(player);
    }

    int count_player_lines(char symbol) {
        int count = 0;
        vector<vector<bool>> counted(5, vector<bool>(5, false));

        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == symbol &&
                    board[i][j + 1] == symbol &&
                    board[i][j + 2] == symbol) {
                    count++;
                }
            }
        }

        for (int j = 0; j < 5; j++) {
            for (int i = 0; i < 3; i++) {
                if (board[i][j] == symbol &&
                    board[i + 1][j] == symbol &&
                    board[i + 2][j] == symbol) {
                    count++;
                }
            }
        }

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == symbol &&
                    board[i + 1][j + 1] == symbol &&
                    board[i + 2][j + 2] == symbol) {
                    count++;
                }
            }
        }

        for (int i = 0; i < 3; i++) {
            for (int j = 2; j < 5; j++) {
                if (board[i][j] == symbol &&
                    board[i + 1][j - 1] == symbol &&
                    board[i + 2][j - 2] == symbol) {
                    count++;
                }
            }
        }

        return count;
    }
};

#endif