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
#include "Simple_UI.h"
using namespace std;

// Function declarations
void play_misere_game();
void play_5x5_game();
void play_word_game();
void display_welcome();
void display_menu();
int get_user_choice();
void clear_screen();
void press_enter_to_continue();
PlayerType choose_player_type(const string& player_label);
string get_player_name(const string& player_label);

int main() {
    srand(static_cast<unsigned int>(time(0)));  

    while (true) {
        display_menu();
        int choice = get_user_choice();

        switch (choice) {
        case 1:
            cout << "\nStarting Misere Tic Tac Toe Game...\n";
            cout << "Loading game environment...\n";
            play_misere_game();
            break;
        case 2:
            cout << "\nStarting 5x5 Tic Tac Toe Game...\n";
            cout << "Initializing expanded game board...\n";
            play_5x5_game();
            break;
        case 3:
            cout << "\nStarting Word Tic Tac Toe Game...\n";
            cout << "Loading dictionary and word validation...\n";
            play_word_game();
            break;
        case 4:
            cout << "\nThank you for playing our Board Games Collection!\n";
            cout << "We hope you enjoyed the games. Come back soon for more updates!\n";
            cout << "Closing application...\n";
            return 0;
        default:
            cout << "\nThat selection is not available.\n";
            cout << "Please choose a number between 1 and 4 from the menu.\n";
        }

        if (choice != 4) {
            press_enter_to_continue();
        }
    }

    return 0;
}

void display_welcome() {
    cout << "======================================================\n";
    cout << "           BOARD GAMES COLLECTION CENTER\n";
    cout << "======================================================\n";
    cout << "\nWelcome to our interactive gaming platform!\n";
    cout << "\nNow featuring THREE exciting games:\n";
    cout << "  1. Misere Tic Tac Toe - Reverse psychology challenge\n";
    cout << "  2. 5x5 Tic Tac Toe - Expanded strategic gameplay\n";
    cout << "  3. Word Tic Tac Toe - Vocabulary building fun\n";
    cout << "\nSelect a game from the menu below to begin your adventure!\n";
}

void display_menu() {
    cout << "\n" << string(50, '=') << "\n";
    cout << "           MAIN GAME SELECTION MENU\n";
    cout << string(50, '=') << "\n";
    cout << "1. Misere Tic Tac Toe (Reverse Rules)\n";
    cout << "2. 5x5 Tic Tac Toe (Expanded Board)\n";
    cout << "3. Word Tic Tac Toe (Vocabulary Challenge)\n";
    cout << "4. Exit Game Collection\n";
    cout << string(50, '=') << "\n";
    cout << "Please enter your selection (1-4): ";
}

int get_user_choice() {
    int choice;

    while (!(cin >> choice)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Please enter a valid number between 1 and 4: ";
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return choice;
}

void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void press_enter_to_continue() {
    cout << "\nPress the Enter key to return to the main menu...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
    clear_screen();
    display_welcome();
}

PlayerType choose_player_type(const string& player_label) {
    int choice;
    cout << "\nPlease select the type for " << player_label << ":\n";
    cout << "1. Human Player (You control the moves)\n";
    cout << "2. Random Computer (Makes random valid moves)\n";
    cout << "3. Smart Computer (Uses strategy to play)\n";
    cout << "Enter your selection (1, 2, or 3): ";

    while (!(cin >> choice) || choice < 1 || choice > 3) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Please enter 1 for Human, 2 for Random Computer, or 3 for Smart Computer: ";
    }

    switch (choice) {
    case 1:
        cout << "Human player selected for " << player_label << "\n";
        return PlayerType::HUMAN;
    case 2:
        cout << "Random computer selected for " << player_label << "\n";
        return PlayerType::RANDOM;
    case 3:
        cout << "Smart computer selected for " << player_label << "\n";
        return PlayerType::AI;
    default:
        return PlayerType::HUMAN;
    }
}

