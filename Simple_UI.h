#pragma once
#ifndef SIMPLE_UI_H
#define SIMPLE_UI_H

#include "BoardGame_Classes.h"
#include "Misere_Board.h"
#include "FiveByFive_Board.h"
#include "WordTicTacToe_Board.h"
#include "Infinity_Board.h"
#include "FourByFour_Moving_Board.h"
#include "pyramid_tic_tac_toe.h"
#include <limits>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
#include <algorithm>
#include <map>

class Simple_UI : public UI<char> {
private:
    Board<char>* game_board;
    string game_type;
    PyramidTicTacToe* pyramid_game;

public:
    Simple_UI(Board<char>* board, string game_name) : UI("Game session starting now!", 3), game_board(board), game_type(game_name), pyramid_game(nullptr) {
        if (game_type == "word") {
            cout << "Word Tic Tac Toe - Form valid 3-letter words to win!\n";
        }
        else if (game_type == "5x5") {
            cout << "5x5 Tic Tac Toe - Create the most 3-in-a-row sequences!\n";
        }
        else if (game_type == "infinity") {
            cout << "Infinity Tic-Tac-Toe - Marks disappear every 3 moves!\n";
        }
        else if (game_type == "4x4") {
            cout << "4x4 Moving Tic-Tac-Toe - Move your tokens strategically!\n";
        }
        else {
            cout << "Misere Tic Tac Toe - Remember: 3 in a row means you lose!\n";
        }
    }

    void set_pyramid_game(PyramidTicTacToe* game) {
        pyramid_game = game;
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
        int player1_lines = 0, player2_lines = 0;

        vector<vector<char>> board_matrix = board->get_board_matrix();

        player1_lines = count_sequences(board_matrix, 'X');
        player2_lines = count_sequences(board_matrix, 'O');

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
        string winning_word = "WIN";

        cout << "\n" << string(50, '=') << "\n";
        cout << "GAME OVER - WORD FOUND!\n";
        cout << string(50, '=') << "\n";
        cout << "Winning word: " << winning_word << "\n";
        cout << "WINNER: " << winner->get_name() << "!\n";
        cout << "Congratulations for forming a valid word!\n";
        cout << string(50, '=') << "\n";
    }

private:
    // Count sequences for 5x5 board
    int count_sequences(const vector<vector<char>>& board, char symbol) {
        int count = 0;
        int size = board.size();

        // Check horizontal sequences
        for (int i = 0; i < size; i++) {
            for (int j = 0; j <= size - 3; j++) {
                if (board[i][j] == symbol && board[i][j + 1] == symbol && board[i][j + 2] == symbol) {
                    count++;
                }
            }
        }

        // Check vertical sequences
        for (int j = 0; j < size; j++) {
            for (int i = 0; i <= size - 3; i++) {
                if (board[i][j] == symbol && board[i + 1][j] == symbol && board[i + 2][j] == symbol) {
                    count++;
                }
            }
        }

        // Check diagonal (top-left to bottom-right)
        for (int i = 0; i <= size - 3; i++) {
            for (int j = 0; j <= size - 3; j++) {
                if (board[i][j] == symbol && board[i + 1][j + 1] == symbol && board[i + 2][j + 2] == symbol) {
                    count++;
                }
            }
        }

        // Check diagonal (top-right to bottom-left)
        for (int i = 0; i <= size - 3; i++) {
            for (int j = 2; j < size; j++) {
                if (board[i][j] == symbol && board[i + 1][j - 1] == symbol && board[i + 2][j - 2] == symbol) {
                    count++;
                }
            }
        }

        return count;
    }

