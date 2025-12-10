#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include "WordTicTacToe_Board.h"
#include "DictionaryManager.h"

using namespace std;

WordTicTacToe_Board::WordTicTacToe_Board() : Board(3, 3) {
    // Initialize board with dots
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            board[i][j] = '.';
        }
    }
    srand(static_cast<unsigned int>(time(0)));

    // Initialize dictionary manager
    dictManager = DictionaryManager::getInstance();
}

vector<string> WordTicTacToe_Board::getAllLines() const {
    vector<string> lines;

    // Rows
    for (int i = 0; i < 3; ++i) {
        string row;
        for (int j = 0; j < 3; ++j) {
            row += board[i][j];
        }
        lines.push_back(row);
    }

    // Columns
    for (int j = 0; j < 3; ++j) {
        string col;
        for (int i = 0; i < 3; ++i) {
            col += board[i][j];
        }
        lines.push_back(col);
    }

    // Diagonals
    string diag1, diag2;
    for (int i = 0; i < 3; ++i) {
        diag1 += board[i][i];
        diag2 += board[i][2 - i];
    }
    lines.push_back(diag1);
    lines.push_back(diag2);

    return lines;
}

bool WordTicTacToe_Board::isWordCompleteInLine(const vector<char>& line) const {
    // Check if all positions in line are filled with letters (no dots)
    for (char c : line) {
        if (c == '.') return false;
    }
    return true;
}

bool WordTicTacToe_Board::isValidWordFormation(const vector<char>& line) const {
    string word;
    for (char c : line) {
        if (c != '.') {
            word += c;
        }
    }

    if (word.empty()) return true; // Empty line is valid

    // Check if this partial/full word is valid
    return dictManager->isValidPartialWord(word);
}

bool WordTicTacToe_Board::isValidMove(int x, int y, char symbol) const {
    if (x < 0 || x >= 3 || y < 0 || y >= 3) return false;
    if (board[x][y] != '.') return false;

    // Test the move
    vector<vector<char>> testBoard = board;
    testBoard[x][y] = symbol;

    // Check all affected lines
    // Row
    vector<char> row = { testBoard[x][0], testBoard[x][1], testBoard[x][2] };
    if (!isValidWordFormation(row)) return false;

    // Column
    vector<char> col = { testBoard[0][y], testBoard[1][y], testBoard[2][y] };
    if (!isValidWordFormation(col)) return false;

    // Diagonal 1 (if on main diagonal)
    if (x == y) {
        vector<char> diag1 = { testBoard[0][0], testBoard[1][1], testBoard[2][2] };
        if (!isValidWordFormation(diag1)) return false;
    }

    // Diagonal 2 (if on anti-diagonal)
    if (x + y == 2) {
        vector<char> diag2 = { testBoard[0][2], testBoard[1][1], testBoard[2][0] };
        if (!isValidWordFormation(diag2)) return false;
    }

    return true;
}

bool WordTicTacToe_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char symbol = move->get_symbol();

    // Check if move is valid according to dictionary rules
    if (!isValidMove(x, y, symbol)) {
        cout << "Invalid move! This would not form a valid word.\n";
        return false;
    }

    // Make the move
    board[x][y] = symbol;
    n_moves++;

    // Check if any complete word is formed
    auto lines = getAllLines();
    for (const auto& line : lines) {
        if (line.find('.') == string::npos) { // Complete line
            if (dictManager->isValidWord(line)) {
                cout << "Word formed: " << line << "\n";
            }
        }
    }

    return true;
}

vector<pair<int, int>> WordTicTacToe_Board::getValidLetterPositions(char symbol) const {
    vector<pair<int, int>> validPositions;

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board[i][j] == '.' && isValidMove(i, j, symbol)) {
                validPositions.push_back({ i, j });
            }
        }
    }

    return validPositions;
}

vector<string> WordTicTacToe_Board::getAllValidWords() const {
    vector<string> validWords;
    auto lines = getAllLines();

    for (const auto& line : lines) {
        if (line.find('.') == string::npos && dictManager->isValidWord(line)) {
            validWords.push_back(line);
        }
    }

    return validWords;
}

bool WordTicTacToe_Board::is_win(Player<char>* player) {
    // Check if player has formed a valid 3-letter word
    auto validWords = getAllValidWords();

    // Player wins if they have more valid words than opponent
    char playerSymbol = player->get_symbol();
    int playerWordCount = 0;

    for (const auto& word : validWords) {
        // Count words that contain player's letters (assuming mixed words are possible)
        // For simplicity, check if word contains player's symbol
        if (word.find(playerSymbol) != string::npos) {
            playerWordCount++;
        }
    }

    return playerWordCount > 0;
}

