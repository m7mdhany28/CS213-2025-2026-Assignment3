#include<iostream>
#include<vector>
#include<memory>
#include<string>
#include"BoardGame_Classes.h"
#include"Numerical_Board.h"
#include"Numerical_UI.h"
#include"SUS_Board.h"
#include"SUS_UI.h"
#include"FourInRow_UI.h"
#include"FourInRow_Board.h"
#include"PlayGameFunctions.h"
#include "Misere_Board.h"
#include "FiveByFive_Board.h"
#include "WordTicTacToe_Board.h"
#include "Infinity_Board.h"
#include "FourByFour_Moving_Board.h"
#include "pyramid_tic_tac_toe.h"
#include "Simple_UI.h"
#include <limits>
#include"MemoryGame_UI.h"
#include"Diamond_Classes.h"
#include"XO_Board.h"
#include"XO_UI.h"
#include "UltimateTicTacToe_UI.h"

using namespace std;

// Player setup helper functions - MOVE TO HEADER TO REDUCE STACK USAGE
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
void PlayGameFunctions::playUltimateGame() {
    cout << "\n" << string(60, '=') << "\n";
    cout << "      ULTIMATE TIC TAC TOE - META CHALLENGE!\n";
    cout << string(60, '=') << "\n";
    cout << "9 Boards in 1 Game - Win the small boards to win the big one!\n";
    cout << "Rules:\n";
    cout << "1. Play in any board to start\n";
    cout << "2. Your next move must be in the board corresponding to your last move\n";
    cout << "3. Win a small board by getting 3 in a row\n";
    cout << "4. Win the game by winning 3 small boards in a row on the main board\n";
    cout << string(60, '=') << "\n";

    UltimateTicTacToe_UI* game_ui = new UltimateTicTacToe_UI();
    UltimateTicTacToe_Board* ultimate_board = new UltimateTicTacToe_Board();
    Player<char>** players = game_ui->setup_players();

    {
        GameManager<char> ultimate_game(ultimate_board, players, game_ui);
        ultimate_game.run();
    }

    // Cleanup
    delete ultimate_board;
    for (int i = 0; i < 2; ++i) {
        delete players[i];
    }
    delete[] players;
    delete game_ui;

    cout << "\nThank you for playing Ultimate Tic Tac Toe!\n";
}
PlayerType choosePlayerPersonality(const string& playerLabel) {
    int choice = 0; // FIXED: Initialize variable
    cout << "\nWhat kind of player should " << playerLabel << " be?\n";
    cout << "1. Human Player (You make the decisions)\n";
    cout << "2. Computer Player (Strategic AI)\n";
    cout << "Enter your selection (1 or 2): ";

    while (!(cin >> choice) || choice < 1 || choice > 2) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Please enter 1 for Human or 2 for Computer: ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    switch (choice) {
    case 1: return PlayerType::HUMAN;
    case 2: return PlayerType::COMPUTER;
    default: return PlayerType::HUMAN;
    }
}

// Game function implementations - REDUCE STACK USAGE BY USING HEAP FOR LARGE OBJECTS
void PlayGameFunctions::playXOGame() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    // Use heap allocation for large stack usage
    UI<char>* game_ui = new XO_UI();
    Board<char>* xo_board = new X_O_Board();
    Player<char>** players = game_ui->setup_players();

    {
        GameManager<char> x_o_game(xo_board, players, game_ui);
        x_o_game.run();
    } // GameManager destroyed here to free stack

    delete xo_board;
    for (int i = 0; i < 2; ++i) {
        delete players[i];
    }
    delete game_ui;
    delete[] players;
}

void PlayGameFunctions::playNumericalGame() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    UI<int>* game_ui = new Numerical_UI();
    Board<int>* NumericalBoard = new Numerical_Board();
    Player<int>** players = game_ui->setup_players();

    {
        GameManager<int> NumericalGame(NumericalBoard, players, game_ui);
        NumericalGame.run();
    }

    delete NumericalBoard;
    for (int i = 0; i < 2; ++i) {
        delete players[i];
    }
    delete game_ui;
    delete[] players;
}

void PlayGameFunctions::playSUSGame() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    UI<char>* game_ui = new SUS_UI();
    Board<char>* SUSBoard = new SUS_Board();
    Player<char>** players = game_ui->setup_players();

    {
        GameManager<char> SUSGame(SUSBoard, players, game_ui);
        SUSGame.run();
    }

    delete SUSBoard;
    for (int i = 0; i < 2; ++i) {
        delete players[i];
    }
    delete game_ui;
    delete[] players;
}

void PlayGameFunctions::playFourInRowGame() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    UI<char>* game_ui = new FourInRow_UI();
    Board<char>* FourInRowBoard = new FourInRow_Board();
    Player<char>** players = game_ui->setup_players();

    {
        GameManager<char> FourInRowGame(FourInRowBoard, players, game_ui);
        FourInRowGame.run();
    }

    delete FourInRowBoard;
    for (int i = 0; i < 2; ++i) {
        delete players[i];
    }
    delete game_ui;
    delete[] players;
}

