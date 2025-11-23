#pragma once
#ifndef SIMPLE_UI_H
#define SIMPLE_UI_H

#include "BoardGame_Classes.h"
#include "Misere_Board.h"
#include "FiveByFive_Board.h"
#include "WordTicTacToe_Board.h"
#include "Infinity_Board.h"
#include <limits>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
#include <algorithm>

class Simple_UI : public UI<char> {
private:
    Board<char>* game_board;
    string game_type;

public:
    Simple_UI(Board<char>* board, string game_name) : UI("Game session starting now!", 3), game_board(board), game_type(game_name) {
        if (game_type == "word") {
            cout << "Word Tic Tac Toe - Form valid 3-letter words to win!\n";
        }
        else if (game_type == "5x5") {
            cout << "5x5 Tic Tac Toe - Create the most 3-in-a-row sequences!\n";
        }
        else if (game_type == "infinity") {
            cout << "Infinity Tic-Tac-Toe - Marks disappear every 3 moves!\n";
        }
        else {
            cout << "Misere Tic Tac Toe - Remember: 3 in a row means you lose!\n";
        }
    }

    Move<char>* get_move(Player<char>* player) override {
        if (player->get_type() == PlayerType::HUMAN) {
            return get_human_move(player);
        }
        else if (player->get_type() == PlayerType::RANDOM) {
            return get_random_computer_move(player);
        }
        else {
            return get_smart_ai_move(player);
        }
    }

    Player<char>* create_player(string& name, char symbol, PlayerType type) override {
        return new Player<char>(name, symbol, type);
    }

    void display_5x5_results(FiveByFive_Board* board, Player<char>* player1, Player<char>* player2) {
        int player1_lines = board->count_player_lines('X');
        int player2_lines = board->count_player_lines('O');

        cout << "\n" << string(50, '=') << "\n";
        cout << "GAME OVER - FINAL SCORE\n";
        cout << string(50, '=') << "\n";
        cout << player1->get_name() << " (X): " << player1_lines << " three-in-a-row sequences\n";
        cout << player2->get_name() << " (O): " << player2_lines << " three-in-a-row sequences\n";

        if (player1_lines > player2_lines) {
            cout << "WINNER: " << player1->get_name() << "!\n";
        }
        else if (player2_lines > player1_lines) {
            cout << "WINNER: " << player2->get_name() << "!\n";
        }
        else {
            cout << "The game is a TIE!\n";
        }
        cout << string(50, '=') << "\n";
    }

    void display_word_game_results(WordTicTacToe_Board* board, Player<char>* winner) {
        string winning_word = board->get_winning_word();

        cout << "\n" << string(50, '=') << "\n";
        cout << "GAME OVER - WORD FOUND!\n";
        cout << string(50, '=') << "\n";
        cout << "Winning word: " << winning_word << "\n";
        cout << "WINNER: " << winner->get_name() << "!\n";
        cout << "Congratulations for forming a valid word!\n";
        cout << string(50, '=') << "\n";
    }

private:
    Move<char>* get_human_move(Player<char>* player) {
        int x, y;
        char letter;

        if (game_type == "word") {
            cout << "\n" << player->get_name() << ", it's your turn!\n";
            cout << "Enter a letter (A-Z) to place: ";
            while (!(cin >> letter) || !isalpha(letter)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Please enter a valid letter (A-Z): ";
            }
            letter = toupper(letter);
            cout << "Now enter the position (row and column 0-2) to place '" << letter << "': ";
            while (!(cin >> x >> y) || x < 0 || x > 2 || y < 0 || y > 2) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Please enter two valid numbers (0-2) for row and column: ";
            }
            cout << "You placed '" << letter << "' at position: Row " << x << ", Column " << y << "\n";
        }
        else {
            cout << "\n" << player->get_name() << ", it is your turn to play.\n";
            cout << "You are playing with symbol: " << player->get_symbol() << "\n";
            int max_coord = (game_type == "5x5") ? 4 : 2;
            cout << "Please enter your move as two numbers (0-" << max_coord << " for row and column): ";
            while (!(cin >> x >> y)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please enter two numbers separated by space: ";
            }
            cout << "You have selected position: Row " << x << ", Column " << y << "\n";
            letter = player->get_symbol();
        }

