#pragma once
#ifndef SIMPLE_UI_H
#define SIMPLE_UI_H

#include "BoardGame_Classes.h"
#include <limits>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>

class Simple_UI : public UI<char> {
private:
    Board<char>* game_board;

public:
    Simple_UI(Board<char>* board) : UI("Game session starting now! Remember the reverse rules: three in a row means you lose!", 3), game_board(board) {}

    Move<char>* get_move(Player<char>* player) override {
        // Human player
        if (player->get_type() == PlayerType::HUMAN) {
            return get_human_move(player);
        }
        // Computer player
        else {
            return get_computer_move(player);
        }
    }

    Player<char>* create_player(string& name, char symbol, PlayerType type) override {
        return new Player<char>(name, symbol, type);
    }

private:
    Move<char>* get_human_move(Player<char>* player) {
        int x, y;
        cout << "\n" << player->get_name() << ", it is your turn to play.\n";
        cout << "You are playing with symbol: " << player->get_symbol() << "\n";
        cout << "Please enter your move as two numbers (row then column): ";

        while (!(cin >> x >> y)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter two numbers separated by space: ";
        }

        cout << "You have selected position: Row " << x << ", Column " << y << "\n";
        return new Move<char>(x, y, player->get_symbol());
    }

    Move<char>* get_computer_move(Player<char>* player) {
        cout << "\n" << player->get_name() << " (Computer) is now planning its move...\n";

        // Brief pause to simulate thinking
        this_thread::sleep_for(chrono::milliseconds(1200));

        if (player->get_type() == PlayerType::RANDOM) {
            return get_random_move(player);
        }
        else {
            return get_smart_move(player);
        }
    }

    Move<char>* get_random_move(Player<char>* player) {
        cout << "Computer is selecting a random valid position...\n";

        // Get board dimensions
        int rows = game_board->get_rows();
        int cols = game_board->get_columns();

        // Find all empty cells
        vector<vector<char>> board = game_board->get_board_matrix();
        vector<pair<int, int>> empty_cells;

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (board[i][j] == 0) {
                    empty_cells.push_back({ i, j });
                }
            }
        }

        if (empty_cells.empty()) {
            cout << "No moves available - this should not happen in normal play.\n";
            return new Move<char>(0, 0, player->get_symbol());
        }

        // Select a random empty cell
        int random_index = rand() % empty_cells.size();
        int x = empty_cells[random_index].first;
        int y = empty_cells[random_index].second;

        cout << "Computer has chosen position: Row " << x << ", Column " << y << "\n";
        return new Move<char>(x, y, player->get_symbol());
    }

    Move<char>* get_smart_move(Player<char>* player) {
        cout << "Computer is analyzing the board for strategic opportunities...\n";
        // For initial implementation, use random moves
        // Future enhancement: Implement minimax algorithm or strategic evaluation
        return get_random_move(player);
    }
};

#endif