string get_player_name(const string& player_label) {
    string name;

    // Clear the input buffer before reading the name
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Please enter a name for " << player_label << ": ";
    getline(cin, name);

    // Remove any leading/trailing whitespace
    name.erase(0, name.find_first_not_of(" \t\n\r\f\v"));
    name.erase(name.find_last_not_of(" \t\n\r\f\v") + 1);

    if (name.empty()) {
        name = player_label;
        cout << "Using default name: " << name << "\n";
    }
    return name;
}

void play_misere_game() {
    cout << "\n" << string(60, '=') << "\n";
    cout << "          MISERE TIC TAC TOE - REVERSE CHALLENGE\n";
    cout << string(60, '=') << "\n";
    cout << "\nWelcome to Misere Tic Tac Toe!\n";
    cout << "\nGame Rules - The Reverse Psychology Edition:\n";
    cout << "  - Normal Tic Tac Toe: Get 3 in a row to WIN\n";
    cout << "  - Misere Tic Tac Toe: Get 3 in a row to LOSE\n";
    cout << "  - Your goal: Force your opponent to complete a line\n";
    cout << "  - If the board fills with no complete lines: It's a draw\n";
    cout << "\nHow to Play:\n";
    cout << "  - Players take turns placing X and O on a 3x3 grid\n";
    cout << "  - Enter coordinates as ROW then COLUMN (both 0-2)\n";
    cout << "  - Example: Enter '1 1' to place in the center position\n";
    cout << "  - Think carefully: Avoid making lines yourself!\n";
    cout << "\nLet's set up the players and begin the game!\n";

    // Create game components
    Board<char>* board = new Misere_Board();
    UI<char>* ui = new Simple_UI(board, "misere");

    // Player setup
    cout << "\n" << string(40, '-') << "\n";
    cout << "PLAYER CONFIGURATION\n";
    cout << string(40, '-') << "\n";

    // Player 1 setup
    cout << "\nConfiguring first player (will use X symbol):\n";
    string name1 = get_player_name("First Player");
    PlayerType type1 = choose_player_type("First Player");

    // Player 2 setup  
    cout << "\nConfiguring second player (will use O symbol):\n";
    string name2 = get_player_name("Second Player");
    PlayerType type2 = choose_player_type("Second Player");

    Player<char>* players[2] = {
        new Player<char>(name1, 'X', type1),
        new Player<char>(name2, 'O', type2)
    };

    cout << "\n" << string(50, '-') << "\n";
    cout << "GAME STARTING: " << name1 << " (X) vs " << name2 << " (O)\n";
    cout << string(50, '-') << "\n";

    if (type1 != PlayerType::HUMAN) {
        cout << "Note: " << name1 << " is computer controlled\n";
    }
    if (type2 != PlayerType::HUMAN) {
        cout << "Note: " << name2 << " is computer controlled\n";
    }

    cout << "\nThe game board is displayed below.\n";
    cout << "Columns are numbered 0-2 at the top, rows 0-2 on the left.\n";

    // Custom game loop for Misere rules
    ui->display_board_matrix(board->get_board_matrix());

    while (true) {
        for (int i : {0, 1}) {
            Player<char>* current = players[i];
            Player<char>* opponent = players[1 - i];

            Move<char>* move = ui->get_move(current);

            while (!board->update_board(move)) {
                cout << "That move cannot be made. The position may be already occupied or invalid.\n";
                cout << "Please select an empty position on the board.\n";
                delete move;
                move = ui->get_move(current);
            }

            ui->display_board_matrix(board->get_board_matrix());

            // MISERE RULES: If you get 3 in a row, YOU LOSE
            if (board->is_win(current)) {
                cout << string(60, '=') << "\n";
                cout << "GAME OVER - FINAL RESULT\n";
                cout << string(60, '=') << "\n";
                ui->display_message(current->get_name() + " completed three in a row and loses the game!");
                ui->display_message(opponent->get_name() + " wins by strategic play!");
                ui->display_message("Congratulations to the winner!");
                cout << string(60, '=') << "\n";
                delete move;
                goto game_over;
            }

            if (board->is_draw(current)) {
                cout << string(60, '=') << "\n";
                cout << "GAME OVER - FINAL RESULT\n";
                cout << string(60, '=') << "\n";
                ui->display_message("The game ends in a draw!");
                ui->display_message("Both players successfully avoided creating three in a row!");
                ui->display_message("Excellent defensive play by both competitors!");
                cout << string(60, '=') << "\n";
                delete move;
                goto game_over;
            }

            delete move;
        }
    }

game_over:
    // Cleanup
    delete board;
    delete players[0];
    delete players[1];
    delete ui;

    cout << "\nMisere Tic Tac Toe game has concluded.\n";
    cout << "Thank you for playing! Returning to the main menu...\n";
}