        return new Move<char>(x, y, letter);
    }

    Move<char>* get_random_computer_move(Player<char>* player) {
        cout << "\n" << player->get_name() << " (Random Computer) is making a move...\n";
        this_thread::sleep_for(chrono::milliseconds(800));

        if (game_type == "word") {
            char random_letter = 'A' + (rand() % 26);
            cout << "Random computer chooses letter: " << random_letter << "\n";
            return get_random_letter_move(player, random_letter);
        }
        else {
            return get_random_move(player);
        }
    }

    Move<char>* get_smart_ai_move(Player<char>* player) {
        cout << "\n" << player->get_name() << " (Smart AI) is analyzing the board...\n";
        this_thread::sleep_for(chrono::milliseconds(1200));

        if (game_type == "word") {
            return get_smart_word_move(player);
        }
        else if (game_type == "5x5") {
            return get_smart_5x5_move(player);
        }
        else if (game_type == "infinity") {
            return get_smart_infinity_move(player);
        }
        else {
            return get_smart_misere_move(player);
        }
    }

    Move<char>* get_smart_5x5_move(Player<char>* player) {
        FiveByFive_Board* board = dynamic_cast<FiveByFive_Board*>(game_board);
        if (!board) return get_random_move(player);

        vector<vector<char>> board_matrix = board->get_board_matrix();
        char symbol = player->get_symbol();
        char opponent_symbol = (symbol == 'X') ? 'O' : 'X';

        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                if (board_matrix[i][j] == 0) {
                    board_matrix[i][j] = symbol;
                    if (count_sequences_at(board_matrix, symbol, i, j) > 0) {
                        cout << "Smart AI completes a sequence at: " << i << " " << j << "\n";
                        return new Move<char>(i, j, symbol);
                    }
                    board_matrix[i][j] = 0;
                }
            }
        }

        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                if (board_matrix[i][j] == 0) {
                    board_matrix[i][j] = opponent_symbol;
                    if (count_sequences_at(board_matrix, opponent_symbol, i, j) > 0) {
                        cout << "Smart AI blocks opponent at: " << i << " " << j << "\n";
                        return new Move<char>(i, j, symbol);
                    }
                    board_matrix[i][j] = 0;
                }
            }
        }

        vector<pair<int, int>> priority_positions = { {2,2}, {1,1}, {1,3}, {3,1}, {3,3}, {0,0}, {0,4}, {4,0}, {4,4} };
        for (auto pos : priority_positions) {
            if (board_matrix[pos.first][pos.second] == 0) {
                cout << "Smart AI takes strategic position: " << pos.first << " " << pos.second << "\n";
                return new Move<char>(pos.first, pos.second, symbol);
            }
        }

        cout << "Smart AI makes a strategic random move.\n";
        return get_random_move(player);
    }

    Move<char>* get_smart_word_move(Player<char>* player) {
        WordTicTacToe_Board* board = dynamic_cast<WordTicTacToe_Board*>(game_board);
        if (!board) return get_random_move(player);

        vector<vector<char>> board_matrix = board->get_board_matrix();
        char random_letter = 'A' + (rand() % 26);
        vector<string> sequences = board->get_all_sequences();
        for (int s = 0; s < sequences.size(); s++) {
            string seq = sequences[s];
            int empty_pos = -1;
            int empty_count = 0;

            for (int i = 0; i < 3; i++) {
                if (seq[i] == ' ') {
                    empty_pos = i;
                    empty_count++;
                }
            }

            if (empty_count == 1) {
                for (char c = 'A'; c <= 'Z'; c++) {
                    string test_word = seq;
                    test_word[empty_pos] = c;
                    if (board->is_word_valid(test_word)) {
                        pair<int, int> pos = get_position_from_sequence(s, empty_pos, board_matrix);
                        if (pos.first != -1 && board_matrix[pos.first][pos.second] == 0) {
                            cout << "Smart AI completes word '" << test_word << "' with letter: " << c << " at " << pos.first << " " << pos.second << "\n";
                            return new Move<char>(pos.first, pos.second, c);
                        }
                    }
                }
            }
        }

        vector<char> common_letters = { 'A', 'E', 'I', 'O', 'U', 'T', 'S', 'R', 'N' };
        vector<pair<int, int>> word_hotspots = { {1,1}, {0,1}, {1,0}, {1,2}, {2,1} };

        for (auto pos : word_hotspots) {
            if (board_matrix[pos.first][pos.second] == 0) {
                char best_letter = common_letters[rand() % common_letters.size()];
                cout << "Smart AI places '" << best_letter << "' at word hotspot: " << pos.first << " " << pos.second << "\n";
                return new Move<char>(pos.first, pos.second, best_letter);
            }
        }

        cout << "Smart AI places '" << random_letter << "' at strategic position.\n";
        return get_random_letter_move(player, random_letter);
    }

    Move<char>* get_smart_misere_move(Player<char>* player) {
        vector<vector<char>> board_matrix = game_board->get_board_matrix();
        char symbol = player->get_symbol();
        char opponent_symbol = (symbol == 'X') ? 'O' : 'X';

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board_matrix[i][j] == 0) {
                    board_matrix[i][j] = symbol;
                    if (!would_create_three_in_row(board_matrix, symbol, i, j)) {
                        cout << "Smart AI makes safe move: " << i << " " << j << "\n";
                        return new Move<char>(i, j, symbol);
                    }
                    board_matrix[i][j] = 0;
                }
            }
        }

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board_matrix[i][j] == 0) {
                    board_matrix[i][j] = opponent_symbol;
                    if (would_create_three_in_row(board_matrix, opponent_symbol, i, j)) {
                        cout << "Smart AI forces opponent into trap: " << i << " " << j << "\n";
                        return new Move<char>(i, j, symbol);
                    }
                    board_matrix[i][j] = 0;
                }
            }
        }

        cout << "Smart AI makes defensive move.\n";
        return get_random_move(player);
    }

    Move<char>* get_smart_infinity_move(Player<char>* player) {
        Infinity_Board* board = dynamic_cast<Infinity_Board*>(game_board);
        if (!board) return get_random_move(player);

        vector<vector<char>> board_matrix = board->get_board_matrix();
        char symbol = player->get_symbol();
        char opponent_symbol = (symbol == 'X') ? 'O' : 'X';
        int removal_countdown = board->get_next_removal_countdown();

        if (removal_countdown == 1) {
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    if (board_matrix[i][j] == 0) {
                        board_matrix[i][j] = symbol;
                        if (would_create_three_in_row(board_matrix, symbol, i, j)) {
                            cout << "Smart AI rushes to complete sequence before removal: " << i << " " << j << "\n";
                            return new Move<char>(i, j, symbol);
                        }
                        board_matrix[i][j] = 0;
                    }
                }
            }
        }

        vector<pair<int, int>> center_first = { {1,1}, {0,0}, {0,2}, {2,0}, {2,2}, {0,1}, {1,0}, {1,2}, {2,1} };

        for (auto pos : center_first) {
            if (board_matrix[pos.first][pos.second] == 0) {
                cout << "Smart AI takes strategic position: " << pos.first << " " << pos.second << "\n";
                return new Move<char>(pos.first, pos.second, symbol);
            }
        }

        cout << "Smart AI makes a calculated move.\n";
        return get_random_move(player);
    }

    Move<char>* get_random_move(Player<char>* player) {
        int rows = game_board->get_rows();
        int cols = game_board->get_columns();
        vector<vector<char>> board_matrix = game_board->get_board_matrix();
        vector<pair<int, int>> empty_cells;

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (board_matrix[i][j] == 0) {
                    empty_cells.push_back({ i, j });
                }
            }
        }

        if (empty_cells.empty()) {
            cout << "No moves available.\n";
            return new Move<char>(0, 0, player->get_symbol());
        }

        int random_index = rand() % empty_cells.size();
        int x = empty_cells[random_index].first;
        int y = empty_cells[random_index].second;

        cout << "Chosen position: Row " << x << ", Column " << y << "\n";
        return new Move<char>(x, y, player->get_symbol());
    }

    Move<char>* get_random_letter_move(Player<char>* player, char letter) {
        int rows = game_board->get_rows();
        int cols = game_board->get_columns();
        vector<vector<char>> board_matrix = game_board->get_board_matrix();
        vector<pair<int, int>> empty_cells;

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (board_matrix[i][j] == 0) {
                    empty_cells.push_back({ i, j });
                }
            }
        }

        if (empty_cells.empty()) {
            cout << "No moves available.\n";
            return new Move<char>(0, 0, letter);
        }

        int random_index = rand() % empty_cells.size();
        int x = empty_cells[random_index].first;
        int y = empty_cells[random_index].second;

        cout << "Chosen position: Row " << x << ", Column " << y << "\n";
        return new Move<char>(x, y, letter);
    }

    int count_sequences_at(const vector<vector<char>>& board, char symbol, int x, int y) {
        int count = 0;
        int size = board.size();

        for (int j = max(0, y - 2); j <= min(size - 3, y); j++) {
            if (board[x][j] == symbol && board[x][j + 1] == symbol && board[x][j + 2] == symbol) {
                count++;
            }
        }

        for (int i = max(0, x - 2); i <= min(size - 3, x); i++) {
            if (board[i][y] == symbol && board[i + 1][y] == symbol && board[i + 2][y] == symbol) {
                count++;
            }
        }

        for (int i = max(0, x - 2); i <= min(size - 3, x); i++) {
            for (int j = max(0, y - 2); j <= min(size - 3, y); j++) {
                if (i + 2 < size && j + 2 < size) {
                    if (board[i][j] == symbol && board[i + 1][j + 1] == symbol && board[i + 2][j + 2] == symbol) {
                        count++;
                    }
                }
                if (i + 2 < size && j - 2 >= 0) {
                    if (board[i][j] == symbol && board[i + 1][j - 1] == symbol && board[i + 2][j - 2] == symbol) {
                        count++;
                    }
                }
            }
        }

        return count;
    }

    bool would_create_three_in_row(const vector<vector<char>>& board, char symbol, int x, int y) {
        vector<vector<char>> test_board = board;
        test_board[x][y] = symbol;
        return count_sequences_at(test_board, symbol, x, y) > 0;
    }

    pair<int, int> get_position_from_sequence(int seq_index, int pos_in_seq, const vector<vector<char>>& board) {
        if (seq_index < 3) {
            return { seq_index, pos_in_seq };
        }
        else if (seq_index < 6) {
            return { pos_in_seq, seq_index - 3 };
        }
        else if (seq_index == 6) {
            return { pos_in_seq, pos_in_seq };
        }
        else {
            return { pos_in_seq, 2 - pos_in_seq };
        }
    }
};

#endif