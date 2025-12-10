#include <iostream>
#include <iomanip>
#include "UltimateTicTacToe_Board.h"

using namespace std;

UltimateTicTacToe_Board::UltimateTicTacToe_Board()
    : Board(9, 9),  // Overall board is 9x9 conceptually
    next_board_pos(-1, -1),
    current_player(X_SYMBOL) {

    // Initialize main board with blanks
    main_board = vector<vector<char>>(3, vector<char>(3, BLANK_SYMBOL));

    // Initialize all 9 small boards (3x3 each) as 4D vector
    small_boards.resize(3);
    for (int i = 0; i < 3; ++i) {
        small_boards[i].resize(3);
        for (int j = 0; j < 3; ++j) {
            small_boards[i][j].resize(3);
            for (int k = 0; k < 3; ++k) {
                small_boards[i][j][k].resize(3, BLANK_SYMBOL);
            }
        }
    }
}

bool UltimateTicTacToe_Board::decode_coordinates(int encoded_x, int encoded_y,
    int& main_row, int& main_col,
    int& small_row, int& small_col) {
    // Decode: encoded_x = main_row * 3 + small_row
    //         encoded_y = main_col * 3 + small_col
    if (encoded_x < 0 || encoded_x >= 9 || encoded_y < 0 || encoded_y >= 9)
        return false;

    main_row = encoded_x / 3;
    main_col = encoded_y / 3;
    small_row = encoded_x % 3;
    small_col = encoded_y % 3;

    return true;
}

pair<int, int> UltimateTicTacToe_Board::encode_coordinates(int main_row, int main_col,
    int small_row, int small_col) {
    int encoded_x = main_row * 3 + small_row;
    int encoded_y = main_col * 3 + small_col;
    return make_pair(encoded_x, encoded_y);
}

bool UltimateTicTacToe_Board::valid_move(int main_row, int main_col, int small_row, int small_col) {
    // Check bounds
    if (main_row < 0 || main_row >= 3 || main_col < 0 || main_col >= 3 ||
        small_row < 0 || small_row >= 3 || small_col < 0 || small_col >= 3) {
        return false;
    }

    // If next board is specified, check if move is in correct board
    if (next_board_pos.first != -1 && next_board_pos.second != -1) {
        if (next_board_pos.first != main_row || next_board_pos.second != main_col) {
            return false;
        }
    }

    // Check if main board cell is already won
    if (main_board[main_row][main_col] != BLANK_SYMBOL) {
        return false;
    }

    // Check if small board cell is empty
    return small_boards[main_row][main_col][small_row][small_col] == BLANK_SYMBOL;
}

bool UltimateTicTacToe_Board::update_board(Move<char>* move) {
    int encoded_x = move->get_x();
    int encoded_y = move->get_y();
    char symbol = move->get_symbol();

    int main_row, main_col, small_row, small_col;

    // Decode the coordinates
    if (!decode_coordinates(encoded_x, encoded_y, main_row, main_col, small_row, small_col)) {
        return false;
    }

    // Validate the move
    if (!valid_move(main_row, main_col, small_row, small_col)) {
        return false;
    }

    // Make the move
    small_boards[main_row][main_col][small_row][small_col] = symbol;

    // Update the overall board for display
    board[encoded_x][encoded_y] = symbol;
    n_moves++;

    // Check if small board is won after this move
    char winner = check_small_board_winner(main_row, main_col);
    if (winner != BLANK_SYMBOL) {
        main_board[main_row][main_col] = winner;
        // Update main board display
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                pair<int, int> encoded = encode_coordinates(main_row, main_col, i, j);
                board[encoded.first][encoded.second] = winner;
            }
        }
    }

    // Set next board position
    // If the small cell you play in corresponds to a board that's still playable,
    // next player must play in that board
    if (main_board[small_row][small_col] == BLANK_SYMBOL) {
        // Check if that board has any empty cells
        bool has_empty = false;
        for (int i = 0; i < 3 && !has_empty; ++i) {
            for (int j = 0; j < 3 && !has_empty; ++j) {
                if (small_boards[small_row][small_col][i][j] == BLANK_SYMBOL) {
                    has_empty = true;
                }
            }
        }
        if (has_empty) {
            next_board_pos = make_pair(small_row, small_col);
        }
        else {
            next_board_pos = make_pair(-1, -1); // Board is full, can choose any
        }
    }
    else {
        // That board is already won, next player can choose any board
        next_board_pos = make_pair(-1, -1);
    }

    // Switch player
    switch_player();

    return true;
}

bool UltimateTicTacToe_Board::is_win(Player<char>* player) {
    return check_main_board_winner(player->get_symbol());
}

bool UltimateTicTacToe_Board::is_lose(Player<char>* player) {
    char opponent = (player->get_symbol() == X_SYMBOL) ? O_SYMBOL : X_SYMBOL;
    return check_main_board_winner(opponent);
}

