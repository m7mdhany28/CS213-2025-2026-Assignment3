#ifndef ULTIMATE_TIC_TAC_TOE_BOARD_H
#define ULTIMATE_TIC_TAC_TOE_BOARD_H

#include "BoardGame_Classes.h"
#include <vector>

using namespace std;

class UltimateTicTacToe_Board : public Board<char> {
private:
    // 3x3 grid of small boards
    vector<vector<vector<vector<char>>>> small_boards;

    // Main board (3x3) showing winners of small boards
    vector<vector<char>> main_board;

    // Current small board that must be played in (or -1,-1 for any board)
    pair<int, int> next_board_pos;

    // Track whose turn it is
    char current_player;

    // Symbol constants
    const char BLANK_SYMBOL = '.';
    const char X_SYMBOL = 'X';
    const char O_SYMBOL = 'O';

public:
    UltimateTicTacToe_Board();

    // Check if a move is valid
    bool valid_move(int main_row, int main_col, int small_row, int small_col);

    // Update the board with a move
    bool update_board(Move<char>* move) override;

    // Check if player has won
    bool is_win(Player<char>* player) override;

    // Check if player has lost
    bool is_lose(Player<char>* player) override;

    // Check for draw
    bool is_draw(Player<char>* player) override;

    // Check if game is over
    bool game_is_over(Player<char>* player) override;

    // Display the board (not virtual in base class)
    void display_board();

    // Get current board position that must be played
    pair<int, int> get_next_board_position() const { return next_board_pos; }

    // Get current player
    char get_current_player() const { return current_player; }

    // Switch to next player
    void switch_player();

    // Check if a small board is won
    char check_small_board_winner(int main_row, int main_col);

    // Check if main board is won
    bool check_main_board_winner(char symbol);

    // Get cell value from small board
    char get_small_cell(int main_row, int main_col, int small_row, int small_col);

    // Set cell value in small board
    void set_small_cell(int main_row, int main_col, int small_row, int small_col, char symbol);

    // Helper to convert single coordinates to 4 coordinates
    bool decode_coordinates(int encoded_x, int encoded_y, int& main_row, int& main_col, int& small_row, int& small_col);

    // Encode 4 coordinates into 2 for Move class
    pair<int, int> encode_coordinates(int main_row, int main_col, int small_row, int small_col);
};

#endif