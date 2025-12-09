/**
 * @file Diamond_Classes.cpp
 * @brief Implementation of Diamond Tic-Tac-Toe classes.
 */

#include <iostream>
#include <iomanip>
#include <cctype>
#include <algorithm>
#include "Diamond_Classes.h"

using namespace std;

//--------------------------------------- Diamond_Board Implementation----------------------------------------------------

Diamond_Board::Diamond_Board() : Board(7, 7) {
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;
    
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 7; j++) {
            if (!is_valid_position(i, j)) {
                board[i][j] = ' ';
            }
        }
    }
}

bool Diamond_Board::is_valid_position(int x, int y) const {    
    if (x < 0 || x >= 7 || y < 0 || y >= 7)
        return false;
    
    if (x == 0 || x == 6) return y == 3;
    if (x == 1 || x == 5) return (y >= 2 && y <= 4);
    if (x == 2 || x == 4) return (y >= 1 && y <= 5);
    if (x == 3) return true; // all columns valid (middle row)
    
    return false;
}

bool is_valid_position(int x, int y){    
    if (x < 0 || x >= 7 || y < 0 || y >= 7)
        return false;
    
    if (x == 0 || x == 6) return y == 3;
    if (x == 1 || x == 5) return (y >= 2 && y <= 4);
    if (x == 2 || x == 4) return (y >= 1 && y <= 5);
    if (x == 3) return true; // all columns valid (middle row)
    
    return false;
}


bool Diamond_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char mark = move->get_symbol();

    if (is_valid_position(x, y) && board[x][y] == blank_symbol) {
        n_moves++;
        board[x][y] = toupper(mark);
        return true;
    }
    return false;
}

bool Diamond_Board::check_line_length(Player<char>* player, int length, 
                                       vector<pair<string, vector<pair<int, int>>>>& found_lines) {
    const char sym = player->get_symbol();
    bool found = false;

    for (int i = 0; i < 7; i++) {
        vector<pair<int, int>> line;
        for (int j = 0; j < 7; j++) {
            if (is_valid_position(i, j) && board[i][j] == sym) {
                line.push_back({i, j});
            } else if (is_valid_position(i, j) && board[i][j] != sym) {
                if (line.size() == length) {
                    found_lines.push_back({"horizontal", line});
                    found = true;
                }
                line.clear();
            }
        }
        if (line.size() == length) {
            found_lines.push_back({"horizontal", line});
            found = true;
        }
    }

    for (int j = 0; j < 7; j++) {
        vector<pair<int, int>> line;
        for (int i = 0; i < 7; i++) {
            if (is_valid_position(i, j) && board[i][j] == sym) {
                line.push_back({i, j});
            } else if (is_valid_position(i, j) && board[i][j] != sym) {
                if (line.size() == length) {
                    found_lines.push_back({"vertical", line});
                    found = true;
                }
                line.clear();
            }
        }
        if (line.size() == length) {
            found_lines.push_back({"vertical", line});
            found = true;
        }
    }

    for (int start = -6; start <= 6; start++) {
        vector<pair<int, int>> line;
        for (int i = 0; i < 7; i++) {
            int j = i - start;
            if (j >= 0 && j < 7 && is_valid_position(i, j) && board[i][j] == sym) {
                line.push_back({i, j});
            } else if (j >= 0 && j < 7 && is_valid_position(i, j) && board[i][j] != sym) {
                if (line.size() == length) {
                    found_lines.push_back({"diagonal_tlbr", line});
                    found = true;
                }
                line.clear();
            }
        }
        if (line.size() == length) {
            found_lines.push_back({"diagonal_tlbr", line});
            found = true;
        }
    }

    for (int start = 0; start <= 12; start++) {
        vector<pair<int, int>> line;
        for (int i = 0; i < 7; i++) {
            int j = start - i;
            if (j >= 0 && j < 7 && is_valid_position(i, j) && board[i][j] == sym) {
                line.push_back({i, j});
            } else if (j >= 0 && j < 7 && is_valid_position(i, j) && board[i][j] != sym) {
                if (line.size() == length) {
                    found_lines.push_back({"diagonal_trbl", line});
                    found = true;
                }
                line.clear();
            }
        }
        if (line.size() == length) {
            found_lines.push_back({"diagonal_trbl", line});
            found = true;
        }
    }

    return found;
}