void PlayGameFunctions::playMemoryGame() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    UI<char>* game_ui = new MemoryGame_UI();

    Board<char>* MemoryGameBoard = new X_O_Board();

    Player<char>** players = game_ui->setup_players();

    GameManager<char> MemoryGame(MemoryGameBoard , players , game_ui);

    MemoryGame.run();

    delete MemoryGameBoard;
    
    for(int i = 0 ; i < 2 ; ++i){
        delete players[i];
    }

    delete game_ui;

    delete[] players;
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

    // FIXED: Proper template instantiation
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

            while (next_move != nullptr && !game_board->update_board(next_move)) {
                cout << "That move doesn't work. The spot might be taken or off the board.\n";
                delete next_move;
                next_move = game_interface->get_move(current_player);
            }

            if (next_move == nullptr) continue;

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

    // FIXED: Proper template instantiation
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

            while (move != nullptr && !board->update_board(move)) {
                cout << "That move cannot be made. The position may be already occupied or invalid.\n";
                delete move;
                move = ui->get_move(current);
            }

            if (move == nullptr) continue;

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

    WordTicTacToe_Board* word_board = new WordTicTacToe_Board();

    // Use Simple_UI with proper initialization
    Simple_UI* ui = new Simple_UI(word_board, "word");

    cout << "\nConfiguring first player:\n";
    string name1 = getPlayerIdentity("First Player");
    PlayerType type1 = choosePlayerPersonality("First Player");

    cout << "\nConfiguring second player:\n";
    string name2 = getPlayerIdentity("Second Player");
    PlayerType type2 = choosePlayerPersonality("Second Player");

    Player<char>* players[2] = {
        new Player<char>(name1, 'A', type1),
        new Player<char>(name2, 'B', type2)
    };

    cout << "\n" << string(50, '-') << "\n";
    cout << "GAME STARTING: " << name1 << " (A) vs " << name2 << " (B)\n";
    cout << string(50, '-') << "\n";

    // Create a simple game loop
    int current_player = 0;
    bool game_over = false;

    while (!game_over) {
        Player<char>* current = players[current_player];

        // Display board
        ui->display_board_matrix(word_board->get_board_matrix());

        // Get move
        Move<char>* move = ui->get_move(current);

        if (move != nullptr) {
            if (word_board->update_board(move)) {
                // Check for win
                if (word_board->is_win(current)) {
                    ui->display_board_matrix(word_board->get_board_matrix());
                    cout << "\n" << string(50, '=') << "\n";
                    cout << "GAME OVER - WORD FOUND!\n";
                    cout << string(50, '=') << "\n";
                    cout << current->get_name() << " formed three '" << current->get_symbol() << "' in a row!\n";
                    cout << "WINNER: " << current->get_name() << "!\n";
                    cout << string(50, '=') << "\n";
                    game_over = true;
                }
                // Check for draw
                else if (word_board->is_draw(current)) {
                    ui->display_board_matrix(word_board->get_board_matrix());
                    cout << string(50, '=') << "\n";
                    cout << "GAME OVER - FINAL RESULT\n";
                    cout << string(50, '=') << "\n";
                    cout << "The game ends in a draw!\n";
                    cout << string(50, '=') << "\n";
                    game_over = true;
                }
            }
            delete move;
        }

        // Switch player
        current_player = 1 - current_player;
    }

    // Cleanup
    delete word_board;
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

    // FIXED: Proper template instantiation
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

            while (move != nullptr && !board->update_board(move)) {
                cout << "That move cannot be made. The position may be already occupied or invalid.\n";
                delete move;
                move = ui->get_move(current);
            }

            if (move == nullptr) continue;

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

    // FIXED: Proper template instantiation
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

            while (move != nullptr && !board->update_board(move)) {
                cout << "Invalid move! Please try again.\n";
                delete move;
                move = ui->get_move(current);
            }

            if (move == nullptr) continue;

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
    int row = 0, col = 0; // FIXED: Initialize variables
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
            bool validMove = false;
            while (!validMove) {
                cout << "Enter your move (row col): ";
                if (cin >> row >> col) {
                    if (game.makeMove(row, col)) {
                        validMove = true;
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

void PlayGameFunctions::playDiamondGame(){
    UI<char>* game_ui = new Diamond_UI();

    // Create the game board - Diamond shape on 5x5 grid
    Board<char>* diamond_board = new Diamond_Board();

    // Use the UI to set up the players for the game
    Player<char>** players = game_ui->setup_players();

    // Create the game manager with the board and the array of players
    GameManager<char> diamond_game(diamond_board, players, game_ui);

    // Display game instructions
    cout << "\n=== Diamond Tic-Tac-Toe Rules ===\n";
    cout << "1. The board is arranged in a diamond shape (5x5 grid)\n";
    cout << "2. Players take turns placing X or O on valid positions\n";
    cout << "3. To WIN: Complete BOTH a line of 3 AND a line of 4 simultaneously\n";
    cout << "4. The two lines must be in different directions\n";
    cout << "5. They can share one common mark\n";
    cout << "6. Lines can be horizontal, vertical, or diagonal\n";
    cout << "\nValid positions:\n";
    cout << "  Row 0: column 2 only\n";
    cout << "  Row 1: columns 1, 2, 3\n";
    cout << "  Row 2: columns 0, 1, 2, 3, 4 (full row)\n";
    cout << "  Row 3: columns 1, 2, 3\n";
    cout << "  Row 4: column 2 only\n";
    cout << "\nPress Enter to start...";
    cin.ignore();
    cin.get();

    // Run the game loop
    diamond_game.run();

    // --- Cleanup ---
    // Delete the dynamically allocated board object
    delete diamond_board;

    // Delete the individual player objects
    for (int i = 0; i < 2; ++i) {
        delete players[i];
    }
    
    // Delete the dynamically allocated array of player pointers
    delete[] players;

    // Delete the UI object
    delete game_ui;

    cout << "\nThank you for playing Diamond Tic-Tac-Toe!\n";
}