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
#include <unordered_map>
#include <stack>



class Simple_UI : public UI<char> {
private:
    Board<char>* game_board;
    string game_type;
    PyramidTicTacToe* pyramid_game;
    unordered_map<Player<char>*, int> invalid_move_counts;

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
        else if (game_type == "pyramid") {
            cout << "Pyramid Tic-Tac-Toe - Triangular board challenge!\n";
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
        else {
            return get_strategic_computer_move(player);
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

private:
    // ========== STRATEGIC COMPUTER MOVES ==========

    Move<char>* get_strategic_computer_move(Player<char>* player) {
        cout << "\n" << player->get_name() << " (Computer) is planning a move...\n";
        this_thread::sleep_for(chrono::milliseconds(800));

        if (game_type == "misere") {
            return get_enhanced_misere_move(player);
        }
        else if (game_type == "5x5") {
            return get_enhanced_5x5_move(player);
        }
        else if (game_type == "word") {
            return get_enhanced_word_move(player);
        }
        else if (game_type == "infinity") {
            return get_enhanced_infinity_move(player);
        }
        else if (game_type == "4x4") {
            return get_enhanced_4x4_move(player);
        }
        else if (game_type == "pyramid") {
            return get_enhanced_pyramid_move(player);
        }

        return get_fallback_move(player);
    }

    // ========== MISERE GAME (Fixed with threat detection) ==========
    // ========== MISERE GAME (Fixed with proper strategy) ==========

    Move<char>* get_enhanced_misere_move(Player<char>* player) {
        char symbol = player->get_symbol();
        char opponent = (symbol == 'X') ? 'O' : 'X';
        vector<vector<char>> board = game_board->get_board_matrix();

        cout << player->get_name() << " analyzing misere threats...\n";

        // Find all empty positions
        vector<pair<int, int>> empty_positions;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == 0) {
                    empty_positions.push_back({ i, j });
                }
            }
        }

        if (empty_positions.empty()) {
            return get_fallback_move(player);
        }

        // Phase 1: Check if any move would immediately lose (make three in a row)
        vector<pair<int, int>> losing_moves;
        vector<pair<int, int>> safe_moves;

        for (auto& pos : empty_positions) {
            board[pos.first][pos.second] = symbol;
            bool would_lose = check_three_in_row(board, symbol);
            board[pos.first][pos.second] = 0;

            if (would_lose) {
                losing_moves.push_back(pos);
            }
            else {
                safe_moves.push_back(pos);
            }
        }

        // Phase 2: If we have safe moves, choose the best one
        if (!safe_moves.empty()) {
            // Evaluate each safe move
            int best_score = -10000;
            pair<int, int> best_move = safe_moves[0];

            for (auto& move : safe_moves) {
                int score = 0;
                int row = move.first;
                int col = move.second;

                // Position value: Corners are safest in misere
                if ((row == 0 && col == 0) || (row == 0 && col == 2) ||
                    (row == 2 && col == 0) || (row == 2 && col == 2)) {
                    score += 10;  // Corners are best
                }
                else if (row == 1 && col == 1) {
                    score += 5;   // Center is medium risk
                }
                else {
                    score += 7;   // Edges are good
                }

                // Check if this move creates a forcing situation for opponent
                board[row][col] = symbol;

                // Count how many ways opponent could be forced to lose
                int opponent_forced_lose_count = 0;

                // Check all opponent responses
                for (int i = 0; i < 3; i++) {
                    for (int j = 0; j < 3; j++) {
                        if (board[i][j] == 0) {
                            board[i][j] = opponent;

                            // Check if opponent made three in a row (they lose!)
                            if (check_three_in_row(board, opponent)) {
                                opponent_forced_lose_count++;
                            }

                            board[i][j] = 0;
                        }
                    }
                }

                score += opponent_forced_lose_count * 20;

                // Check if we leave opponent a chance to force us
                bool opponent_can_force_us = false;
                for (int i = 0; i < 3; i++) {
                    for (int j = 0; j < 3; j++) {
                        if (board[i][j] == 0) {
                            board[i][j] = opponent;

                            // After opponent move, check if any of our moves would lose
                            for (int x = 0; x < 3; x++) {
                                for (int y = 0; y < 3; y++) {
                                    if (board[x][y] == 0) {
                                        board[x][y] = symbol;
                                        if (check_three_in_row(board, symbol)) {
                                            opponent_can_force_us = true;
                                        }
                                        board[x][y] = 0;
                                    }
                                }
                            }

                            board[i][j] = 0;
                            if (opponent_can_force_us) break;
                        }
                    }
                    if (opponent_can_force_us) break;
                }

                if (opponent_can_force_us) {
                    score -= 15;
                }

                board[row][col] = 0;

                if (score > best_score) {
                    best_score = score;
                    best_move = move;
                }
            }

            cout << "Choosing strategic move at: " << best_move.first << " " << best_move.second << "\n";
            return new Move<char>(best_move.first, best_move.second, symbol);
        }

