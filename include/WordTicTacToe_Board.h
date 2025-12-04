#pragma once
#ifndef WORDTICTACTOE_BOARD_H
#define WORDTICTACTOE_BOARD_H

#include "BoardGame_Classes.h"
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <cctype>
#include <set>

using namespace std;

class WordTicTacToe_Board : public Board<char> {
private:
    set<string> dictionary;
    string last_winning_word;
    vector<vector<char>> player_ownership;
    vector<string> all_valid_words;

public:
    WordTicTacToe_Board() : Board(3, 3) {
        n_moves = 0;
        player_ownership.resize(3, vector<char>(3, ' '));
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                board[i][j] = 0;
            }
        }
        load_dictionary("dic.txt");
        all_valid_words.clear();
        last_winning_word = "";
    }

    bool update_board(Move<char>* move) override {
        int row = move->get_x();
        int col = move->get_y();
        char letter = toupper(move->get_symbol());  // Ensure uppercase
        char player_symbol = (n_moves % 2 == 0) ? 'X' : 'O';

        if (row >= 0 && row < 3 && col >= 0 && col < 3 && board[row][col] == 0) {
            board[row][col] = letter;
            player_ownership[row][col] = player_symbol;
            n_moves++;

            // Debug output
            cout << player_symbol << " placed '" << letter << "' at (" << row << "," << col << ")\n";

            // Check immediately if this created a winning word
            check_for_words();

            return true;
        }
        return false;
    }

    bool is_win(Player<char>* player) override {
        char player_symbol = player->get_symbol();

        // Check all words to see if this player owns any
        for (const string& word : all_valid_words) {
            // Find which positions contain this word
            vector<pair<int, int>> positions = get_word_positions(word);

            // Check if all positions belong to this player
            bool all_owned_by_player = true;
            for (auto& pos : positions) {
                if (player_ownership[pos.first][pos.second] != player_symbol) {
                    all_owned_by_player = false;
                    break;
                }
            }

            if (all_owned_by_player) {
                last_winning_word = word;
                return true;
            }
        }

        return false;
    }

    bool is_lose(Player<char>* player) override {
        return false;
    }

    bool is_draw(Player<char>* player) override {
        if (n_moves == 9) {
            // Check if any player has a winning word
            vector<vector<char>> test_board = board;

            // Convert board to string for checking
            vector<string> words = get_all_words_on_board();

            bool any_valid_word = false;
            for (const string& word : words) {
                string upper_word = word;
                transform(upper_word.begin(), upper_word.end(), upper_word.begin(), ::toupper);

                if (dictionary.find(upper_word) != dictionary.end()) {
                    any_valid_word = true;
                    break;
                }
            }

            // Draw if no valid words AND board is full
            return !any_valid_word;
        }
        return false;
    }

    bool game_is_over(Player<char>* player) override {
        return is_win(player) || is_draw(player) || n_moves == 9;
    }

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

        // Show valid words found
        if (!all_valid_words.empty()) {
            cout << "Valid words on board: ";
            for (const string& word : all_valid_words) {
                cout << word << " ";
            }
            cout << "\n";
        }
    }

    string get_winning_word() const {
        return last_winning_word;
    }

