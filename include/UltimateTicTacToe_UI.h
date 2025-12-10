#ifndef ULTIMATE_TIC_TAC_TOE_UI_H
#define ULTIMATE_TIC_TAC_TOE_UI_H

#include "BoardGame_Classes.h"
#include "UltimateTicTacToe_Board.h"
#include <sstream>

class UltimateTicTacToe_UI : public UI<char> {
private:
    UltimateTicTacToe_Board* game_board;

public:
    UltimateTicTacToe_UI();
    ~UltimateTicTacToe_UI();

    // Get move from player
    Move<char>* get_move(Player<char>* player) override;

    // Create a player
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;

    // Setup players
    Player<char>** setup_players() override;

    // Get AI move (basic implementation)
    Move<char>* get_AI_move(Player<char>* player);

    // Display the board with additional info
    void display_game_state();

    // Show available moves
    void show_available_moves(pair<int, int> next_board);

    // Parse input string to coordinates
    bool parse_input(const string& input, int& main_row, int& main_col, int& small_row, int& small_col);
};

#endif