void play_5x5_game() {
    cout << "\n" << string(60, '=') << "\n";
    cout << "          5x5 TIC TAC TOE - EXPANDED CHALLENGE\n";
    cout << string(60, '=') << "\n";
    cout << "\nWelcome to 5x5 Tic Tac Toe!\n";
    cout << "\nGame Rules - Strategic Sequence Building:\n";
    cout << "  - Play on a larger 5x5 grid instead of traditional 3x3\n";
    cout << "  - Create as many three-in-a-row sequences as possible\n";
    cout << "  - Game continues until only one square remains empty\n";
    cout << "  - Player with the most sequences at the end wins\n";
    cout << "  - Total moves: 24 (12 for each player)\n";
    cout << "\nHow to Play:\n";
    cout << "  - Players take turns placing X and O on a 5x5 grid\n";
    cout << "  - Enter coordinates as ROW then COLUMN (both 0-4)\n";
    cout << "  - Multiple three-in-a-row sequences can overlap\n";
    cout << "  - Think strategically to maximize your sequences!\n";
    cout << "\nLet's set up the players and begin the game!\n";

    // Create game components
    FiveByFive_Board* board = new FiveByFive_Board();
    Simple_UI* ui = new Simple_UI(board, "5x5");

    // Player setup
    cout << "\n" << string(40, '-') << "\n";
    cout << "PLAYER CONFIGURATION\n";
    cout << string(40, '-') << "\n";

    // Player 1 setup
    cout << "\nConfiguring first player (will use X symbol):\n";
    string name1 = get_player_name("First Player");
    PlayerType type1 = choose_player_type("First Player");

    // Player 2 setup  
    cout << "\nConfiguring second player (will use O symbol):\n";
    string name2 = get_player_name("Second Player");
    PlayerType type2 = choose_player_type("Second Player");

    Player<char>* players[2] = {
        new Player<char>(name1, 'X', type1),
        new Player<char>(name2, 'O', type2)
    };

    cout << "\n" << string(50, '-') << "\n";
    cout << "GAME STARTING: " << name1 << " (X) vs " << name2 << " (O)\n";
    cout << string(50, '-') << "\n";

    if (type1 != PlayerType::HUMAN) cout << "Note: " << name1 << " is computer controlled\n";
    if (type2 != PlayerType::HUMAN) cout << "Note: " << name2 << " is computer controlled\n";

    cout << "\nThe 5x5 game board is displayed below.\n";
    cout << "Columns are numbered 0-4 at the top, rows 0-4 on the left.\n";

    // Game loop for 5x5
    ui->display_board_matrix(board->get_board_matrix());

    while (true) {
        for (int i : {0, 1}) {
            Player<char>* current = players[i];

            Move<char>* move = ui->get_move(current);

            while (!board->update_board(move)) {
                cout << "That move cannot be made. The position may be already occupied or invalid.\n";
                cout << "Please select an empty position on the board.\n";
                delete move;
                move = ui->get_move(current);
            }

            ui->display_board_matrix(board->get_board_matrix());

            if (board->game_is_over(current)) {
                // Display final results with sequence counts
                int player1_lines = board->count_player_lines('X');
                int player2_lines = board->count_player_lines('O');

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
                    cout << "The game is a TIE!\n";
                }
                cout << string(50, '=') << "\n";
                delete move;
                goto game_over;
            }

            delete move;
        }
    }

