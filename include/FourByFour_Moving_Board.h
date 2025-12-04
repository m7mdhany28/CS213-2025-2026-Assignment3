#pragma once
#ifndef FOURBYFOUR_MOVING_BOARD_H
#define FOURBYFOUR_MOVING_BOARD_H

#include "BoardGame_Classes.h"
#include <vector>
#include <utility>
#include <iostream>
#include <algorithm>

using namespace std;

class FourByFour_Moving_Board : public Board<char> {
private:
    vector<pair<int, int>> x_positions;
    vector<pair<int, int>> o_positions;
    bool game_ended;
    int max_moves;
    int move_count;

public:
    FourByFour_Moving_Board() : Board(4, 4) {
        n_moves = 0;
        move_count = 0;
        game_ended = false;
        max_moves = 50;  // Prevent infinite games

        // Initialize empty board
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                board[i][j] = 0;
            }
        }

        setup_starting_positions();
    }

    bool update_board(Move<char>* move) override {
        if (game_ended) return false;

        int target_x = move->get_x();
        int target_y = move->get_y();
        char symbol = move->get_symbol();

        if (target_x < 0 || target_x >= 4 || target_y < 0 || target_y >= 4) {
            return false;
        }

        if (board[target_x][target_y] != 0) {
            return false;  // Target is occupied
        }

        // Find which token to move
        vector<pair<int, int>>& positions = (symbol == 'X') ? x_positions : o_positions;
        pair<int, int> token_to_move = { -1, -1 };

        for (const auto& pos : positions) {
            // Check if token is adjacent (up, down, left, right)
            int dx = abs(pos.first - target_x);
            int dy = abs(pos.second - target_y);

            if ((dx == 1 && dy == 0) || (dx == 0 && dy == 1)) {
                token_to_move = pos;
                break;
            }
        }

        if (token_to_move.first == -1) {
            return false;  // No adjacent token found
        }

        // Move the token
        board[token_to_move.first][token_to_move.second] = 0;
        board[target_x][target_y] = symbol;

        // Update position in vector
        auto it = find(positions.begin(), positions.end(), token_to_move);
        if (it != positions.end()) {
            *it = { target_x, target_y };
        }

        n_moves++;
        move_count++;

        // Check for win after move
        if (check_win(symbol)) {
            game_ended = true;
        }
        // Check for draw
        else if (move_count >= max_moves || !has_valid_moves('X') || !has_valid_moves('O')) {
            game_ended = true;
        }

        return true;
    }

    bool is_win(Player<char>* player) override {
        if (!player) return false;
        return check_win(player->get_symbol());
    }

    bool check_win(char symbol) {
        // Check horizontal wins (3 in a row)
        for (int row = 0; row < 4; row++) {
            for (int col = 0; col < 2; col++) {  // Only need to check starting at columns 0 and 1
                if (board[row][col] == symbol &&
                    board[row][col + 1] == symbol &&
                    board[row][col + 2] == symbol) {
                    return true;
                }
            }
        }

        // Check vertical wins
        for (int col = 0; col < 4; col++) {
            for (int row = 0; row < 2; row++) {  // Only need to check starting at rows 0 and 1
                if (board[row][col] == symbol &&
                    board[row + 1][col] == symbol &&
                    board[row + 2][col] == symbol) {
                    return true;
                }
            }
        }

        // Check diagonal wins (top-left to bottom-right)
        for (int row = 0; row < 2; row++) {
            for (int col = 0; col < 2; col++) {
                if (board[row][col] == symbol &&
                    board[row + 1][col + 1] == symbol &&
                    board[row + 2][col + 2] == symbol) {
                    return true;
                }
            }
        }

        // Check diagonal wins (top-right to bottom-left)
        for (int row = 0; row < 2; row++) {
            for (int col = 2; col < 4; col++) {
                if (board[row][col] == symbol &&
                    board[row + 1][col - 1] == symbol &&
                    board[row + 2][col - 2] == symbol) {
                    return true;
                }
            }
        }

        return false;
    }

    bool is_lose(Player<char>* player) override {
        return false;
    }

    bool is_draw(Player<char>* player) override {
        return game_ended && !is_win(player);
    }

    bool game_is_over(Player<char>* player) override {
        return game_ended;
    }

    void display_board() {
        cout << "\n    0   1   2   3\n";
        cout << "  +---------------+\n";
        for (int i = 0; i < 4; i++) {
            cout << i << " |";
            for (int j = 0; j < 4; j++) {
                char display_char = (board[i][j] == 0) ? ' ' : board[i][j];
                cout << " " << display_char << " ";
                if (j < 3) cout << "|";
            }
            cout << "|\n";
            if (i < 3) cout << "  |---+---+---+---|\n";
        }
        cout << "  +---------------+\n";

        cout << "Moves: " << move_count << " | ";
        cout << "X tokens: " << x_positions.size() << " | ";
        cout << "O tokens: " << o_positions.size() << "\n";

        if (max_moves - move_count < 10) {
            cout << "Moves remaining: " << (max_moves - move_count) << "\n";
        }
    }

    vector<pair<int, int>> get_player_positions(char symbol) {
        return (symbol == 'X') ? x_positions : o_positions;
    }

    bool would_create_three_in_row(char symbol, int target_x, int target_y) {
        if (board[target_x][target_y] != 0) return false;

        // Temporarily place piece
        char original = board[target_x][target_y];
        board[target_x][target_y] = symbol;

        bool would_win = check_win(symbol);

        // Restore original
        board[target_x][target_y] = original;

        return would_win;
    }

    vector<pair<int, int>> generate_valid_moves_for_player(char player_symbol) {
        vector<pair<int, int>> valid_moves;
        vector<pair<int, int>>& positions = (player_symbol == 'X') ? x_positions : o_positions;

        for (const auto& token_pos : positions) {
            int dx[] = { -1, 1, 0, 0 };
            int dy[] = { 0, 0, -1, 1 };

            for (int d = 0; d < 4; d++) {
                int new_x = token_pos.first + dx[d];
                int new_y = token_pos.second + dy[d];

                if (new_x >= 0 && new_x < 4 && new_y >= 0 && new_y < 4 &&
                    board[new_x][new_y] == 0) {
                    valid_moves.push_back({ new_x, new_y });
                }
            }
        }

        // Remove duplicates
        sort(valid_moves.begin(), valid_moves.end());
        valid_moves.erase(unique(valid_moves.begin(), valid_moves.end()), valid_moves.end());

        return valid_moves;
    }

private:
    void setup_starting_positions() {
        // X starts in top-left quadrant
        x_positions = { {0,0}, {0,1}, {1,0}, {1,1} };
        // O starts in bottom-right quadrant
        o_positions = { {2,2}, {2,3}, {3,2}, {3,3} };

        // Place tokens on board
        for (auto pos : x_positions) board[pos.first][pos.second] = 'X';
        for (auto pos : o_positions) board[pos.first][pos.second] = 'O';

        cout << "Starting positions set - 4 tokens per player\n";
        cout << "X starts in top-left quadrant\n";
        cout << "O starts in bottom-right quadrant\n";
    }

    bool has_valid_moves(char symbol) {
        return !generate_valid_moves_for_player(symbol).empty();
    }
};

#endif