bool Diamond_Board::is_win(Player<char>* player) {
    vector<pair<string, vector<pair<int, int>>>> lines_of_3;
    vector<pair<string, vector<pair<int, int>>>> lines_of_4;

    bool has_3 = check_line_length(player, 3, lines_of_3);
    bool has_4 = check_line_length(player, 4, lines_of_4);

    if (!has_3 || !has_4)
        return false;

    for (auto& line3 : lines_of_3) {
        for (auto& line4 : lines_of_4) {
            if (line3.first != line4.first) {
                return true;
            }
        }
    }

    return false;
}

bool Diamond_Board::is_draw(Player<char>* player) {
    int filled_cells = 0;
    
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 7; j++) {
            if (is_valid_position(i, j) && board[i][j] != blank_symbol) {
                filled_cells++;
            }
        }
    }
    
    return (filled_cells == 25 && !is_win(player));
}

bool Diamond_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

//--------------------------------------- Diamond_UI Implementation-----------------------------------------

Diamond_UI::Diamond_UI() : UI<char>("Welcome to FCAI Diamond Tic-Tac-Toe Game", 3) {}

Player<char>* Diamond_UI::create_player(string& name, char symbol, PlayerType type) {
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
         << " player: " << name << " (" << symbol << ")\n";
    return new Player<char>(name, symbol, type);
}

Move<char>* Diamond_UI::get_move(Player<char>* player) {
    int x, y;
    
    if (player->get_type() == PlayerType::HUMAN) {
        cout << "\n" << player->get_name() << " (" << player->get_symbol() 
             << "), enter your move (row col): ";
        cin >> x >> y;
    }
    else if (player->get_type() == PlayerType::COMPUTER) {
        Board<char>* board_ptr = player->get_board_ptr();
        do {
            x = rand() % 7;
            y = rand() % 7;
        } while (!is_valid_position(x, y) || board_ptr->get_cell(x, y) != '.');
        
        cout << "Computer plays: " << x << " " << y << endl;
    }
    
    return new Move<char>(x, y, player->get_symbol());
}

void Diamond_UI::display_board_matrix(const vector<vector<char>>& matrix) const {
    if (matrix.empty() || matrix[0].empty()) return;

    cout << "\n";
    cout << "         Diamond Tic-Tac-Toe Board\n";
    cout << "         ==========================\n\n";

    // Row 0: 1 cell (top point of diamond)
    cout << "              " << matrix[0][3] << "\n\n";

    // Row 1: 3 cells
    cout << "          " << matrix[1][2] << "   " << matrix[1][3] << "   " << matrix[1][4] << "\n\n";

    // Row 2: 5 cells
    cout << "      " << matrix[2][1] << "   " << matrix[2][2] << "   " << matrix[2][3] 
         << "   " << matrix[2][4] << "   " << matrix[2][5] << "\n\n";

    // Row 3: 7 cells (widest part - middle)
    cout << "  " << matrix[3][0] << "   " << matrix[3][1] << "   " << matrix[3][2] << "   " << matrix[3][3]
         << "   " << matrix[3][4] << "   " << matrix[3][5] << "   " << matrix[3][6] << "\n\n";

    // Row 4: 5 cells
    cout << "      " << matrix[4][1] << "   " << matrix[4][2] << "   " << matrix[4][3] 
         << "   " << matrix[4][4] << "   " << matrix[4][5] << "\n\n";

    // Row 5: 3 cells
    cout << "          " << matrix[5][2] << "   " << matrix[5][3] << "   " << matrix[5][4] << "\n\n";

    // Row 6: 1 cell (bottom point of diamond)
    cout << "              " << matrix[6][3] << "\n\n";

    cout << "  Position format: row(0-6) col(0-6)\n";
    cout << "  Valid diamond positions:\n";
    cout << "    Row 0: (0,3)\n";
    cout << "    Row 1: (1,2) (1,3) (1,4)\n";
    cout << "    Row 2: (2,1) (2,2) (2,3) (2,4) (2,5)\n";
    cout << "    Row 3: (3,0) (3,1) (3,2) (3,3) (3,4) (3,5) (3,6)\n";
    cout << "    Row 4: (4,1) (4,2) (4,3) (4,4) (4,5)\n";
    cout << "    Row 5: (5,2) (5,3) (5,4)\n";
    cout << "    Row 6: (6,3)\n\n";
}