    // Check if move would complete a valid three-in-a-row sequence
    bool would_complete_sequence(vector<vector<char>> board, char symbol, int row, int col) {
        if (board[row][col] != 0) return false;

        board[row][col] = symbol;

        // Check horizontal sequences
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 3; j++) {
                bool involves_our_move = false;
                for (int k = 0; k < 3; k++) {
                    if (i == row && (j + k) == col) {
                        involves_our_move = true;
                        break;
                    }
                }

                if (involves_our_move) {
                    if (board[i][j] == symbol &&
                        board[i][j + 1] == symbol &&
                        board[i][j + 2] == symbol) {
                        return true;
                    }
                }
            }
        }

        // Check vertical sequences
        for (int j = 0; j < 5; j++) {
            for (int i = 0; i < 3; i++) {
                bool involves_our_move = false;
                for (int k = 0; k < 3; k++) {
                    if ((i + k) == row && j == col) {
                        involves_our_move = true;
                        break;
                    }
                }

                if (involves_our_move) {
                    if (board[i][j] == symbol &&
                        board[i + 1][j] == symbol &&
                        board[i + 2][j] == symbol) {
                        return true;
                    }
                }
            }
        }

        // Check diagonal sequences
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                bool involves_our_move = false;
                for (int k = 0; k < 3; k++) {
                    if ((i + k) == row && (j + k) == col) {
                        involves_our_move = true;
                        break;
                    }
                }

                if (involves_our_move) {
                    if (board[i][j] == symbol &&
                        board[i + 1][j + 1] == symbol &&
                        board[i + 2][j + 2] == symbol) {
                        return true;
                    }
                }
            }
        }

        // Check anti-diagonal sequences
        for (int i = 0; i < 3; i++) {
            for (int j = 2; j < 5; j++) {
                bool involves_our_move = false;
                for (int k = 0; k < 3; k++) {
                    if ((i + k) == row && (j - k) == col) {
                        involves_our_move = true;
                        break;
                    }
                }

                if (involves_our_move) {
                    if (board[i][j] == symbol &&
                        board[i + 1][j - 1] == symbol &&
                        board[i + 2][j - 2] == symbol) {
                        return true;
                    }
                }
            }
        }

        return false;
    }

    // Check three in row for 3x3 board
    bool check_three_in_row(vector<vector<char>> board, char symbol) {
        // Check rows
        for (int i = 0; i < 3; i++) {
            if (board[i][0] == symbol && board[i][1] == symbol && board[i][2] == symbol) return true;
        }
        // Check columns
        for (int j = 0; j < 3; j++) {
            if (board[0][j] == symbol && board[1][j] == symbol && board[2][j] == symbol) return true;
        }
        // Check diagonals
        if (board[0][0] == symbol && board[1][1] == symbol && board[2][2] == symbol) return true;
        if (board[0][2] == symbol && board[1][1] == symbol && board[2][0] == symbol) return true;
        return false;
    }

    // Check if move wins immediately
    bool would_win_immediately(vector<vector<char>> board, char symbol, int row, int col) {
        board[row][col] = symbol;
        return check_three_in_row(board, symbol);
    }

    // ========== ENHANCED 4x4 MOVING AI LOGIC ==========

    Move<char>* get_enhanced_smart_4x4_move(Player<char>* player) {
        FourByFour_Moving_Board* board = dynamic_cast<FourByFour_Moving_Board*>(game_board);
        if (!board) return get_random_move(player);

        char symbol = player->get_symbol();
        char opponent_symbol = (symbol == 'X') ? 'O' : 'X';
        vector<vector<char>> board_matrix = game_board->get_board_matrix();

        cout << "Smart AI analyzing 4x4 movement...\n";

        // STRATEGY 1: Win immediately if possible
        vector<pair<int, int>> winning_moves;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (board_matrix[i][j] == 0) {
                    if (board->would_create_three_in_row(symbol, i, j) &&
                        !board->is_oscillating_move(symbol, i, j)) {
                        winning_moves.push_back({ i, j });
                    }
                }
            }
        }
        if (!winning_moves.empty()) {
            auto best_win = get_most_strategic_position(winning_moves);
            cout << "WINNING move to: " << best_win.first << " " << best_win.second << "\n";
            return new Move<char>(best_win.first, best_win.second, symbol);
        }

        // STRATEGY 2: Block opponent from winning
        vector<pair<int, int>> blocking_moves;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (board_matrix[i][j] == 0) {
                    if (board->would_create_three_in_row(opponent_symbol, i, j) &&
                        !board->is_oscillating_move(symbol, i, j)) {
                        blocking_moves.push_back({ i, j });
                    }
                }
            }
        }
        if (!blocking_moves.empty()) {
            auto best_block = get_most_strategic_position(blocking_moves);
            cout << "BLOCKING opponent at: " << best_block.first << " " << best_block.second << "\n";
            return new Move<char>(best_block.first, best_block.second, symbol);
        }

        // STRATEGY 3: Create double threats
        vector<pair<int, int>> double_threat_moves;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (board_matrix[i][j] == 0 && !board->is_oscillating_move(symbol, i, j)) {
                    if (creates_double_threat(board_matrix, symbol, i, j)) {
                        double_threat_moves.push_back({ i, j });
                    }
                }
            }
        }
        if (!double_threat_moves.empty()) {
            auto best_threat = get_most_strategic_position(double_threat_moves);
            cout << "Creating double threat at: " << best_threat.first << " " << best_threat.second << "\n";
            return new Move<char>(best_threat.first, best_threat.second, symbol);
        }

        // STRATEGY 4: Control strategic positions
        vector<pair<int, int>> strategic_moves;
        vector<pair<int, int>> center_positions = { {1,1}, {1,2}, {2,1}, {2,2} };
        for (auto pos : center_positions) {
            if (board_matrix[pos.first][pos.second] == 0 &&
                !board->is_oscillating_move(symbol, pos.first, pos.second)) {
                strategic_moves.push_back(pos);
            }
        }
        if (!strategic_moves.empty()) {
            auto best_strategic = strategic_moves[0];
            cout << "Controlling strategic position at: " << best_strategic.first << " " << best_strategic.second << "\n";
            return new Move<char>(best_strategic.first, best_strategic.second, symbol);
        }

        // STRATEGY 5: Any valid non-oscillating move
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (board_matrix[i][j] == 0 &&
                    !board->is_oscillating_move(symbol, i, j) &&
                    board->can_any_token_move_to(i, j)) {
                    cout << "Strategic non-oscillating move to: " << i << " " << j << "\n";
                    return new Move<char>(i, j, symbol);
                }
            }
        }

        // STRATEGY 6: Fallback to any valid move
        cout << "Using fallback move (oscillation possible)\n";
        return get_random_move(player);
    }

    bool creates_double_threat(vector<vector<char>> board, char symbol, int row, int col) {
        if (board[row][col] != 0) return false;

        board[row][col] = symbol;
        int threats = 0;

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (board[i][j] == 0 && (i != row || j != col)) {
                    if (would_complete_sequence_4x4(board, symbol, i, j)) {
                        threats++;
                        if (threats >= 2) return true;
                    }
                }
            }
        }
        return false;
    }

    bool would_complete_sequence_4x4(vector<vector<char>> board, char symbol, int row, int col) {
        if (board[row][col] != 0) return false;

        board[row][col] = symbol;

        // Check horizontal
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 2; j++) {
                if (board[i][j] == symbol && board[i][j + 1] == symbol && board[i][j + 2] == symbol) {
                    return true;
                }
            }
        }

        // Check vertical
        for (int j = 0; j < 4; j++) {
            for (int i = 0; i < 2; i++) {
                if (board[i][j] == symbol && board[i + 1][j] == symbol && board[i + 2][j] == symbol) {
                    return true;
                }
            }
        }

        // Check diagonals
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 2; j++) {
                if (board[i][j] == symbol && board[i + 1][j + 1] == symbol && board[i + 2][j + 2] == symbol) {
                    return true;
                }
            }
        }

        for (int i = 0; i < 2; i++) {
            for (int j = 2; j < 4; j++) {
                if (board[i][j] == symbol && board[i + 1][j - 1] == symbol && board[i + 2][j - 2] == symbol) {
                    return true;
                }
            }
        }

        return false;
    }

    pair<int, int> get_most_strategic_position(const vector<pair<int, int>>& moves) {
        for (auto move : moves) {
            if ((move.first == 1 || move.first == 2) && (move.second == 1 || move.second == 2)) {
                return move;
            }
        }
        for (auto move : moves) {
            if (move.first == 1 || move.first == 2 || move.second == 1 || move.second == 2) {
                return move;
            }
        }
        return moves[0];
    }

    // ========== MAIN MOVE FUNCTIONS ==========

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
        else if (game_type == "4x4") {
            cout << "\n" << player->get_name() << ", select target position to move to (row col 0-3): ";
            while (!(cin >> x >> y) || x < 0 || x > 3 || y < 0 || y > 3) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Please enter two valid numbers (0-3): ";
            }
            cout << "Moving to: " << x << " " << y << "\n";
            letter = player->get_symbol();
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
        else if (game_type == "pyramid" && pyramid_game) {
            auto move = pyramid_game->getComputerMove(false);
            if (move.first != -1 && move.second != -1) {
                cout << "Random pyramid move at: " << move.first << " " << move.second << "\n";
                return new Move<char>(move.first, move.second, player->get_symbol());
            }
        }
        else {
            return get_random_move(player);
        }
        return get_random_move(player);
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
        else if (game_type == "4x4") {
            return get_enhanced_smart_4x4_move(player);
        }
        else if (game_type == "pyramid" && pyramid_game) {
            return get_smart_pyramid_move(player);
        }
        else {
            return get_smart_misere_move(player);
        }
    }

    // Other game type AI implementations
    Move<char>* get_smart_misere_move(Player<char>* player) {
        char symbol = player->get_symbol();
        vector<vector<char>> board_matrix = game_board->get_board_matrix();

        // Simple strategy: avoid creating three in a row
        vector<pair<int, int>> safe_moves;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board_matrix[i][j] == 0) {
                    if (!would_win_immediately(board_matrix, symbol, i, j)) {
                        safe_moves.push_back({ i, j });
                    }
                }
            }
        }

        if (!safe_moves.empty()) {
            auto move = safe_moves[rand() % safe_moves.size()];
            cout << "Safe strategic move at: " << move.first << " " << move.second << "\n";
            return new Move<char>(move.first, move.second, symbol);
        }

        return get_random_move(player);
    }

    Move<char>* get_smart_5x5_move(Player<char>* player) {
        char symbol = player->get_symbol();
        char opponent_symbol = (symbol == 'X') ? 'O' : 'X';
        vector<vector<char>> board_matrix = game_board->get_board_matrix();

        // Strategy 1: Complete a sequence if possible
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                if (board_matrix[i][j] == 0) {
                    if (would_complete_sequence(board_matrix, symbol, i, j)) {
                        cout << "Completing sequence at: " << i << " " << j << "\n";
                        return new Move<char>(i, j, symbol);
                    }
                }
            }
        }

        // Strategy 2: Block opponent sequences
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                if (board_matrix[i][j] == 0) {
                    if (would_complete_sequence(board_matrix, opponent_symbol, i, j)) {
                        cout << "Blocking opponent at: " << i << " " << j << "\n";
                        return new Move<char>(i, j, symbol);
                    }
                }
            }
        }

        return get_random_move(player);
    }

    Move<char>* get_smart_word_move(Player<char>* player) {
        vector<char> common_letters = { 'E', 'A', 'R', 'I', 'O', 'T', 'N', 'S', 'L', 'C' };
        char letter = common_letters[rand() % common_letters.size()];
        return get_random_letter_move(player, letter);
    }

    Move<char>* get_smart_infinity_move(Player<char>* player) {
        Infinity_Board* infinity_board = dynamic_cast<Infinity_Board*>(game_board);
        if (!infinity_board) return get_random_move(player);

        char symbol = player->get_symbol();
        char opponent_symbol = (symbol == 'X') ? 'O' : 'X';
        vector<vector<char>> board_matrix = game_board->get_board_matrix();

        // Strategy 1: Win immediately if possible
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board_matrix[i][j] == 0) {
                    if (would_win_immediately(board_matrix, symbol, i, j)) {
                        cout << "Winning immediately at: " << i << " " << j << "\n";
                        return new Move<char>(i, j, symbol);
                    }
                }
            }
        }

        // Strategy 2: Block opponent from winning
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board_matrix[i][j] == 0) {
                    if (would_win_immediately(board_matrix, opponent_symbol, i, j)) {
                        cout << "Blocking opponent at: " << i << " " << j << "\n";
                        return new Move<char>(i, j, symbol);
                    }
                }
            }
        }

        return get_random_move(player);
    }

    Move<char>* get_smart_pyramid_move(Player<char>* player) {
        if (!pyramid_game) return get_random_move(player);
        auto move = pyramid_game->getComputerMove(true);
        return new Move<char>(move.first, move.second, player->get_symbol());
    }

    // Basic move generators
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
            return new Move<char>(0, 0, player->get_symbol());
        }

        int random_index = rand() % empty_cells.size();
        auto move = empty_cells[random_index];
        cout << "Random move at: " << move.first << " " << move.second << "\n";
        return new Move<char>(move.first, move.second, player->get_symbol());
    }

    Move<char>* get_random_letter_move(Player<char>* player, char letter) {
        auto move = get_random_move(player);
        return new Move<char>(move->get_x(), move->get_y(), letter);
    }
};

#endif