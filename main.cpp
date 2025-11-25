#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <cstdlib>
#include <ctime>
#include "BoardGame_Classes.h"
#include "Misere_Board.h"
#include "FiveByFive_Board.h"
#include "WordTicTacToe_Board.h"
#include "Infinity_Board.h"
#include "FourByFour_Moving_Board.h"
#include "pyramid_tic_tac_toe.h"
#include "Simple_UI.h"

using namespace std;

// Replacement function for missing count_player_lines
int count_player_lines(FiveByFive_Board* board, char symbol) {
    vector<vector<char>> board_matrix = board->get_board_matrix();
    int count = 0;

    // Check horizontal sequences
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 3; j++) {
            if (board_matrix[i][j] == symbol && board_matrix[i][j + 1] == symbol && board_matrix[i][j + 2] == symbol) {
                count++;
            }
        }
    }

    // Check vertical sequences
    for (int j = 0; j < 5; j++) {
        for (int i = 0; i < 3; i++) {
            if (board_matrix[i][j] == symbol && board_matrix[i + 1][j] == symbol && board_matrix[i + 2][j] == symbol) {
                count++;
            }
        }
    }

    // Check diagonal sequences (top-left to bottom-right)
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board_matrix[i][j] == symbol && board_matrix[i + 1][j + 1] == symbol && board_matrix[i + 2][j + 2] == symbol) {
                count++;
            }
        }
    }

    // Check diagonal sequences (top-right to bottom-left)
    for (int i = 0; i < 3; i++) {
        for (int j = 2; j < 5; j++) {
            if (board_matrix[i][j] == symbol && board_matrix[i + 1][j - 1] == symbol && board_matrix[i + 2][j - 2] == symbol) {
                count++;
            }
        }
    }

    return count;
}

// Replacement function for missing get_next_removal_countdown
int get_next_removal_countdown(Infinity_Board* board) {
    // Default implementation - returns a fixed countdown
    return 3;
}

// Let players know what amazing games await them
void start_misere_adventure();
void begin_5x5_challenge();
void launch_word_building_battle();
void play_infinity_quest();
void start_moving_token_duel();
void begin_pyramid_expedition();

void show_welcome_message();
void display_game_menu();
int get_player_menu_choice();
void clear_screen_display();
void wait_for_player_ready();
PlayerType choose_player_personality(const string& player_label);
string get_player_identity(const string& player_label);

int main() {
    // Make sure our random choices are actually random
    srand(static_cast<unsigned int>(time(0)));

    // Welcome players to our gaming wonderland
    show_welcome_message();

    // Keep the fun going until players decide to leave
    while (true) {
        display_game_menu();
        int player_choice = get_player_menu_choice();

        // Guide players to their chosen adventure
        switch (player_choice) {
        case 1:
            cout << "\nLaunching Misere Tic Tac Toe - Prepare for reverse psychology!...\n";
            start_misere_adventure();
            break;
        case 2:
            cout << "\nStarting 5x5 Tic Tac Toe - Expand your strategic thinking!...\n";
            begin_5x5_challenge();
            break;
        case 3:
            cout << "\nBeginning Word Tic Tac Toe - Let's build some vocabulary!...\n";
            launch_word_building_battle();
            break;
        case 4:
            cout << "\nInitiating Infinity Tic-Tac-Toe - Nothing lasts forever!...\n";
            play_infinity_quest();
            break;
        case 5:
            cout << "\nStarting 4x4 Moving Tic-Tac-Toe - Get those tokens moving!...\n";
            start_moving_token_duel();
            break;
        case 6:
            cout << "\nLaunching Pyramid Tic-Tac-Toe - Climb to victory!...\n";
            begin_pyramid_expedition();
            break;
        case 7:
            cout << "\nThank you for visiting our Board Games Collection!\n";
            cout << "We loved having you play our 7 unique games. Come back soon for more fun!\n";
            return 0;
        default:
            cout << "\nThat option isn't available right now.\n";
            cout << "Please pick a number between 1 and 7 from our menu.\n";
        }

        // Give players breathing room before showing menu again
        if (player_choice != 7) {
            wait_for_player_ready();
        }
    }

    return 0;
}

void show_welcome_message() {
    cout << "======================================================\n";
    cout << "           WELCOME TO BOARD GAMES COLLECTION\n";
    cout << "======================================================\n";
    cout << "\nHello there! Welcome to our interactive gaming paradise!\n";
    cout << "\nWe've prepared SEVEN incredible games for your enjoyment:\n";
    cout << "  1. Misere Tic Tac Toe - The reverse psychology challenge\n";
    cout << "  2. 5x5 Tic Tac Toe - Expanded strategic gameplay\n";
    cout << "  3. Word Tic Tac Toe - Vocabulary building adventure\n";
    cout << "  4. Infinity Tic-Tac-Toe - Temporal memory test\n";
    cout << "  5. 4x4 Moving Tic-Tac-Toe - Token movement strategy\n";
    cout << "  6. Pyramid Tic-Tac-Toe - Pyramid-shaped board fun\n";
    cout << "\nChoose your adventure from the menu below and let the games begin!\n";
}