bool UltimateTicTacToe_Board::is_draw(Player<char>* player) {
    // Check if main board has a winner
    if (check_main_board_winner(X_SYMBOL) || check_main_board_winner(O_SYMBOL)) {
        return false;
    }

    // Check if all cells are filled
    if (n_moves >= 81) { // 9 boards * 9 cells each
        return true;
    }

    // Check if all small boards are completed (won or full)
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (main_board[i][j] == BLANK_SYMBOL) {
                // Check if small board still has empty cells
                for (int r = 0; r < 3; ++r) {
                    for (int c = 0; c < 3; ++c) {
                        if (small_boards[i][j][r][c] == BLANK_SYMBOL) {
                            return false;
                        }
                    }
                }
            }
        }
    }

    return true;
}

bool UltimateTicTacToe_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_lose(player) || is_draw(player);
}

void UltimateTicTacToe_Board::display_board() {
    cout << "\nUltimate Tic Tac Toe Board:\n";
    cout << "=============================\n\n";

    // Display all 9 small boards in a 3x3 grid
    for (int big_row = 0; big_row < 3; ++big_row) {
        // Display 3 rows of each small board row
        for (int small_row = 0; small_row < 3; ++small_row) {
            for (int big_col = 0; big_col < 3; ++big_col) {
                cout << " ";
                for (int small_col = 0; small_col < 3; ++small_col) {
                    char cell = small_boards[big_row][big_col][small_row][small_col];
                    cout << cell << " ";
                }
                if (big_col < 2) cout << "|";
            }
            cout << endl;
        }
        if (big_row < 2) {
            cout << "---------+---------+---------" << endl;
        }
    }

    cout << "\nMain Board (winners of small boards):\n";
    cout << "-----------------------------------\n";
    for (int i = 0; i < 3; ++i) {
        cout << " ";
        for (int j = 0; j < 3; ++j) {
            cout << main_board[i][j];
            if (j < 2) cout << " | ";
        }
        cout << endl;
        if (i < 2) cout << "---+---+---" << endl;
    }

    cout << "\nCurrent player: " << current_player << endl;
    if (next_board_pos.first != -1) {
        cout << "Must play in board: (" << next_board_pos.first << ", " << next_board_pos.second << ")\n";
    }
    else {
        cout << "Can play in any available board\n";
    }
    cout << endl;
}

char UltimateTicTacToe_Board::check_small_board_winner(int main_row, int main_col) {
    // Check rows
    for (int i = 0; i < 3; ++i) {
        if (small_boards[main_row][main_col][i][0] != BLANK_SYMBOL &&
            small_boards[main_row][main_col][i][0] == small_boards[main_row][main_col][i][1] &&
            small_boards[main_row][main_col][i][1] == small_boards[main_row][main_col][i][2]) {
            return small_boards[main_row][main_col][i][0];
        }
    }

    // Check columns
    for (int j = 0; j < 3; ++j) {
        if (small_boards[main_row][main_col][0][j] != BLANK_SYMBOL &&
            small_boards[main_row][main_col][0][j] == small_boards[main_row][main_col][1][j] &&
            small_boards[main_row][main_col][1][j] == small_boards[main_row][main_col][2][j]) {
            return small_boards[main_row][main_col][0][j];
        }
    }

    // Check diagonals
    if (small_boards[main_row][main_col][0][0] != BLANK_SYMBOL &&
        small_boards[main_row][main_col][0][0] == small_boards[main_row][main_col][1][1] &&
        small_boards[main_row][main_col][1][1] == small_boards[main_row][main_col][2][2]) {
        return small_boards[main_row][main_col][0][0];
    }

    if (small_boards[main_row][main_col][0][2] != BLANK_SYMBOL &&
        small_boards[main_row][main_col][0][2] == small_boards[main_row][main_col][1][1] &&
        small_boards[main_row][main_col][1][1] == small_boards[main_row][main_col][2][0]) {
        return small_boards[main_row][main_col][0][2];
    }

    // Check if board is full (draw)
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (small_boards[main_row][main_col][i][j] == BLANK_SYMBOL) {
                return BLANK_SYMBOL; // Still playable
            }
        }
    }

    return 'D'; // D for draw (full but no winner)
}

bool UltimateTicTacToe_Board::check_main_board_winner(char symbol) {
    // Check rows
    for (int i = 0; i < 3; ++i) {
        if (main_board[i][0] == symbol &&
            main_board[i][1] == symbol &&
            main_board[i][2] == symbol) {
            return true;
        }
    }

    // Check columns
    for (int j = 0; j < 3; ++j) {
        if (main_board[0][j] == symbol &&
            main_board[1][j] == symbol &&
            main_board[2][j] == symbol) {
            return true;
        }
    }

    // Check diagonals
    if (main_board[0][0] == symbol &&
        main_board[1][1] == symbol &&
        main_board[2][2] == symbol) {
        return true;
    }

    if (main_board[0][2] == symbol &&
        main_board[1][1] == symbol &&
        main_board[2][0] == symbol) {
        return true;
    }

    return false;
}

char UltimateTicTacToe_Board::get_small_cell(int main_row, int main_col, int small_row, int small_col) {
    return small_boards[main_row][main_col][small_row][small_col];
}

void UltimateTicTacToe_Board::set_small_cell(int main_row, int main_col, int small_row, int small_col, char symbol) {
    small_boards[main_row][main_col][small_row][small_col] = symbol;
}

void UltimateTicTacToe_Board::switch_player() {
    current_player = (current_player == X_SYMBOL) ? O_SYMBOL : X_SYMBOL;
}