game_over:
    // Cleanup
    delete board;
    delete players[0];
    delete players[1];
    delete ui;

    cout << "\n5x5 Tic Tac Toe game has concluded.\n";
    cout << "Thank you for playing! Returning to the main menu...\n";
}

void play_word_game() {
    cout << "\n" << string(60, '=') << "\n";
    cout << "          WORD TIC TAC TOE - VOCABULARY CHALLENGE\n";
    cout << string(60, '=') << "\n";
    cout << "\nWelcome to Word Tic Tac Toe!\n";
    cout << "\nGame Rules - Word Building Strategy:\n";
    cout << "  - Players place letters on a 3x3 grid\n";
    cout << "  - Goal: Form valid three-letter words\n";
    cout << "  - Words can be horizontal, vertical, or diagonal\n";
    cout << "  - First player to form a valid word wins immediately\n";
    cout << "  - If board fills with no valid words: It's a draw\n";
    cout << "  - Dictionary of valid words is provided\n";
    cout << "\nHow to Play:\n";
    cout << "  - Players choose ANY letter (A-Z) for each move\n";
    cout << "  - Then enter coordinates as ROW then COLUMN (both 0-2)\n";
    cout << "  - Build upon existing letters to form words\n";
    cout << "  - Think about word possibilities with each move!\n";
    cout << "\nLet's set up the players and begin the word challenge!\n";

    // Create game components
    WordTicTacToe_Board* board = new WordTicTacToe_Board();
    Simple_UI* ui = new Simple_UI(board, "word");

    // Player setup
    cout << "\n" << string(40, '-') << "\n";
    cout << "PLAYER CONFIGURATION\n";
    cout << string(40, '-') << "\n";

    // Player 1 setup
    cout << "\nConfiguring first player:\n";
    string name1 = get_player_name("First Player");
    PlayerType type1 = choose_player_type("First Player");

    // Player 2 setup  
    cout << "\nConfiguring second player:\n";
    string name2 = get_player_name("Second Player");
    PlayerType type2 = choose_player_type("Second Player");

    Player<char>* players[2] = {
        new Player<char>(name1, '?', type1), // Using '?' as placeholder
        new Player<char>(name2, '?', type2)  // Using '?' as placeholder
    };

    cout << "\n" << string(50, '-') << "\n";
    cout << "GAME STARTING: " << name1 << " vs " << name2 << "\n";
    cout << string(50, '-') << "\n";

    if (type1 != PlayerType::HUMAN) cout << "Note: " << name1 << " is computer controlled\n";
    if (type2 != PlayerType::HUMAN) cout << "Note: " << name2 << " is computer controlled\n";

    cout << "\nThe word game board is displayed below.\n";
    cout << "Build three-letter words horizontally, vertically, or diagonally!\n";

    // Game loop for word game
    ui->display_board_matrix(board->get_board_matrix());

    while (true) {
        for (int i : {0, 1}) {
            Player<char>* current = players[i];
            Player<char>* opponent = players[1 - i];

            Move<char>* move = ui->get_move(current);

            while (!board->update_board(move)) {
                cout << "That move cannot be made. The position may be already occupied or invalid.\n";
                cout << "Please select an empty position on the board.\n";
                delete move;
                move = ui->get_move(current);
            }

            ui->display_board_matrix(board->get_board_matrix());

            if (board->is_win(current)) {
                string winning_word = board->get_winning_word();
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
                cout << "Excellent defensive play by both competitors!\n";
                cout << string(50, '=') << "\n";
                delete move;
                goto game_over;
            }

            delete move;
        }
    }

game_over:
    // Cleanup
    delete board;
    delete players[0];
    delete players[1];
    delete ui;

    cout << "\nWord Tic Tac Toe game has concluded.\n";
    cout << "Thank you for playing! Returning to the main menu...\n";
}