void display_game_menu() {
    cout << "\n" << string(50, '=') << "\n";
    cout << "           MAIN GAME SELECTION MENU\n";
    cout << string(50, '=') << "\n";
    cout << "1. Misere Tic Tac Toe (Reverse Rules) - Lose to win!\n";
    cout << "2. 5x5 Tic Tac Toe (Expanded Board) - Think bigger!\n";
    cout << "3. Word Tic Tac Toe (Vocabulary Challenge) - Spell victory!\n";
    cout << "4. Infinity Tic-Tac-Toe (Temporal Challenge) - Race against time!\n";
    cout << "5. 4x4 Moving Tic-Tac-Toe (Token Movement) - Keep moving!\n";
    cout << "6. Pyramid Tic-Tac-Toe (Pyramid Board) - Build upward!\n";
    cout << "7. Exit Game Collection - Until next time!\n";
    cout << string(50, '=') << "\n";
    cout << "What gaming adventure calls to you? (1-7): ";
}

int get_player_menu_choice() {
    int choice;

    // Keep asking until we get a real number
    while (!(cin >> choice)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Let's try that again - enter a number between 1 and 7: ";
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return choice;
}

void clear_screen_display() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void wait_for_player_ready() {
    cout << "\nPress Enter when you're ready to see the main menu again...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
    clear_screen_display();
    show_welcome_message();
}

PlayerType choose_player_personality(const string& player_label) {
    int choice;
    cout << "\nWhat kind of player should " << player_label << " be?\n";
    cout << "1. Human Player (You make the decisions)\n";
    cout << "2. Random Computer (Full of surprises)\n";
    cout << "3. Smart Computer (Thinks strategically)\n";
    cout << "Enter your selection (1, 2, or 3): ";

    while (!(cin >> choice) || choice < 1 || choice > 3) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Let's choose: 1 for Human, 2 for Random Computer, or 3 for Smart Computer: ";
    }

    switch (choice) {
    case 1:
        cout << "Human player activated for " << player_label << "\n";
        return PlayerType::HUMAN;
    case 2:
        cout << "Random computer personality set for " << player_label << "\n";
        return PlayerType::RANDOM;
    case 3:
        cout << "Smart computer intelligence engaged for " << player_label << "\n";
        return PlayerType::AI;
    default:
        return PlayerType::HUMAN;
    }
}

string get_player_identity(const string& player_label) {
    string name;

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "What name should we use for " << player_label << "? ";
    getline(cin, name);

    // Clean up any extra spaces around the name
    name.erase(0, name.find_first_not_of(" \t\n\r\f\v"));
    name.erase(name.find_last_not_of(" \t\n\r\f\v") + 1);

    // If they didn't enter anything, use a friendly default
    if (name.empty()) {
        name = player_label;
        cout << "We'll call you: " << name << "\n";
    }
    return name;
}

