/**
 * @file Diamond_Classes.h
 * @brief Defines the Diamond Tic-Tac-Toe specific classes that extend the generic board game framework.
 *
 * This file provides:
 * - `Diamond_Board`: A specialized board class for the Diamond Tic-Tac-Toe game.
 * - `Diamond_UI`: A user interface class tailored to Diamond Tic-Tac-Toe game setup and player interaction.
 */

#ifndef DIAMOND_CLASSES_H
#define DIAMOND_CLASSES_H

#include "BoardGame_Classes.h"
#include <algorithm>
using namespace std;

/**
 * @class Diamond_Board
 * @brief Represents the Diamond Tic-Tac-Toe game board.
 *
 * This class inherits from the generic `Board<char>` class and implements
 * the specific logic required for the Diamond Tic-Tac-Toe game, including
 * move updates, win detection (two simultaneous lines), and display functions.
 * The board is a 7x7 grid arranged in a diamond shape.
 *
 * @see Board
 */
class Diamond_Board : public Board<char> {
private:
    char blank_symbol = '.'; ///< Character used to represent an empty cell on the board.

    /**
     * @brief Checks if a position is valid within the diamond shape.
     * @param x Row index.
     * @param y Column index.
     * @return true if the position is within the diamond, false otherwise.
     */
    bool is_valid_position(int x, int y) const;

    /**
     * @brief Checks if a player has a line of a specific length.
     * @param player Pointer to the player being checked.
     * @param length The length of the line to check for (3 or 4).
     * @param found_lines Vector to store the found lines (direction and positions).
     * @return true if at least one line of the specified length is found.
     */
    bool check_line_length(Player<char>* player, int length, vector<pair<string, vector<pair<int, int>>>>& found_lines);

public:
    /**
     * @brief Default constructor that initializes a 7x7 Diamond board.
     */
    Diamond_Board();

    /**
     * @brief Updates the board with a player's move.
     * @param move Pointer to a Move<char> object containing move coordinates and symbol.
     * @return true if the move is valid and successfully applied, false otherwise.
     */
    bool update_board(Move<char>* move);

    /**
     * @brief Checks if the given player has won the game.
     * A player wins by simultaneously completing a line of 3 and a line of 4 in different directions.
     * @param player Pointer to the player being checked.
     * @return true if the player has won, false otherwise.
     */
    bool is_win(Player<char>* player);

    /**
     * @brief Checks if the given player has lost the game.
     * @param player Pointer to the player being checked.
     * @return Always returns false (not used in Diamond Tic-Tac-Toe logic).
     */
    bool is_lose(Player<char>*) { return false; }

    /**
     * @brief Checks if the game has ended in a draw.
     * @param player Pointer to the player being checked.
     * @return true if all valid cells are filled and no player has won, false otherwise.
     */
    bool is_draw(Player<char>* player);

    /**
     * @brief Determines if the game is over (win or draw).
     * @param player Pointer to the player to evaluate.
     * @return true if the game has ended, false otherwise.
     */
    bool game_is_over(Player<char>* player);

};

/**
 * @class Diamond_UI
 * @brief User Interface class for the Diamond Tic-Tac-Toe game.
 *
 * Inherits from the generic `UI<char>` base class and provides
 * Diamond Tic-Tac-Toe specific functionality for player setup and move input.
 *
 * @see UI
 */
class Diamond_UI : public UI<char> {
public:
    /**
     * @brief Constructs a Diamond_UI object.
     *
     * Initializes the base `UI<char>` class with the welcome message.
     */
    Diamond_UI();

    /**
     * @brief Destructor for Diamond_UI.
     */
    ~Diamond_UI() {}

    /**
     * @brief Creates a player of the specified type.
     * @param name Name of the player.
     * @param symbol Character symbol ('X' or 'O') assigned to the player.
     * @param type The type of the player (Human or Computer).
     * @return Pointer to the newly created Player<char> instance.
     */
    Player<char>* create_player(string& name, char symbol, PlayerType type);

    /**
     * @brief Retrieves the next move from a player.
     * @param player Pointer to the player whose move is being requested.
     * @return A pointer to a new `Move<char>` object representing the player's action.
     */
    Move<char>* get_move(Player<char>* player);

    /**
     * @brief Override to display the board in diamond shape format.

    */
    void display_board_matrix(const vector<vector<char>>& matrix) const;
};

#endif // DIAMOND_CLASSES_H
