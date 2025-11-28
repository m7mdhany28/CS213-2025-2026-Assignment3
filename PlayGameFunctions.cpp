#include "PlayGameFunctions.h"
#include "BoardGame_Classes.h"
#include "Misere_Board.h"
#include "FiveByFive_Board.h"
#include "WordTicTacToe_Board.h"
#include "Infinity_Board.h"
#include "FourByFour_Moving_Board.h"
#include "pyramid_tic_tac_toe.h"
#include "Simple_UI.h"
#include <iostream>
#include <string>
#include <limits>

using namespace std;

// Player setup helper functions
string getPlayerIdentity(const string& playerLabel) {
    string name;
    cout << "What name should we use for " << playerLabel << "? ";
    getline(cin, name);
    name.erase(0, name.find_first_not_of(" \t\n\r\f\v"));
    name.erase(name.find_last_not_of(" \t\n\r\f\v") + 1);
    if (name.empty()) {
        name = playerLabel;
        cout << "We'll call you: " << name << "\n";
    }
    return name;
}

PlayerType choosePlayerPersonality(const string& playerLabel) {
    int choice;
    cout << "\nWhat kind of player should " << playerLabel << " be?\n";
    cout << "1. Human Player (You make the decisions)\n";
    cout << "2. Random Computer (Full of surprises)\n";
    cout << "3. Smart Computer (Thinks strategically)\n";
    cout << "Enter your selection (1, 2, or 3): ";

    while (!(cin >> choice) || choice < 1 || choice > 3) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Let's choose: 1 for Human, 2 for Random Computer, or 3 for Smart Computer: ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    switch (choice) {
    case 1: return PlayerType::HUMAN;
    case 2: return PlayerType::RANDOM;
    case 3: return PlayerType::AI;
    default: return PlayerType::HUMAN;
    }
}

void PlayGameFunctions::playMisereGame() {
    cout << "\n" << string(60, '=') << "\n";
    cout << "          MISERE TIC TAC TOE - REVERSE PSYCHOLOGY CHALLENGE\n";
    cout << string(60, '=') << "\n";

    Board<char>* game_board = new Misere_Board();
    UI<char>* game_interface = new Simple_UI(game_board, "misere");

    cout << "\nGetting to know our first player (using X marks):\n";
    string player1_name = getPlayerIdentity("First Player");
    PlayerType player1_type = choosePlayerPersonality("First Player");

    cout << "\nNow let's meet our second player (using O marks):\n";
    string player2_name = getPlayerIdentity("Second Player");
    PlayerType player2_type = choosePlayerPersonality("Second Player");

    Player<char>* players[2] = {
        new Player<char>(player1_name, 'X', player1_type),
        new Player<char>(player2_name, 'O', player2_type)
    };

    cout << "\n" << string(50, '~') << "\n";
    cout << "GAME STARTING: " << player1_name << " (X) vs " << player2_name << " (O)\n";
    cout << string(50, '~') << "\n";

    game_interface->display_board_matrix(game_board->get_board_matrix());

    while (true) {
        for (int i = 0; i < 2; i++) {
            Player<char>* current_player = players[i];
            Player<char>* other_player = players[1 - i];

            Move<char>* next_move = game_interface->get_move(current_player);

            while (!game_board->update_board(next_move)) {
                cout << "That move doesn't work. The spot might be taken or off the board.\n";
                delete next_move;
                next_move = game_interface->get_move(current_player);
            }

            game_interface->display_board_matrix(game_board->get_board_matrix());

            if (game_board->is_win(current_player)) {
                cout << string(60, '=') << "\n";
                cout << "GAME OVER - THE PLOT TWIST!\n";
                cout << string(60, '=') << "\n";
                game_interface->display_message(current_player->get_name() + " made three in a row... and loses!");
                game_interface->display_message(other_player->get_name() + " wins by clever strategy!");
                cout << string(60, '=') << "\n";
                delete next_move;
                goto game_finished;
            }

            if (game_board->is_draw(current_player)) {
                cout << string(60, '=') << "\n";
                cout << "GAME OVER - SMART PLAYERS!\n";
                cout << string(60, '=') << "\n";
                game_interface->display_message("The game ends in a draw!");
                game_interface->display_message("Both players successfully avoided making three in a row!");
                cout << string(60, '=') << "\n";
                delete next_move;
                goto game_finished;
            }
            delete next_move;
        }
    }

game_finished:
    delete game_board;
    delete players[0];
    delete players[1];
    delete game_interface;
}