        // Phase 3: If all moves are losing, choose one that might force opponent to also lose
        if (!losing_moves.empty()) {
            cout << "All moves dangerous, choosing least bad option...\n";

            // Try to find a move where opponent also has dangerous response
            for (auto& move : losing_moves) {
                board[move.first][move.second] = symbol;

                // Check if opponent also has dangerous responses
                for (int i = 0; i < 3; i++) {
                    for (int j = 0; j < 3; j++) {
                        if (board[i][j] == 0) {
                            board[i][j] = opponent;
                            if (check_three_in_row(board, opponent)) {
                                // At least we can force opponent to lose too
                                board[i][j] = 0;
                                board[move.first][move.second] = 0;
                                cout << "Forcing mutual loss at: " << move.first << " " << move.second << "\n";
                                return new Move<char>(move.first, move.second, symbol);
                            }
                            board[i][j] = 0;
                        }
                    }
                }

                board[move.first][move.second] = 0;
            }

            // Just pick first losing move
            auto move = losing_moves[0];
            cout << "No choice, taking dangerous move: " << move.first << " " << move.second << "\n";
            return new Move<char>(move.first, move.second, symbol);
        }

        return get_fallback_move(player);
    }

    // ========== 5x5 GAME ==========

    Move<char>* get_enhanced_5x5_move(Player<char>* player) {
        char symbol = player->get_symbol();
        char opponent = (symbol == 'X') ? 'O' : 'X';
        vector<vector<char>> board = game_board->get_board_matrix();

        // First, check immediate wins
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                if (board[i][j] == 0 && would_complete_sequence(board, symbol, i, j)) {
                    cout << "Completing sequence at: " << i << " " << j << "\n";
                    return new Move<char>(i, j, symbol);
                }
            }
        }

        // Block opponent sequences
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                if (board[i][j] == 0 && would_complete_sequence(board, opponent, i, j)) {
                    cout << "Blocking opponent at: " << i << " " << j << "\n";
                    return new Move<char>(i, j, symbol);
                }
            }
        }

        // Take strategic positions
        vector<pair<int, int>> strategic_positions = {
            {2,2}, {1,2}, {2,1}, {2,3}, {3,2},  // Center area
            {1,1}, {1,3}, {3,1}, {3,3}           // Inner ring
        };

        for (auto pos : strategic_positions) {
            if (board[pos.first][pos.second] == 0) {
                cout << "Taking strategic position: " << pos.first << " " << pos.second << "\n";
                return new Move<char>(pos.first, pos.second, symbol);
            }
        }

        return get_fallback_move(player);
    }

    // ========== WORD GAME (Fixed word detection) ==========
 

    Move<char>* get_enhanced_word_move(Player<char>* player) {
        char symbol = player->get_symbol();
        char opponent = (symbol == 'A') ? 'B' : 'A';
        vector<vector<char>> board = game_board->get_board_matrix();

        cout << player->get_name() << " analyzing word board...\n";

        // Cast to WordTicTacToe_Board to access dictionary methods
        WordTicTacToe_Board* wordBoard = dynamic_cast<WordTicTacToe_Board*>(game_board);
        if (!wordBoard) {
            return get_fallback_move(player);
        }

        // Get valid moves according to dictionary
        vector<pair<int, int>> validMoves = wordBoard->getValidLetterPositions(symbol);

        if (validMoves.empty()) {
            cout << "No valid word moves available!\n";
            return get_fallback_move(player);
        }

        // STRATEGY 1: Check for immediate word completion
        for (auto& pos : validMoves) {
            // Test if placing here would complete a valid word
            board[pos.first][pos.second] = symbol;

            // Check if this creates a complete valid word
            WordTicTacToe_Board tempBoard = *wordBoard;
            Move<char> testMove(pos.first, pos.second, symbol);
            tempBoard.update_board(&testMove);

            auto validWords = tempBoard.getAllValidWords();
            if (!validWords.empty()) {
                cout << "Completing word at: " << pos.first << " " << pos.second << "\n";
                return new Move<char>(pos.first, pos.second, symbol);
            }

            board[pos.first][pos.second] = '.';
        }

        // STRATEGY 2: Take strategic positions
        // Center is most valuable
        if (find(validMoves.begin(), validMoves.end(), make_pair(1, 1)) != validMoves.end()) {
            cout << "Taking center: 1 1\n";
            return new Move<char>(1, 1, symbol);
        }

        // Corners
        vector<pair<int, int>> corners = { {0,0}, {0,2}, {2,0}, {2,2} };
        for (auto& corner : corners) {
            if (find(validMoves.begin(), validMoves.end(), corner) != validMoves.end()) {
                cout << "Taking corner: " << corner.first << " " << corner.second << "\n";
                return new Move<char>(corner.first, corner.second, symbol);
            }
        }

        // Any valid move
        int idx = rand() % validMoves.size();
        auto pos = validMoves[idx];
        cout << "Random valid move at: " << pos.first << " " << pos.second << "\n";
        return new Move<char>(pos.first, pos.second, symbol);
    }

    // ========== INFINITY GAME ==========

    Move<char>* get_enhanced_infinity_move(Player<char>* player) {
        char symbol = player->get_symbol();
        char opponent = (symbol == 'X') ? 'O' : 'X';
        vector<vector<char>> board = game_board->get_board_matrix();

        // Strategy based on removal timing
        Infinity_Board* infinity_board = dynamic_cast<Infinity_Board*>(game_board);
        if (infinity_board) {
            int moves_until_removal = infinity_board->get_removal_countdown();

            if (moves_until_removal == 1) {
                // About to remove - place in corner (lasts longer)
                vector<pair<int, int>> corners = { {0,0}, {0,2}, {2,0}, {2,2} };
                for (auto pos : corners) {
                    if (board[pos.first][pos.second] == 0) {
                        cout << "Safe corner before removal: " << pos.first << " " << pos.second << "\n";
                        return new Move<char>(pos.first, pos.second, symbol);
                    }
                }
            }
        }

        // Try to win immediately
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == 0 && would_win_immediately(board, symbol, i, j)) {
                    cout << "Winning move: " << i << " " << j << "\n";
                    return new Move<char>(i, j, symbol);
                }
            }
        }

        // Block opponent from winning
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == 0 && would_win_immediately(board, opponent, i, j)) {
                    cout << "Blocking opponent: " << i << " " << j << "\n";
                    return new Move<char>(i, j, symbol);
                }
            }
        }

        // Take durable positions (corners last longer)
        vector<pair<int, int>> corners = { {0,0}, {0,2}, {2,0}, {2,2} };
        for (auto corner : corners) {
            if (board[corner.first][corner.second] == 0) {
                cout << "Durable corner: " << corner.first << " " << corner.second << "\n";
                return new Move<char>(corner.first, corner.second, symbol);
            }
        }

        // Take center
        if (board[1][1] == 0) {
            cout << "Taking center: 1 1\n";
            return new Move<char>(1, 1, symbol);
        }

        return get_fallback_move(player);
    }

    // ========== 4�4 MOVING GAME (Fixed movement logic) ==========

    Move<char>* get_enhanced_4x4_move(Player<char>* player) {
        FourByFour_Moving_Board* moving_board = dynamic_cast<FourByFour_Moving_Board*>(game_board);
        if (!moving_board) {
            cout << "ERROR: Not a 4x4 board\n";
            return get_fallback_move(player);
        }

        char symbol = player->get_symbol();
        char opponent = (symbol == 'X') ? 'O' : 'X';

        cout << "4x4 AI analyzing movement...\n";

        // Generate valid moves
        vector<pair<int, int>> valid_moves = moving_board->generate_valid_moves_for_player(symbol);

        if (valid_moves.empty()) {
            cout << "No valid moves available!\n";
            return get_fallback_move(player);
        }

        // 1. Immediate win
        for (auto& move : valid_moves) {
            if (moving_board->would_create_three_in_row(symbol, move.first, move.second)) {
                cout << "WINNING move to: " << move.first << " " << move.second << "\n";
                return new Move<char>(move.first, move.second, symbol);
            }
        }

        // 2. Block opponent from winning
        vector<pair<int, int>> opponent_moves = moving_board->generate_valid_moves_for_player(opponent);
        for (auto& opp_move : opponent_moves) {
            if (moving_board->would_create_three_in_row(opponent, opp_move.first, opp_move.second)) {
                // Can we move to block?
                for (auto& move : valid_moves) {
                    if (move.first == opp_move.first && move.second == opp_move.second) {
                        cout << "BLOCKING opponent at: " << move.first << " " << move.second << "\n";
                        return new Move<char>(move.first, move.second, symbol);
                    }
                }
            }
        }

        // 3. Control center
        vector<pair<int, int>> center = { {1,1}, {1,2}, {2,1}, {2,2} };
        for (auto& move : valid_moves) {
            if (find(center.begin(), center.end(), move) != center.end()) {
                cout << "Controlling center: " << move.first << " " << move.second << "\n";
                return new Move<char>(move.first, move.second, symbol);
            }
        }

        // 4. Move toward opponent
        auto opp_positions = moving_board->get_player_positions(opponent);
        if (!opp_positions.empty()) {
            pair<int, int> best_move = valid_moves[0];
            int min_distance = 100;

            for (auto& move : valid_moves) {
                for (auto& opp_pos : opp_positions) {
                    int distance = abs(move.first - opp_pos.first) + abs(move.second - opp_pos.second);
                    if (distance < min_distance) {
                        min_distance = distance;
                        best_move = move;
                    }
                }
            }

            cout << "Aggressive move to: " << best_move.first << " " << best_move.second << "\n";
            return new Move<char>(best_move.first, best_move.second, symbol);
        }

        // 5. Any valid move
        auto move = valid_moves[0];
        cout << "Moving to: " << move.first << " " << move.second << "\n";
        return new Move<char>(move.first, move.second, symbol);
    }

    // ========== PYRAMID GAME ==========

    Move<char>* get_enhanced_pyramid_move(Player<char>* player) {
        if (!pyramid_game) return get_fallback_move(player);

        auto move = pyramid_game->getComputerMove(true);
        if (move.first != -1 && move.second != -1) {
            cout << "Pyramid move at: " << move.first << " " << move.second << "\n";
            return new Move<char>(move.first, move.second, player->get_symbol());
        }

        return get_fallback_move(player);
    }

    // ========== HELPER FUNCTIONS ==========

    Move<char>* get_fallback_move(Player<char>* player) {
        int rows = game_board->get_rows();
        int cols = game_board->get_columns();
        vector<vector<char>> board_matrix = game_board->get_board_matrix();

        // Find any empty cell
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (board_matrix[i][j] == 0) {
                    cout << "Fallback move at: " << i << " " << j << "\n";
                    return new Move<char>(i, j, player->get_symbol());
                }
            }
        }

        return new Move<char>(0, 0, player->get_symbol());
    }

    bool check_three_in_row(const vector<vector<char>>& board, char symbol) {
        // Check rows
        for (int i = 0; i < board.size(); i++) {
            int size = board[i].size();
            for (int j = 0; j <= size - 3; j++) {
                if (board[i][j] == symbol &&
                    board[i][j + 1] == symbol &&
                    board[i][j + 2] == symbol) {
                    return true;
                }
            }
        }

        // Check columns
        int rows = board.size();
        if (rows == 0) return false;
        int cols = board[0].size();

        for (int j = 0; j < cols; j++) {
            for (int i = 0; i <= rows - 3; i++) {
                if (board[i][j] == symbol &&
                    board[i + 1][j] == symbol &&
                    board[i + 2][j] == symbol) {
                    return true;
                }
            }
        }

        // Check diagonals
        if (rows >= 3 && cols >= 3) {
            // Main diagonal
            if (board[0][0] == symbol && board[1][1] == symbol && board[2][2] == symbol) return true;
            // Anti-diagonal
            if (board[0][2] == symbol && board[1][1] == symbol && board[2][0] == symbol) return true;
        }

        return false;
    }

    int count_sequences(const vector<vector<char>>& board, char symbol) {
        int count = 0;
        int size = board.size();
        if (size == 0) return 0;
        int cols = board[0].size();

        // Check horizontal sequences
        for (int i = 0; i < size; i++) {
            for (int j = 0; j <= cols - 3; j++) {
                if (board[i][j] == symbol &&
                    board[i][j + 1] == symbol &&
                    board[i][j + 2] == symbol) {
                    count++;
                }
            }
        }

        // Check vertical sequences
        for (int j = 0; j < cols; j++) {
            for (int i = 0; i <= size - 3; i++) {
                if (board[i][j] == symbol &&
                    board[i + 1][j] == symbol &&
                    board[i + 2][j] == symbol) {
                    count++;
                }
            }
        }

        // Check diagonal (top-left to bottom-right)
        for (int i = 0; i <= size - 3; i++) {
            for (int j = 0; j <= cols - 3; j++) {
                if (board[i][j] == symbol &&
                    board[i + 1][j + 1] == symbol &&
                    board[i + 2][j + 2] == symbol) {
                    count++;
                }
            }
        }

        // Check diagonal (top-right to bottom-left)
        for (int i = 0; i <= size - 3; i++) {
            for (int j = 2; j < cols; j++) {
                if (board[i][j] == symbol &&
                    board[i + 1][j - 1] == symbol &&
                    board[i + 2][j - 2] == symbol) {
                    count++;
                }
            }
        }

        return count;
    }

    bool would_complete_sequence(vector<vector<char>> board, char symbol, int row, int col) {
        if (board[row][col] != 0) return false;

        board[row][col] = symbol;

        // Check horizontal
        int cols = board[0].size();
        for (int start_col = max(0, col - 2); start_col <= min(cols - 3, col); start_col++) {
            if (board[row][start_col] == symbol &&
                board[row][start_col + 1] == symbol &&
                board[row][start_col + 2] == symbol) {
                return true;
            }
        }

        // Check vertical
        int rows = board.size();
        for (int start_row = max(0, row - 2); start_row <= min(rows - 3, row); start_row++) {
            if (board[start_row][col] == symbol &&
                board[start_row + 1][col] == symbol &&
                board[start_row + 2][col] == symbol) {
                return true;
            }
        }

        // Check diagonals
        if (rows >= 3 && cols >= 3) {
            // Main diagonal
            for (int offset = -2; offset <= 0; offset++) {
                int r = row + offset;
                int c = col + offset;
                if (r >= 0 && r <= rows - 3 && c >= 0 && c <= cols - 3) {
                    if (board[r][c] == symbol &&
                        board[r + 1][c + 1] == symbol &&
                        board[r + 2][c + 2] == symbol) {
                        return true;
                    }
                }
            }

            // Anti-diagonal
            for (int offset = -2; offset <= 0; offset++) {
                int r = row + offset;
                int c = col - offset;
                if (r >= 0 && r <= rows - 3 && c >= 2 && c < cols) {
                    if (board[r][c] == symbol &&
                        board[r + 1][c - 1] == symbol &&
                        board[r + 2][c - 2] == symbol) {
                        return true;
                    }
                }
            }
        }

        return false;
    }

    bool would_win_immediately(vector<vector<char>> board, char symbol, int row, int col) {
        if (board[row][col] != 0) return false;

        board[row][col] = symbol;
        return check_three_in_row(board, symbol);
    }

    // Human move function
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
        else if (game_type == "5x5") {
            cout << "\n" << player->get_name() << ", it is your turn to play.\n";
            cout << "You are playing with symbol: " << player->get_symbol() << "\n";
            cout << "Please enter your move as two numbers (0-4 for row and column): ";
            while (!(cin >> x >> y) || x < 0 || x > 4 || y < 0 || y > 4) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please enter two numbers between 0 and 4: ";
            }
            cout << "You have selected position: Row " << x << ", Column " << y << "\n";
            letter = player->get_symbol();
        }
        else if (game_type == "pyramid") {
            cout << "\n" << player->get_name() << ", it is your turn to play.\n";
            cout << "Valid coordinates:\n";
            cout << "Row 0: [0,0]\n";
            cout << "Row 1: [1,0] [1,1] [1,2]\n";
            cout << "Row 2: [2,0] [2,1] [2,2] [2,3] [2,4]\n";
            cout << "Please enter your move (row col): ";
            while (!(cin >> x >> y)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please enter two numbers: ";
            }
            cout << "You have selected position: Row " << x << ", Column " << y << "\n";
            letter = player->get_symbol();
        }
        else {
            cout << "\n" << player->get_name() << ", it is your turn to play.\n";
            cout << "You are playing with symbol: " << player->get_symbol() << "\n";
            cout << "Please enter your move as two numbers (0-2 for row and column): ";
            while (!(cin >> x >> y) || x < 0 || x > 2 || y < 0 || y > 2) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please enter two numbers between 0 and 2: ";
            }
            cout << "You have selected position: Row " << x << ", Column " << y << "\n";
            letter = player->get_symbol();
        }

        return new Move<char>(x, y, letter);
    }
};

#endif