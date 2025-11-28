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
    vector<pair<int, int>> initial_x_positions; // Track starting positions
    vector<pair<int, int>> initial_o_positions;
    vector<vector<int>> move_history;
    int oscillation_check_depth;
    bool game_ended;
    bool first_move_made; // Track if any moves have been made

public:
    FourByFour_Moving_Board() : Board(4, 4) {
        n_moves = 0;
        oscillation_check_depth = 4;
        game_ended = false;
        first_move_made = false;
        move_history.resize(2);

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                board[i][j] = 0;
            }
        }

        setup_balanced_starting_positions();
    }

    bool update_board(Move<char>* move) {
        if (game_ended) return false;

        int x = move->get_x();
        int y = move->get_y();
        char symbol = move->get_symbol();

        bool result = handle_token_move(x, y, symbol);
        if (result) {
            first_move_made = true; // Mark that at least one move has been made
            int player_index = (symbol == 'X') ? 0 : 1;
            int move_hash = x * 10 + y;
            move_history[player_index].push_back(move_hash);

            if (move_history[player_index].size() > oscillation_check_depth) {
                move_history[player_index].erase(move_history[player_index].begin());
            }

            if (is_win(nullptr)) {
                game_ended = true;
            }
        }
        return result;
    }

    bool is_win(Player<char>* player) {
        return check_player_win('X') || check_player_win('O');
    }

    bool check_player_win(char symbol) {
        if (!first_move_made) return false;

        // Horizontal
        for (int row = 0; row < 4; row++) {
            for (int start_col = 0; start_col < 2; start_col++) {
                if (board[row][start_col] == symbol &&
                    board[row][start_col + 1] == symbol &&
                    board[row][start_col + 2] == symbol) {
                    if (!is_starting_position_win(row, start_col, symbol)) return true;
                }
            }
        }

        // Vertical
        for (int col = 0; col < 4; col++) {
            for (int start_row = 0; start_row < 2; start_row++) {
                if (board[start_row][col] == symbol &&
                    board[start_row + 1][col] == symbol &&
                    board[start_row + 2][col] == symbol) {
                    if (!is_starting_position_win_vertical(col, start_row, symbol)) return true;
                }
            }
        }

        // Diagonal top-left to bottom-right
        for (int start_row = 0; start_row < 2; start_row++) {
            for (int start_col = 0; start_col < 2; start_col++) {
                if (board[start_row][start_col] == symbol &&
                    board[start_row + 1][start_col + 1] == symbol &&
                    board[start_row + 2][start_col + 2] == symbol) {
                    if (!is_starting_position_win_diagonal(start_row, start_col, symbol, true)) return true;
                }
            }
        }

        // Diagonal top-right to bottom-left
        for (int start_row = 0; start_row < 2; start_row++) {
            for (int start_col = 2; start_col < 4; start_col++) {
                if (board[start_row][start_col] == symbol &&
                    board[start_row + 1][start_col - 1] == symbol &&
                    board[start_row + 2][start_col - 2] == symbol) {
                    if (!is_starting_position_win_diagonal(start_row, start_col, symbol, false)) return true;
                }
            }
        }

        return false;
    }

    bool is_lose(Player<char>* player) { return false; }

    bool is_draw(Player<char>* player) {
        if (game_ended) return false;

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (board[i][j] == 0 && can_any_token_move_to(i, j)) {
                    if (could_create_winning_position(i, j)) return false;
                }
            }
        }
        return true;
    }

    bool game_is_over(Player<char>* player) {
        return game_ended || is_draw(player);
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

        cout << "Moves: " << n_moves << " | ";
        cout << "X tokens: " << x_positions.size() << " | ";
        cout << "O tokens: " << o_positions.size() << "\n";

        if (is_draw(nullptr)) {
            cout << "Game may end in draw soon - limited winning moves\n";
        }
    }

    vector<pair<int, int>> get_player_positions(char symbol) { return (symbol == 'X') ? x_positions : o_positions; }

    bool is_initial_placement_done() const { return true; }

    bool would_create_three_in_row(char symbol, int target_x, int target_y) {
        return check_win_on_board_with_move(symbol, target_x, target_y);
    }

    bool is_oscillating_move(char symbol, int target_x, int target_y) {
        int player_index = (symbol == 'X') ? 0 : 1;
        int move_hash = target_x * 10 + target_y;
        for (int recent_move : move_history[player_index]) {
            if (recent_move == move_hash) return true;
        }
        return false;
    }

    bool is_strategic_position(int x, int y) const {
        if ((x == 1 || x == 2) && (y == 1 || y == 2)) return true;
        if ((x == 0 || x == 3) && (y == 1 || y == 2)) return true;
        if ((y == 0 || y == 3) && (x == 1 || x == 2)) return true;
        return false;
    }

    bool can_any_token_move_to(int target_x, int target_y) {
        for (auto& pos : x_positions)
            if (is_adjacent(pos.first, pos.second, target_x, target_y)) return true;
        for (auto& pos : o_positions)
            if (is_adjacent(pos.first, pos.second, target_x, target_y)) return true;
        return false;
    }

