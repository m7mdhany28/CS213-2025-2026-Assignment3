#pragma once
#ifndef WORDTICTACTOE_BOARD_H
#define WORDTICTACTOE_BOARD_H

#include "BoardGame_Classes.h"
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <iostream>

using namespace std;

class WordTicTacToe_Board : public Board<char> {
private:
    vector<string> dictionary;
    vector<string> found_words;
    vector<vector<char>> player_ownership;

public:
    WordTicTacToe_Board() : Board(3, 3) {
        n_moves = 0;
        player_ownership.resize(3, vector<char>(3, ' '));
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                board[i][j] = 0;
            }
        }
        load_dictionary();
    }

    bool update_board(Move<char>* move) override {
        int row = move->get_x();
        int col = move->get_y();
        char letter = move->get_symbol();
        char player_symbol = (n_moves % 2 == 0) ? 'X' : 'O';

        if (row >= 0 && row < 3 && col >= 0 && col < 3 && board[row][col] == 0) {
            board[row][col] = letter;
            player_ownership[row][col] = player_symbol;
            n_moves++;
            return true;
        }
        return false;
    }

    bool is_win(Player<char>* player) override {
        char player_symbol = player->get_symbol();
        return check_for_player_word(player_symbol);
    }

    bool is_lose(Player<char>* player) override {
        return false;
    }

    bool is_draw(Player<char>* player) override {
        return (n_moves == 9) && !is_win(player);
    }

    bool game_is_over(Player<char>* player) override {
        return is_win(player) || is_draw(player);
    }

    // REMOVED override specifier
    void display_board() {
        cout << "\n   0   1   2\n";
        cout << "  -----------\n";
        for (int i = 0; i < 3; i++) {
            cout << i << "|";
            for (int j = 0; j < 3; j++) {
                cout << " " << (board[i][j] == 0 ? ' ' : board[i][j]) << " |";
            }
            cout << "\n  -----------\n";
        }
    }

    string get_winning_word() {
        if (!found_words.empty()) {
            return found_words.back();
        }
        return "";
    }

private:
    bool check_for_player_word(char player_symbol) {
        vector<string> sequences = get_player_sequences(player_symbol);
        for (const string& word : sequences) {
            if (is_word_valid(word) &&
                find(found_words.begin(), found_words.end(), word) == found_words.end()) {
                found_words.push_back(word);
                return true;
            }
        }
        return false;
    }

    vector<string> get_player_sequences(char player_symbol) {
        vector<string> sequences;
        // Check rows
        for (int row = 0; row < 3; row++) {
            string word = "";
            bool valid_row = true;
            for (int col = 0; col < 3; col++) {
                if (player_ownership[row][col] != player_symbol && board[row][col] != 0) {
                    valid_row = false;
                    break;
                }
                word += (board[row][col] == 0) ? ' ' : board[row][col];
            }
            if (valid_row && word.find(' ') == string::npos) {
                sequences.push_back(word);
            }
        }
        // Check columns
        for (int col = 0; col < 3; col++) {
            string word = "";
            bool valid_col = true;
            for (int row = 0; row < 3; row++) {
                if (player_ownership[row][col] != player_symbol && board[row][col] != 0) {
                    valid_col = false;
                    break;
                }
                word += (board[row][col] == 0) ? ' ' : board[row][col];
            }
            if (valid_col && word.find(' ') == string::npos) {
                sequences.push_back(word);
            }
        }
        // Check diagonals
        string diag1 = "", diag2 = "";
        bool valid_diag1 = true, valid_diag2 = true;
        for (int i = 0; i < 3; i++) {
            if (player_ownership[i][i] != player_symbol && board[i][i] != 0) valid_diag1 = false;
            diag1 += (board[i][i] == 0) ? ' ' : board[i][i];
            if (player_ownership[i][2 - i] != player_symbol && board[i][2 - i] != 0) valid_diag2 = false;
            diag2 += (board[i][2 - i] == 0) ? ' ' : board[i][2 - i];
        }
        if (valid_diag1 && diag1.find(' ') == string::npos) sequences.push_back(diag1);
        if (valid_diag2 && diag2.find(' ') == string::npos) sequences.push_back(diag2);
        return sequences;
    }

    bool is_word_valid(const string& word) {
        return find(dictionary.begin(), dictionary.end(), word) != dictionary.end();
    }

    void load_dictionary() {
        dictionary = {
            "CAT", "DOG", "BAT", "RAT", "MAT", "HAT", "BIT", "FIT", "SIT",
            "TOP", "BAG", "CAR", "BED", "RED", "SUN", "RUN", "MAN", "CAN",
            "PAN", "MAP", "CAP", "TAP", "LAP", "GAP", "ZOO", "BOY", "TOY"
        };
    }
};

#endif