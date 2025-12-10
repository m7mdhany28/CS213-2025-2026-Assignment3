#include <iostream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include "UltimateTicTacToe_UI.h"

using namespace std;

UltimateTicTacToe_UI::UltimateTicTacToe_UI()
    : UI<char>("Welcome to Ultimate Tic Tac Toe!", 3) {
    game_board = new UltimateTicTacToe_Board();
    srand((unsigned int)time(0)); // For random AI moves
}

UltimateTicTacToe_UI::~UltimateTicTacToe_UI() {
    delete game_board;
}

Move<char>* UltimateTicTacToe_UI::get_move(Player<char>* player) {
    if (player->get_type() == PlayerType::HUMAN) {
        // Display the board using our custom display method
        game_board->display_board();

        int main_row, main_col, small_row, small_col;
        bool valid_input = false;

        while (!valid_input) {
            cout << "\n" << player->get_name() << " (" << player->get_symbol() << "), enter your move:\n";

            pair<int, int> next_board = game_board->get_next_board_position();
            if (next_board.first != -1) {
                cout << "You must play in board (" << next_board.first << ", " << next_board.second << ")\n";
                cout << "Enter small board coordinates (row col) [0-2] [0-2]: ";
                cin >> small_row >> small_col;
                main_row = next_board.first;
                main_col = next_board.second;
            }
            else {
                cout << "You can play in any board.\n";
                cout << "Enter main board coordinates (row col) [0-2] [0-2]: ";
                cin >> main_row >> main_col;
                cout << "Enter small board coordinates (row col) [0-2] [0-2]: ";
                cin >> small_row >> small_col;
            }

            if (game_board->valid_move(main_row, main_col, small_row, small_col)) {
                valid_input = true;
            }
            else {
                cout << "Invalid move! Please try again.\n";
                show_available_moves(next_board);
            }
        }

        // Encode coordinates for the Move class
        pair<int, int> encoded = game_board->encode_coordinates(main_row, main_col, small_row, small_col);
        return new Move<char>(encoded.first, encoded.second, player->get_symbol());

    }
    else if (player->get_type() == PlayerType::COMPUTER) {
        cout << "\nComputer (" << player->get_symbol() << ") is thinking...\n";
        return get_AI_move(player);
    }

    return nullptr;
}

Move<char>* UltimateTicTacToe_UI::get_AI_move(Player<char>* player) {
    pair<int, int> next_board = game_board->get_next_board_position();
    vector<pair<int, int>> valid_moves;

    // Collect all valid moves
    if (next_board.first != -1) {
        int mr = next_board.first;
        int mc = next_board.second;
        for (int sr = 0; sr < 3; ++sr) {
            for (int sc = 0; sc < 3; ++sc) {
                if (game_board->valid_move(mr, mc, sr, sc)) {
                    valid_moves.push_back(make_pair(sr, sc));
                }
            }
        }
    }
    else {
        for (int mr = 0; mr < 3; ++mr) {
            for (int mc = 0; mc < 3; ++mc) {
                for (int sr = 0; sr < 3; ++sr) {
                    for (int sc = 0; sc < 3; ++sc) {
                        if (game_board->valid_move(mr, mc, sr, sc)) {
                            valid_moves.push_back(make_pair(mr * 10 + mc, sr * 10 + sc));
                        }
                    }
                }
            }
        }
    }

    if (!valid_moves.empty()) {
        // Pick a random valid move
        int idx = rand() % valid_moves.size();

        if (next_board.first != -1) {
            int mr = next_board.first;
            int mc = next_board.second;
            int sr = valid_moves[idx].first;
            int sc = valid_moves[idx].second;
            pair<int, int> encoded = game_board->encode_coordinates(mr, mc, sr, sc);
            return new Move<char>(encoded.first, encoded.second, player->get_symbol());
        }
        else {
            int encoded_val = valid_moves[idx].first;
            int mr = encoded_val / 10;
            int mc = encoded_val % 10;
            encoded_val = valid_moves[idx].second;
            int sr = encoded_val / 10;
            int sc = encoded_val % 10;
            pair<int, int> encoded = game_board->encode_coordinates(mr, mc, sr, sc);
            return new Move<char>(encoded.first, encoded.second, player->get_symbol());
        }
    }

    // Fallback: return a move to (0,0,0,0) if no valid move found
    pair<int, int> encoded = game_board->encode_coordinates(0, 0, 0, 0);
    return new Move<char>(encoded.first, encoded.second, player->get_symbol());
}

Player<char>* UltimateTicTacToe_UI::create_player(string& name, char symbol, PlayerType type) {
    cout << "Creating " << (type == PlayerType::HUMAN ? "Human" : "Computer")
        << " player: " << name << " (" << symbol << ")" << endl;
    return new Player<char>(name, symbol, type);
}

Player<char>** UltimateTicTacToe_UI::setup_players() {
    Player<char>** players = new Player<char>*[2];
    vector<string> type_options = { "Human", "Computer" };

    string nameX = get_player_name("Player X");
    PlayerType typeX = get_player_type_choice("Player X", type_options);
    players[0] = create_player(nameX, 'X', typeX);

    string nameO = get_player_name("Player O");
    PlayerType typeO = get_player_type_choice("Player O", type_options);
    players[1] = create_player(nameO, 'O', typeO);

    return players;
}

void UltimateTicTacToe_UI::display_game_state() {
    game_board->display_board();
}

void UltimateTicTacToe_UI::show_available_moves(pair<int, int> next_board) {
    cout << "\nAvailable moves:\n";
    int count = 0;

    if (next_board.first != -1) {
        int mr = next_board.first;
        int mc = next_board.second;
        for (int sr = 0; sr < 3; ++sr) {
            for (int sc = 0; sc < 3; ++sc) {
                if (game_board->valid_move(mr, mc, sr, sc)) {
                    cout << "  Board (" << mr << "," << mc << ") -> Small (" << sr << "," << sc << ")\n";
                    count++;
                }
            }
        }
    }
    else {
        for (int mr = 0; mr < 3; ++mr) {
            for (int mc = 0; mc < 3; ++mc) {
                for (int sr = 0; sr < 3; ++sr) {
                    for (int sc = 0; sc < 3; ++sc) {
                        if (game_board->valid_move(mr, mc, sr, sc)) {
                            cout << "  Board (" << mr << "," << mc << ") -> Small (" << sr << "," << sc << ")\n";
                            count++;
                            if (count >= 10) {
                                cout << "  ... and " << (81 - count) << " more moves\n";
                                return;
                            }
                        }
                    }
                }
            }
        }
    }

    if (count == 0) {
        cout << "  No available moves!\n";
    }
}

bool UltimateTicTacToe_UI::parse_input(const string& input, int& main_row, int& main_col, int& small_row, int& small_col) {
    stringstream ss(input);
    ss >> main_row >> main_col >> small_row >> small_col;
    return !ss.fail();
}