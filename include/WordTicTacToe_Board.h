// WordTicTacToe_Board.h (add these methods)
#ifndef WORD_TIC_TAC_TOE_BOARD_H
#define WORD_TIC_TAC_TOE_BOARD_H

#include "BoardGame_Classes.h"
#include <string>
#include <vector>

using namespace std;

class WordTicTacToe_Board : public Board<char> {
private:
    // Add a dictionary manager pointer
    class DictionaryManager* dictManager;

    // Helper methods for word checking
    vector<string> getAllLines() const;
    bool isWordCompleteInLine(const vector<char>& line) const;
    bool isValidWordFormation(const vector<char>& line) const;

public:
    WordTicTacToe_Board();

    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* player) override;
    bool is_lose(Player<char>* player) override;
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;

    // Word-specific methods
    bool isValidMove(int x, int y, char symbol) const;
    vector<pair<int, int>> getValidLetterPositions(char symbol) const;
    vector<string> getAllValidWords() const;

    // Helper methods for AI
    vector<pair<int, int>> get_valid_moves();
    bool make_ai_move(char symbol);
};

#endif