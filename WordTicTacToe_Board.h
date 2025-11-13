#pragma once
#ifndef WORDTICTACTOE_BOARD_H
#define WORDTICTACTOE_BOARD_H

#include "BoardGame_Classes.h"
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

class WordTicTacToe_Board : public Board<char> {
private:
    vector<string> dictionary;
    vector<string> found_words;

public:
    WordTicTacToe_Board() : Board(3, 3) {
        n_moves = 0;
      
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                board[i][j] = 0;
            }
        }
        load_dictionary();
    }

    bool update_board(Move<char>* move) override {
        int x = move->get_x();
        int y = move->get_y();
        char symbol = move->get_symbol();

        if (x >= 0 && x < 3 && y >= 0 && y < 3 && board[x][y] == 0) {
            board[x][y] = symbol;
            n_moves++;
            return true;
        }
        return false;
    }

    bool is_win(Player<char>* player) override {
        char symbol = player->get_symbol();
        return check_for_winning_word(symbol);
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

 
    vector<string> get_all_sequences() {
        vector<string> sequences;

   
        for (int i = 0; i < 3; i++) {
            string row = "";
            for (int j = 0; j < 3; j++) {
                row += (board[i][j] == 0) ? ' ' : board[i][j];
            }
            sequences.push_back(row);
        }

      
        for (int j = 0; j < 3; j++) {
            string col = "";
            for (int i = 0; i < 3; i++) {
                col += (board[i][j] == 0) ? ' ' : board[i][j];
            }
            sequences.push_back(col);
        }

        string diag1 = "";
        string diag2 = "";
        for (int i = 0; i < 3; i++) {
            diag1 += (board[i][i] == 0) ? ' ' : board[i][i];
            diag2 += (board[i][2 - i] == 0) ? ' ' : board[i][2 - i];
        }
        sequences.push_back(diag1);
        sequences.push_back(diag2);

        return sequences;
    }

    bool check_for_winning_word(char player_symbol) {
        vector<string> sequences = get_all_sequences();

        for (const string& sequence : sequences) {
            
            if (sequence.find(' ') == string::npos) {
                
                if (is_word_valid(sequence) &&
                    find(found_words.begin(), found_words.end(), sequence) == found_words.end()) {
                    found_words.push_back(sequence);
                    return true;
                }
            }
        }
        return false;
    }

    bool is_word_valid(const string& word) {
        return find(dictionary.begin(), dictionary.end(), word) != dictionary.end();
    }

    
    string get_winning_word() {
        if (!found_words.empty()) {
            return found_words.back();
        }
        return "";
    }

private:
    void load_dictionary() {
        ifstream file("dic.txt");
        string word;

        if (file.is_open()) {
            cout << "Loading dictionary from dic.txt...\n";
            int word_count = 0;
            while (getline(file, word)) {
                
                transform(word.begin(), word.end(), word.begin(), ::toupper);
                dictionary.push_back(word);
                word_count++;
            }
            file.close();
            cout << "Dictionary loaded with " << word_count << " words.\n";
        }
        else {
          
            cout << "Warning: dic.txt not found. Using fallback dictionary.\n";
            dictionary = { "CAT", "DOG", "BAT", "RAT", "MAT", "HAT", "BIT", "FIT", "SIT", "TOP", "BAG", "CAR" };
        }
    }
};

#endif