private:
    void setup_balanced_starting_positions() {
        initial_x_positions = { {0,0},{0,2},{1,1},{1,3} };
        initial_o_positions = { {2,1},{2,3},{3,0},{3,2} };

        x_positions = initial_x_positions;
        o_positions = initial_o_positions;

        for (auto pos : x_positions) board[pos.first][pos.second] = 'X';
        for (auto pos : o_positions) board[pos.first][pos.second] = 'O';

        cout << "Starting positions set - 4 tokens per player (balanced)\n";
    }

    bool handle_token_move(int x, int y, char symbol) {
        if (x < 0 || x >= 4 || y < 0 || y >= 4) return false;
        if (board[x][y] != 0) return false;

        vector<pair<int, int>>& positions = (symbol == 'X') ? x_positions : o_positions;
        pair<int, int> token_to_move = find_best_token_to_move(positions, x, y, symbol);
        if (token_to_move.first == -1) return false;

        board[token_to_move.first][token_to_move.second] = 0;
        board[x][y] = symbol;

        auto it = find(positions.begin(), positions.end(), token_to_move);
        if (it != positions.end()) *it = { x,y };

        n_moves++;
        return true;
    }

    pair<int, int> find_best_token_to_move(const vector<pair<int, int>>& positions, int target_x, int target_y, char symbol) {
        vector<pair<int, int>> adjacent_tokens, strategic_tokens;
        for (const auto& pos : positions) {
            if (is_adjacent(pos.first, pos.second, target_x, target_y)) {
                adjacent_tokens.push_back(pos);
                if (!is_oscillating_move(symbol, pos.first, pos.second)) strategic_tokens.push_back(pos);
            }
        }

        if (adjacent_tokens.empty()) return { -1,-1 };
        if (!strategic_tokens.empty()) return strategic_tokens[0];

        for (auto& token : adjacent_tokens) if (!is_strategic_position(token.first, token.second)) return token;
        return adjacent_tokens[0];
    }

    bool is_adjacent(int from_x, int from_y, int to_x, int to_y) { return (abs(from_x - to_x) == 1 && from_y == to_y) || (from_x == to_x && abs(from_y - to_y) == 1); }

    bool check_win_on_board_with_move(char symbol, int x, int y) const {
        vector<vector<char>> test_board(4, vector<char>(4));
        for (int i = 0; i < 4; i++) for (int j = 0; j < 4; j++) test_board[i][j] = board[i][j];
        test_board[x][y] = symbol;
        return check_player_win_on_board(test_board, symbol);
    }

    bool check_player_win_on_board(const vector<vector<char>>& test_board, char symbol) const {
        for (int row = 0; row < 4; row++) for (int start_col = 0; start_col < 2; start_col++)
            if (test_board[row][start_col] == symbol && test_board[row][start_col + 1] == symbol && test_board[row][start_col + 2] == symbol) return true;

        for (int col = 0; col < 4; col++) for (int start_row = 0; start_row < 2; start_row++)
            if (test_board[start_row][col] == symbol && test_board[start_row + 1][col] == symbol && test_board[start_row + 2][col] == symbol) return true;

        for (int start_row = 0; start_row < 2; start_row++) for (int start_col = 0; start_col < 2; start_col++)
            if (test_board[start_row][start_col] == symbol && test_board[start_row + 1][start_col + 1] == symbol && test_board[start_row + 2][start_col + 2] == symbol) return true;

        for (int start_row = 0; start_row < 2; start_row++) for (int start_col = 2; start_col < 4; start_col++)
            if (test_board[start_row][start_col] == symbol && test_board[start_row + 1][start_col - 1] == symbol && test_board[start_row + 2][start_col - 2] == symbol) return true;

        return false;
    }

    bool could_create_winning_position(int x, int y) {
        return check_win_on_board_with_move('X', x, y) || check_win_on_board_with_move('O', x, y);
    }

    // Check if a three-in-row uses only initial positions
    bool is_starting_position_win(int row, int start_col, char symbol) {
        int initial_count = 0;
        for (int i = 0; i < 3; i++) {
            pair<int, int> pos = { row,start_col + i };
            if ((symbol == 'X' && find(initial_x_positions.begin(), initial_x_positions.end(), pos) != initial_x_positions.end()) ||
                (symbol == 'O' && find(initial_o_positions.begin(), initial_o_positions.end(), pos) != initial_o_positions.end())) initial_count++;
        }
        return initial_count == 3;
    }

    bool is_starting_position_win_vertical(int col, int start_row, char symbol) {
        int initial_count = 0;
        for (int i = 0; i < 3; i++) {
            pair<int, int> pos = { start_row + i,col };
            if ((symbol == 'X' && find(initial_x_positions.begin(), initial_x_positions.end(), pos) != initial_x_positions.end()) ||
                (symbol == 'O' && find(initial_o_positions.begin(), initial_o_positions.end(), pos) != initial_o_positions.end())) initial_count++;
        }
        return initial_count == 3;
    }

    bool is_starting_position_win_diagonal(int start_row, int start_col, char symbol, bool is_main) {
        int initial_count = 0;
        for (int i = 0; i < 3; i++) {
            pair<int, int> pos = is_main ? pair<int, int>{start_row + i, start_col + i} : pair<int, int>{ start_row + i,start_col - i };
            if ((symbol == 'X' && find(initial_x_positions.begin(), initial_x_positions.end(), pos) != initial_x_positions.end()) ||
                (symbol == 'O' && find(initial_o_positions.begin(), initial_o_positions.end(), pos) != initial_o_positions.end())) initial_count++;
        }
        return initial_count == 3;
    }
};

#endif