void PlayGameFunctions::play5x5Game() {
    cout << "\n" << string(60, '=') << "\n";
    cout << "          5x5 TIC TAC TOE - EXPANDED CHALLENGE\n";
    cout << string(60, '=') << "\n";

    FiveByFive_Board* board = new FiveByFive_Board();
    Simple_UI* ui = new Simple_UI(board, "5x5");

    cout << "\nConfiguring first player (will use X symbol):\n";
    string name1 = getPlayerIdentity("First Player");
    PlayerType type1 = choosePlayerPersonality("First Player");

    cout << "\nConfiguring second player (will use O symbol):\n";
    string name2 = getPlayerIdentity("Second Player");
    PlayerType type2 = choosePlayerPersonality("Second Player");

    Player<char>* players[2] = {
        new Player<char>(name1, 'X', type1),
        new Player<char>(name2, 'O', type2)
    };

    cout << "\n" << string(50, '-') << "\n";
    cout << "GAME STARTING: " << name1 << " (X) vs " << name2 << " (O)\n";
    cout << string(50, '-') << "\n";

    board->display_board();

    while (true) {
        for (int i = 0; i < 2; i++) {
            Player<char>* current = players[i];
            Move<char>* move = ui->get_move(current);

            while (!board->update_board(move)) {
                cout << "That move cannot be made. The position may be already occupied or invalid.\n";
                delete move;
                move = ui->get_move(current);
            }

            board->display_board();

            if (board->game_is_over(current)) {
                int player1_lines = board->count_player_sequences('X');
                int player2_lines = board->count_player_sequences('O');

                cout << "\n" << string(50, '=') << "\n";
                cout << "GAME OVER - FINAL SCORE\n";
                cout << string(50, '=') << "\n";
                cout << players[0]->get_name() << " (X): " << player1_lines << " three-in-a-row sequences\n";
                cout << players[1]->get_name() << " (O): " << player2_lines << " three-in-a-row sequences\n";

                if (player1_lines > player2_lines) {
                    cout << "WINNER: " << players[0]->get_name() << "!\n";
                }
                else if (player2_lines > player1_lines) {
                    cout << "WINNER: " << players[1]->get_name() << "!\n";
                }
                else {
                    cout << "The game is a TIE! Well played both!\n";
                }
                cout << string(50, '=') << "\n";

                delete move;
                delete board;
                delete players[0];
                delete players[1];
                delete ui;
                return;
            }
            delete move;
        }
    }
}