bool WordTicTacToe_Board::is_lose(Player<char>* player) {
    char opponent_symbol = (player->get_symbol() == 'A') ? 'B' : 'A';

    // Check rows for opponent
    for (int i = 0; i < 3; ++i) {
        if (board[i][0] == opponent_symbol && board[i][1] == opponent_symbol && board[i][2] == opponent_symbol) {
            return true;
        }
    }

    // Check columns for opponent
    for (int j = 0; j < 3; ++j) {
        if (board[0][j] == opponent_symbol && board[1][j] == opponent_symbol && board[2][j] == opponent_symbol) {
            return true;
        }
    }

    // Check diagonals for opponent
    if (board[0][0] == opponent_symbol && board[1][1] == opponent_symbol && board[2][2] == opponent_symbol) {
        return true;
    }

    if (board[0][2] == opponent_symbol && board[1][1] == opponent_symbol && board[2][0] == opponent_symbol) {
        return true;
    }

    return false;
}

bool WordTicTacToe_Board::is_draw(Player<char>* player) {
    // Draw if board is full and no one won
    return (n_moves == 9) && !is_win(player) && !is_lose(player);
}

bool WordTicTacToe_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_lose(player) || is_draw(player);
}

vector<pair<int, int>> WordTicTacToe_Board::get_valid_moves() {
    vector<pair<int, int>> moves;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board[i][j] == '.') {
                moves.push_back({ i, j });
            }
        }
    }
    return moves;
}

bool WordTicTacToe_Board::make_ai_move(char symbol) {
    vector<pair<int, int>> valid_moves = get_valid_moves();

    if (valid_moves.empty()) {
        return false;
    }

    // Try to win
    for (const auto& move : valid_moves) {
        // Test if this move would win
        board[move.first][move.second] = symbol;
        bool would_win = false;

        // Check rows
        for (int i = 0; i < 3; ++i) {
            if (board[i][0] == symbol && board[i][1] == symbol && board[i][2] == symbol) {
                would_win = true;
                break;
            }
        }

        // Check columns
        for (int j = 0; j < 3; ++j) {
            if (board[0][j] == symbol && board[1][j] == symbol && board[2][j] == symbol) {
                would_win = true;
                break;
            }
        }

        // Check diagonals
        if (board[0][0] == symbol && board[1][1] == symbol && board[2][2] == symbol) {
            would_win = true;
        }

        if (board[0][2] == symbol && board[1][1] == symbol && board[2][0] == symbol) {
            would_win = true;
        }

        board[move.first][move.second] = '.'; // Undo test

        if (would_win) {
            board[move.first][move.second] = symbol;
            n_moves++;
            return true;
        }
    }

    // Try to block opponent
    char opponent_symbol = (symbol == 'A') ? 'B' : 'A';
    for (const auto& move : valid_moves) {
        // Test if this move would block opponent
        board[move.first][move.second] = opponent_symbol;
        bool would_block = false;

        // Check rows
        for (int i = 0; i < 3; ++i) {
            if (board[i][0] == opponent_symbol && board[i][1] == opponent_symbol && board[i][2] == opponent_symbol) {
                would_block = true;
                break;
            }
        }

        // Check columns
        for (int j = 0; j < 3; ++j) {
            if (board[0][j] == opponent_symbol && board[1][j] == opponent_symbol && board[2][j] == opponent_symbol) {
                would_block = true;
                break;
            }
        }

        // Check diagonals
        if (board[0][0] == opponent_symbol && board[1][1] == opponent_symbol && board[2][2] == opponent_symbol) {
            would_block = true;
        }

        if (board[0][2] == opponent_symbol && board[1][1] == opponent_symbol && board[2][0] == opponent_symbol) {
            would_block = true;
        }

        board[move.first][move.second] = '.'; // Undo test

        if (would_block) {
            board[move.first][move.second] = symbol;
            n_moves++;
            return true;
        }
    }

    // If center is available, take it
    if (board[1][1] == '.') {
        board[1][1] = symbol;
        n_moves++;
        return true;
    }

    // Take corners
    vector<pair<int, int>> corners = { {0,0}, {0,2}, {2,0}, {2,2} };
    for (const auto& corner : corners) {
        if (board[corner.first][corner.second] == '.') {
            board[corner.first][corner.second] = symbol;
            n_moves++;
            return true;
        }
    }

    // Take any available move
    int random_index = rand() % valid_moves.size();
    auto move = valid_moves[random_index];
    board[move.first][move.second] = symbol;
    n_moves++;
    return true;
}