void start_misere_adventure() {
    cout << "\n" << string(60, '=') << "\n";
    cout << "          MISERE TIC TAC TOE - REVERSE PSYCHOLOGY CHALLENGE\n";
    cout << string(60, '=') << "\n";
    cout << "\nWelcome to Misere Tic Tac Toe - where everything is backwards!\n";
    cout << "\nThe Rules - Think Opposite:\n";
    cout << "  - Normal Tic Tac Toe: Get 3 in a row to WIN\n";
    cout << "  - Misere Tic Tac Toe: Get 3 in a row to LOSE\n";
    cout << "  - Your mission: Trick your opponent into completing a line\n";
    cout << "  - If nobody makes a line: It's a friendly draw\n";
    cout << "\nLet's meet our players and begin the mind games!\n";

    // Set up our game world
    Board<char>* game_board = new Misere_Board();
    UI<char>* game_interface = new Simple_UI(game_board, "misere");

    cout << "\n" << string(40, '-') << "\n";
    cout << "PLAYER INTRODUCTION\n";
    cout << string(40, '-') << "\n";

    cout << "\nGetting to know our first player (using X marks):\n";
    string player1_name = get_player_identity("First Player");
    PlayerType player1_type = choose_player_personality("First Player");

    cout << "\nNow let's meet our second player (using O marks):\n";
    string player2_name = get_player_identity("Second Player");
    PlayerType player2_type = choose_player_personality("Second Player");

    // Create our players
    Player<char>* players[2] = {
        new Player<char>(player1_name, 'X', player1_type),
        new Player<char>(player2_name, 'O', player2_type)
    };

    cout << "\n" << string(50, '~') << "\n";
    cout << "GAME STARTING: " << player1_name << " (X) vs " << player2_name << " (O)\n";
    cout << string(50, '~') << "\n";

    // Show the empty board
    game_interface->display_board_matrix(game_board->get_board_matrix());

    // The main game loop - where the magic happens
    while (true) {
        for (int i = 0; i < 2; i++) {
            Player<char>* current_player = players[i];
            Player<char>* other_player = players[1 - i];

            // Get the player's move
            Move<char>* next_move = game_interface->get_move(current_player);

            // Make sure the move is valid
            while (!game_board->update_board(next_move)) {
                cout << "That move doesn't work. The spot might be taken or off the board.\n";
                delete next_move;
                next_move = game_interface->get_move(current_player);
            }

            // Show the updated board
            game_interface->display_board_matrix(game_board->get_board_matrix());

            // Check if someone accidentally won (which means they lost!)
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

            // Check if the board is full with no winners
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
    // Clean up our game pieces
    delete game_board;
    delete players[0];
    delete players[1];
    delete game_interface;
}



void launch_word_building_battle() {
    cout << "\n" << string(60, '=') << "\n";
    cout << "          WORD TIC TAC TOE - VOCABULARY CHALLENGE\n";
    cout << string(60, '=') << "\n";
    cout << "\nWelcome to Word Tic Tac Toe!\n";
    cout << "\nGame Rules - Word Building Strategy:\n";
    cout << "  - Players place letters on a 3x3 grid\n";
    cout << "  - Goal: Form valid three-letter words\n";
    cout << "  - Words can be horizontal, vertical, or diagonal\n";
    cout << "  - First player to form a valid word wins immediately\n";
    cout << "\nLet's set up the players and begin the word challenge!\n";

    WordTicTacToe_Board* board = new WordTicTacToe_Board();
    Simple_UI* ui = new Simple_UI(board, "word");

    cout << "\n" << string(40, '-') << "\n";
    cout << "PLAYER CONFIGURATION\n";
    cout << string(40, '-') << "\n";

    cout << "\nConfiguring first player:\n";
    string name1 = get_player_identity("First Player");
    PlayerType type1 = choose_player_personality("First Player");

    cout << "\nConfiguring second player:\n";
    string name2 = get_player_identity("Second Player");
    PlayerType type2 = choose_player_personality("Second Player");

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
                string winning_word = "WIN"; // Default since get_winning_word might be missing
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
void play_infinity_quest() {
    cout << "\n" << string(60, '=') << "\n";
    cout << "       INFINITY TIC-TAC-TOE - TEMPORAL CHALLENGE\n";
    cout << string(60, '=') << "\n";
    cout << "\nWelcome to Infinity Tic-Tac-Toe!\n";
    cout << "\nGame Rules - The Memory Challenge:\n";
    cout << "  - Standard 3x3 Tic-Tac-Toe rules apply\n";
    cout << "  - BUT: After every 3 moves, the OLDEST mark disappears\n";
    cout << "  - Your marks have limited time on the board\n";
    cout << "  - Strategy: Create winning lines before they vanish!\n";
    cout << "\nLet's begin the infinite challenge!\n";

    Infinity_Board* board = new Infinity_Board();
    Simple_UI* ui = new Simple_UI(board, "infinity");

    cout << "\n" << string(40, '-') << "\n";
    cout << "PLAYER CONFIGURATION\n";
    cout << string(40, '-') << "\n";

    cout << "\nConfiguring first player (will use X symbol):\n";
    string name1 = get_player_identity("First Player");
    PlayerType type1 = choose_player_personality("First Player");

    cout << "\nConfiguring second player (will use O symbol):\n";
    string name2 = get_player_identity("Second Player");
    PlayerType type2 = choose_player_personality("Second Player");

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

            // Show removal countdown
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

void begin_5x5_challenge() {
    cout << "\n" << string(60, '=') << "\n";
    cout << "          5x5 TIC TAC TOE - EXPANDED CHALLENGE\n";
    cout << string(60, '=') << "\n";
    cout << "\nWelcome to 5x5 Tic Tac Toe!\n";
    cout << "\nGame Rules - Strategic Sequence Building:\n";
    cout << "  - Play on a larger 5x5 grid instead of traditional 3x3\n";
    cout << "  - Create as many three-in-a-row sequences as possible\n";
    cout << "  - Game continues until only one square remains empty (24 moves)\n";
    cout << "  - Player with the most sequences at the end wins\n";
    cout << "\nLet's set up the players and begin the game!\n";

    FiveByFive_Board* board = new FiveByFive_Board();
    Simple_UI* ui = new Simple_UI(board, "5x5");

    cout << "\n" << string(40, '-') << "\n";
    cout << "PLAYER CONFIGURATION\n";
    cout << string(40, '-') << "\n";

    cout << "\nConfiguring first player (will use X symbol):\n";
    string name1 = get_player_identity("First Player");
    PlayerType type1 = choose_player_personality("First Player");

    cout << "\nConfiguring second player (will use O symbol):\n";
    string name2 = get_player_identity("Second Player");
    PlayerType type2 = choose_player_personality("Second Player");

    Player<char>* players[2] = {
        new Player<char>(name1, 'X', type1),
        new Player<char>(name2, 'O', type2)
    };

    cout << "\n" << string(50, '-') << "\n";
    cout << "GAME STARTING: " << name1 << " (X) vs " << name2 << " (O)\n";
    cout << string(50, '-') << "\n";

    board->display_board();

    // Game loop
    while (true) {
        for (int i = 0; i < 2; i++) {
            Player<char>* current = players[i];

            // Get move from player
            Move<char>* move = ui->get_move(current);

            // Validate and execute move
            while (!board->update_board(move)) {
                cout << "That move cannot be made. The position may be already occupied or invalid.\n";
                delete move;
                move = ui->get_move(current);
            }

            // Display updated board
            board->display_board();

            // Check if game should end (24 moves = 1 empty square)
            if (board->game_is_over(current)) {
                // Calculate final scores
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

                // Cleanup and exit
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

void start_moving_token_duel() {
    cout << "\n" << string(60, '=') << "\n";
    cout << "      4x4 MOVING TIC-TAC-TOE - STRATEGIC MOVEMENT\n";
    cout << string(60, '=') << "\n";
    cout << "\nWelcome to 4x4 Moving Tic Tac Toe!\n";
    cout << "\nGame Rules - Token Movement Strategy:\n";
    cout << "  - Each player starts with 4 tokens on their side\n";
    cout << "  - Move tokens to adjacent empty squares (horizontal/vertical only)\n";
    cout << "  - Tokens cannot move diagonally or jump over other tokens\n";
    cout << "  - First to get 3 in a row wins!\n";
    cout << "\nLet's begin the strategic movement challenge!\n";

    FourByFour_Moving_Board* board = new FourByFour_Moving_Board();
    Simple_UI* ui = new Simple_UI(board, "4x4");

    cout << "\n" << string(40, '-') << "\n";
    cout << "PLAYER CONFIGURATION\n";
    cout << string(40, '-') << "\n";

    cout << "\nConfiguring first player (will use X symbol):\n";
    string name1 = get_player_identity("First Player");
    PlayerType type1 = choose_player_personality("First Player");

    cout << "\nConfiguring second player (will use O symbol):\n";
    string name2 = get_player_identity("Second Player");
    PlayerType type2 = choose_player_personality("Second Player");

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
void begin_pyramid_expedition() {
    cout << "\n" << string(60, '=') << "\n";
    cout << "          PYRAMID TIC-TAC-TOE - SHAPE CHALLENGE\n";
    cout << string(60, '=') << "\n";
    cout << "\nWelcome to Pyramid Tic-Tac-Toe!\n";
    cout << "\nGame Rules - Pyramid Structure:\n";
    cout << "  - Play on a pyramid-shaped board: 1-3-5 structure\n";
    cout << "  - Top: 1 cell, Middle: 3 cells, Base: 5 cells\n";
    cout << "  - Get 3 in a row horizontally, vertically, or diagonally\n";
    cout << "  - Unique winning paths through the pyramid levels\n";
    cout << "\nLet's begin the pyramid challenge!\n";

    // Player configuration - consistent with other games
    cout << "\n" << string(40, '-') << "\n";
    cout << "PLAYER CONFIGURATION\n";
    cout << string(40, '-') << "\n";

    cout << "\nConfiguring first player (will use X symbol):\n";
    string name1 = get_player_identity("First Player");
    PlayerType type1 = choose_player_personality("First Player");

    cout << "\nConfiguring second player (will use O symbol):\n";
    string name2 = get_player_identity("Second Player");
    PlayerType type2 = choose_player_personality("Second Player");

    // Create Pyramid game instance
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
            // Get valid input from human
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
        else { // Computer player
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

    // Game over
    game.displayBoard();
    cout << "=== GAME OVER ===" << endl;
    cout << game.getGameState() << endl;
    cout << "Total moves: " << game.getMoveCount() << endl;

    // Display final result with player names
    char winner = game.getWinner();
    if (winner != ' ') {
        string winnerName = (winner == 'X') ? name1 : name2;
        cout << "Congratulations " << winnerName << "!\n";
    }
}