void PlayGameFunctions::playWordGame() {
    cout << "\n" << string(60, '=') << "\n";
    cout << "          WORD TIC TAC TOE - VOCABULARY CHALLENGE\n";
    cout << string(60, '=') << "\n";

    WordTicTacToe_Board* board = new WordTicTacToe_Board();
    Simple_UI* ui = new Simple_UI(board, "word");

    cout << "\nConfiguring first player:\n";
    string name1 = getPlayerIdentity("First Player");
    PlayerType type1 = choosePlayerPersonality("First Player");

    cout << "\nConfiguring second player:\n";
    string name2 = getPlayerIdentity("Second Player");
    PlayerType type2 = choosePlayerPersonality("Second Player");

    Player<char>* players[2] = {
        new Player<char>(name1, '?', type1),
        new Player<char>(name2, '?', type2)
    };

    cout << "\n" << string(50, '-') << "\n";
    cout << "GAME STARTING: " << name1 << " vs " << name2 << "\n";
    cout << string(50, '-') << "\n";

    ui->display_board_matrix(board->get_board_matrix());

    while (true) {
        for (int i = 0; i < 2; i++) {
            Player<char>* current = players[i];
            Player<char>* opponent = players[1 - i];
            Move<char>* move = ui->get_move(current);

            while (!board->update_board(move)) {
                cout << "That move cannot be made. The position may be already occupied or invalid.\n";
                delete move;
                move = ui->get_move(current);
            }

            ui->display_board_matrix(board->get_board_matrix());

            if (board->is_win(current)) {
                string winning_word = "WIN";
                cout << "\n" << string(50, '=') << "\n";
                cout << "GAME OVER - WORD FOUND!\n";
                cout << string(50, '=') << "\n";
                cout << "Winning word: " << winning_word << "\n";
                cout << "WINNER: " << current->get_name() << "!\n";
                cout << "Congratulations for forming a valid word!\n";
                cout << string(50, '=') << "\n";
                delete move;
                goto game_over;
            }

            if (board->is_draw(current)) {
                cout << string(50, '=') << "\n";
                cout << "GAME OVER - FINAL RESULT\n";
                cout << string(50, '=') << "\n";
                cout << "The game ends in a draw!\n";
                cout << "No valid three-letter words were formed on the board.\n";
                cout << string(50, '=') << "\n";
                delete move;
                goto game_over;
            }
            delete move;
        }
    }

game_over:
    delete board;
    delete players[0];
    delete players[1];
    delete ui;
}
void PlayGameFunctions::playInfinityGame() {
    cout << "\n" << string(60, '=') << "\n";
    cout << "       INFINITY TIC-TAC-TOE - TEMPORAL CHALLENGE\n";
    cout << string(60, '=') << "\n";

    Infinity_Board* board = new Infinity_Board();
    Simple_UI* ui = new Simple_UI(board, "infinity");

    cout << "\nConfiguring first player (will use X symbol):\n";
    string name1 = getPlayerIdentity("First Player");
    PlayerType type1 = choosePlayerPersonality("First Player");

    cout << "\nConfiguring second player (will use O symbol):\n";
    string name2 = getPlayerIdentity("Second Player");
    PlayerType type2 = choosePlayerPersonality("Second Player");

    Player<char>* players[2] = {
        new Player<char>(name1, 'X', type1),
        new Player<char>(name2, 'O', type2)
    };

    cout << "\n" << string(50, '-') << "\n";
    cout << "GAME STARTING: " << name1 << " (X) vs " << name2 << " (O)\n";
    cout << string(50, '-') << "\n";

    board->display_board();

    while (true) {
        for (int i = 0; i < 2; i++) {
            Player<char>* current = players[i];
            Player<char>* opponent = players[1 - i];

            int countdown = board->get_removal_countdown();
            cout << "\nMoves until next removal: " << countdown << "\n";

            Move<char>* move = ui->get_move(current);

            while (!board->update_board(move)) {
                cout << "That move cannot be made. The position may be already occupied or invalid.\n";
                delete move;
                move = ui->get_move(current);
            }

            board->display_board();

            if (board->is_win(current)) {
                cout << string(60, '=') << "\n";
                cout << "GAME OVER - TEMPORAL VICTORY!\n";
                cout << string(60, '=') << "\n";
                cout << current->get_name() << " achieved three in a row before time ran out!\n";
                cout << string(60, '=') << "\n";
                delete move;
                delete board;
                delete players[0];
                delete players[1];
                delete ui;
                return;
            }
            delete move;
        }
    }
}