private:
    void load_dictionary(const string& filename) {
        dictionary.clear();

        // Use the dic.txt file
        ifstream file(filename);
        if (!file.is_open()) {
            // Fallback to common 3-letter words
            vector<string> common_words = {
                "CAT", "DOG", "BAT", "RAT", "MAT", "HAT", "BIT", "FIT", "SIT",
                "TOP", "BAG", "CAR", "BED", "RED", "SUN", "RUN", "MAN", "CAN",
                "PAN", "MAP", "CAP", "TAP", "LAP", "GAP", "ZOO", "BOY", "TOY",
                "TEA", "EGG", "EYE", "ICE", "KEY", "LEG", "MUD", "NET", "OWL",
                "PEN", "QUE", "RUG", "SEA", "TIN", "URN", "VAN", "WAX", "YAK",
                "ZIP", "ACT", "ADD", "AGE", "AIR", "ART", "BAD", "BIG", "BOX",
                "BUS", "CUT", "DAY", "DIE", "DRY", "EAR", "EAT", "END", "FAR",
                "FLY", "GET", "HOT", "INK", "JOB", "KIT", "LIE", "LOW", "MIX",
                "NEW", "OLD", "PET", "PUT", "RUN", "SAY", "SEE", "SHE", "TRY",
                "USE", "WIN", "YES", "ZOO", "ALE", "ASH", "AWE", "BAY", "BEE",
                "BIB", "BOO", "BOW", "BUD", "BUM", "COB", "COD", "CUE", "DAB",
                "DAD", "DAM", "DEN", "DIP", "DOT", "DUE", "DUG", "DYE", "EEL",
                "ELF", "EMU", "ERN", "EWE", "FAX", "FEE", "FEW", "FIG", "FIN",
                "FIR", "FLU", "FOE", "FOG", "FOR", "FOX", "FRY", "GAG", "GEL",
                "GEM", "GET", "GIG", "GIN", "GUM", "GUN", "GUT", "GYM", "HAD",
                "HAM", "HAS", "HEM", "HEN", "HER", "HIC", "HID", "HIM", "HIP",
                "HIS", "HIT", "HOB", "HOD", "HOE", "HOG", "HOP", "HOW", "HUB",
                "HUE", "HUG", "HUM", "HUT", "ICE", "INK", "INN", "ION", "ITS",
                "JAB", "JAG", "JAM", "JAR", "JAW", "JAY", "JET", "JIB", "JIG",
                "JOB", "JOG", "JOT", "JOY", "JUG", "JUT", "KEG", "KEN", "KEY",
                "KID", "KIN", "KIT", "LAB", "LAD", "LAG", "LAP", "LAW", "LAY",
                "LED", "LEG", "LET", "LID", "LIE", "LIP", "LIT", "LOB", "LOG",
                "LOP", "LOT", "LOW", "LUG", "LUX", "LYE", "MAD", "MAN", "MAP",
                "MAT", "MAY", "MED", "MEN", "MET", "MID", "MIX", "MOB", "MOM",
                "MOP", "MOW", "MUD", "MUG", "NAP", "NET", "NIP", "NOD", "NOT",
                "NOW", "NUN", "NUT", "OAK", "OAR", "OAT", "ODD", "OFF", "OIL",
                "OLD", "ONE", "ORB", "ORE", "OUR", "OUT", "OWE", "OWL", "OWN",
                "PAD", "PAL", "PAN", "PAR", "PAT", "PAW", "PAY", "PEG", "PEN",
                "PEP", "PER", "PET", "PEW", "PIC", "PIE", "PIG", "PIN", "PIP",
                "PIT", "PLY", "POD", "POI", "POP", "POT", "POW", "PRO", "PUB",
                "PUD", "PUG", "PUN", "PUP", "PUT", "RAG", "RAM", "RAN", "RAP",
                "RAT", "RAW", "RAY", "RED", "RIB", "RID", "RIG", "RIM", "RIP",
                "ROB", "ROD", "ROE", "ROT", "ROW", "RUB", "RUE", "RUG", "RUM",
                "RUN", "RUT", "RYE", "SAD", "SAG", "SAP", "SAT", "SAW", "SAY",
                "SEA", "SEC", "SEE", "SET", "SEW", "SHE", "SHY", "SIB", "SIC",
                "SIN", "SIP", "SIR", "SIS", "SIT", "SIX", "SKI", "SKY", "SLY",
                "SOB", "SOD", "SON", "SOP", "SOS", "SOT", "SOW", "SOY", "SPA",
                "SPY", "SUB", "SUE", "SUM", "SUN", "SUP", "TAB", "TAD", "TAG",
                "TAM", "TAN", "TAP", "TAR", "TAT", "TAU", "TAX", "TEA", "TED",
                "TEE", "TEN", "THE", "THY", "TIC", "TIE", "TIL", "TIN", "TIP",
                "TIT", "TOD", "TOE", "TOG", "TOM", "TON", "TOO", "TOP", "TOR",
                "TOT", "TOW", "TOY", "TRY", "TUB", "TUG", "TUI", "TUN", "TUT",
                "TWO", "URN", "USE", "VAN", "VAT", "VEX", "VIA", "VIE", "VOW",
                "WAD", "WAG", "WAN", "WAR", "WAS", "WAY", "WEB", "WED", "WEE",
                "WET", "WHO", "WHY", "WIG", "WIN", "WIT", "WOK", "WON", "WOO",
                "WOW", "WRY", "YAK", "YAM", "YAP", "YAW", "YAY", "YEN", "YES",
                "YET", "YEW", "YIN", "YIP", "YOD", "YOU", "YUK", "YUM", "ZAP",
                "ZED", "ZEE", "ZIG", "ZIP", "ZIT", "ZOO"
            };

            for (const string& word : common_words) {
                dictionary.insert(word);
            }
            cout << "Loaded " << dictionary.size() << " common words\n";
            return;
        }

        string word;
        int count = 0;
        while (file >> word) {
            // Convert to uppercase and ensure 3 letters
            if (word.length() == 3) {
                transform(word.begin(), word.end(), word.begin(), ::toupper);
                dictionary.insert(word);
                count++;
            }
        }
        file.close();
        cout << "Loaded " << count << " words from dictionary\n";
    }

    void check_for_words() {
        all_valid_words.clear();
        vector<string> words = get_all_words_on_board();

        for (string& word : words) {
            string upper_word = word;
            transform(upper_word.begin(), upper_word.end(), upper_word.begin(), ::toupper);

            // Check word and its reverse
            if (dictionary.find(upper_word) != dictionary.end()) {
                all_valid_words.push_back(upper_word);
            }
            else {
                // Check reverse
                string reversed = upper_word;
                reverse(reversed.begin(), reversed.end());
                if (dictionary.find(reversed) != dictionary.end()) {
                    all_valid_words.push_back(reversed);
                }
            }
        }
    }

    vector<string> get_all_words_on_board() const {
        vector<string> words;

        // Check rows
        for (int row = 0; row < 3; row++) {
            string word = "";
            for (int col = 0; col < 3; col++) {
                word += (board[row][col] == 0) ? '?' : board[row][col];
            }
            if (word.find('?') == string::npos) {
                words.push_back(word);
            }
        }

        // Check columns
        for (int col = 0; col < 3; col++) {
            string word = "";
            for (int row = 0; row < 3; row++) {
                word += (board[row][col] == 0) ? '?' : board[row][col];
            }
            if (word.find('?') == string::npos) {
                words.push_back(word);
            }
        }

        // Check main diagonal (top-left to bottom-right)
        string diag1 = "";
        for (int i = 0; i < 3; i++) {
            diag1 += (board[i][i] == 0) ? '?' : board[i][i];
        }
        if (diag1.find('?') == string::npos) {
            words.push_back(diag1);
        }

        // Check anti-diagonal (top-right to bottom-left)
        string diag2 = "";
        for (int i = 0; i < 3; i++) {
            diag2 += (board[i][2 - i] == 0) ? '?' : board[i][2 - i];
        }
        if (diag2.find('?') == string::npos) {
            words.push_back(diag2);
        }

        return words;
    }

    vector<pair<int, int>> get_word_positions(const string& word) const {
        vector<pair<int, int>> positions;

        // Check if word exists in rows
        for (int row = 0; row < 3; row++) {
            string row_word = "";
            for (int col = 0; col < 3; col++) {
                row_word += board[row][col];
            }
            if (row_word == word) {
                for (int col = 0; col < 3; col++) {
                    positions.push_back({ row, col });
                }
                return positions;
            }
        }

        // Check columns
        for (int col = 0; col < 3; col++) {
            string col_word = "";
            for (int row = 0; row < 3; row++) {
                col_word += board[row][col];
            }
            if (col_word == word) {
                for (int row = 0; row < 3; row++) {
                    positions.push_back({ row, col });
                }
                return positions;
            }
        }

        // Check main diagonal
        string diag1 = "";
        for (int i = 0; i < 3; i++) {
            diag1 += board[i][i];
        }
        if (diag1 == word) {
            for (int i = 0; i < 3; i++) {
                positions.push_back({ i, i });
            }
            return positions;
        }

        // Check anti-diagonal
        string diag2 = "";
        for (int i = 0; i < 3; i++) {
            diag2 += board[i][2 - i];
        }
        if (diag2 == word) {
            for (int i = 0; i < 3; i++) {
                positions.push_back({ i, 2 - i });
            }
            return positions;
        }

        return positions;
    }
};

#endif