void PlayGameFunctions::playMovingGame() {
    cout << "\n" << string(60, '=') << "\n";
    cout << "      4x4 MOVING TIC-TAC-TOE - STRATEGIC MOVEMENT\n";
    cout << string(60, '=') << "\n";

    FourByFour_Moving_Board* board = new FourByFour_Moving_Board();
    Simple_UI* ui = new Simple_UI(board, "4x4");

    cout << "\nConfiguring first player (will use X symbol):\n";
    string name1 = getPlayerIdentity("First Player");
    PlayerType type1 = choosePlayerPersonality("First Player");

    cout << "\nConfiguring second player (will use O symbol):\n";
    string name2 = getPlayerIdentity("Second Player");
    PlayerType type2 = choosePlayerPersonality("Second Player");

    Player<char>* players[2] = {
        new Player<char>(name1, 'X', type1),
        new Player<char>(name2, 'O', type2)
    };

    cout << "\n" << string(50, '-') << "\n";
    cout << "GAME STARTING: " << name1 << " (X) vs " << name2 << " (O)\n";
    cout << string(50, '-') << "\n";

    board->display_board();

    while (true) {
        for (int i = 0; i < 2; i++) {
            Player<char>* current = players[i];
            Player<char>* opponent = players[1 - i];
            Move<char>* move = ui->get_move(current);

            while (!board->update_board(move)) {
                cout << "Invalid move! Please try again.\n";
                delete move;
                move = ui->get_move(current);
            }

            board->display_board();

            if (board->is_win(current)) {
                cout << string(60, '=') << "\n";
                cout << "GAME OVER - STRATEGIC VICTORY!\n";
                cout << string(60, '=') << "\n";
                cout << current->get_name() << " achieved three in a row!\n";
                cout << string(60, '=') << "\n";
                delete move;
                delete board;
                delete players[0];
                delete players[1];
                delete ui;
                return;
            }

            if (board->is_draw(current)) {
                cout << string(60, '=') << "\n";
                cout << "GAME OVER - DRAW!\n";
                cout << string(60, '=') << "\n";
                cout << "The game ends in a strategic draw!\n";
                cout << string(60, '=') << "\n";
                delete move;
                delete board;
                delete players[0];
                delete players[1];
                delete ui;
                return;
            }
            delete move;
        }
    }
}

void PlayGameFunctions::playPyramidGame() {
    cout << "\n" << string(60, '=') << "\n";
    cout << "          PYRAMID TIC-TAC-TOE - SHAPE CHALLENGE\n";
    cout << string(60, '=') << "\n";

    cout << "\nConfiguring first player (will use X symbol):\n";
    string name1 = getPlayerIdentity("First Player");
    PlayerType type1 = choosePlayerPersonality("First Player");

    cout << "\nConfiguring second player (will use O symbol):\n";
    string name2 = getPlayerIdentity("Second Player");
    PlayerType type2 = choosePlayerPersonality("Second Player");

    PyramidTicTacToe game;
    int row, col;
    bool gameRunning = true;

    cout << "\n" << string(50, '-') << "\n";
    cout << "GAME STARTING: " << name1 << " (X) vs " << name2 << " (O)\n";
    cout << string(50, '-') << "\n";

    while (gameRunning && !game.isGameOver()) {
        game.displayBoard();
        cout << game.getGameState() << endl;

        char currentPlayerSymbol = game.getCurrentPlayer();
        string currentPlayerName = (currentPlayerSymbol == 'X') ? name1 : name2;
        int currentPlayerType = (currentPlayerSymbol == 'X') ? static_cast<int>(type1) : static_cast<int>(type2);

        cout << currentPlayerName << "'s turn:\n";

        if (currentPlayerType == static_cast<int>(PlayerType::HUMAN)) {
            while (true) {
                cout << "Enter your move (row col): ";
                if (cin >> row >> col) {
                    if (game.makeMove(row, col)) {
                        break;
                    }
                    else {
                        cout << "Invalid move! Please try again." << endl;
                        cout << "Valid coordinates: Row 0: [0,0]" << endl;
                        cout << "                  Row 1: [1,0] [1,1] [1,2]" << endl;
                        cout << "                  Row 2: [2,0] [2,1] [2,2] [2,3] [2,4]" << endl;
                    }
                }
                else {
                    cout << "Invalid input! Please enter numbers only." << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
            }
        }
        else {
            cout << currentPlayerName << " is thinking...\n";
            bool isSmartAI = (currentPlayerType == static_cast<int>(PlayerType::AI));
            auto move = game.getComputerMove(isSmartAI);

            if (move.first != -1 && move.second != -1) {
                game.makeMove(move.first, move.second);
                cout << currentPlayerName << " plays at [" << move.first << "," << move.second << "]\n";
            }
            else {
                cout << "No valid moves available!\n";
                break;
            }
        }
        cout << endl;
    }

    game.displayBoard();
    cout << "=== GAME OVER ===" << endl;
    cout << game.getGameState() << endl;
    cout << "Total moves: " << game.getMoveCount() << endl;

    char winner = game.getWinner();
    if (winner != ' ') {
        string winnerName = (winner == 'X') ? name1 : name2;
        cout << "Congratulations " << winnerName << "!\n";
    }
}