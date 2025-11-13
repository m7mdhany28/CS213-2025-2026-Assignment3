#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <cstdlib>
#include <ctime>
#include "BoardGame_Classes.h"
#include "Misere_Board.h"
#include "Simple_UI.h"
using namespace std;

// Function declarations
void play_misere_game();
void display_welcome();
void display_menu();
int get_user_choice();
void clear_screen();
void press_enter_to_continue();
PlayerType choose_player_type(const string& player_label);
string get_player_name(const string& player_label);

int main() {
    srand(static_cast<unsigned int>(time(0)));  // Seed for random computer moves
    display_welcome();

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
            cout << "\nPreparing 5x5 Tic Tac Toe...\n";
            cout << "This exciting expanded version is currently in development.\n";
            cout << "Our team is working hard to deliver more strategic gameplay soon.\n";
            break;
        case 3:
            cout << "\nInitializing Four-in-a-Row...\n";
            cout << "The classic vertical connect game is coming in our next release.\n";
            cout << "Get ready for some gravity-defying fun!\n";
            break;
        case 4:
            cout << "\nSetting up Word Tic Tac Toe...\n";
            cout << "This vocabulary challenge is being fine-tuned for your enjoyment.\n";
            cout << "Dictionary integration and word validation are being perfected.\n";
            break;
        case 5:
            cout << "\nThank you for playing our Board Games Collection!\n";
            cout << "We hope you enjoyed the games. Come back soon for more updates!\n";
            cout << "Closing application...\n";
            return 0;
        default:
            cout << "\nThat selection is not available.\n";
            cout << "Please choose a number between 1 and 5 from the menu.\n";
        }

        if (choice != 5) {
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
    cout << "\nAvailable Features:\n";
    cout << "  - Multiple engaging board games in one application\n";
    cout << "  - Play with friends or challenge computer opponents\n";
    cout << "  - Various difficulty levels for computer players\n";
    cout << "  - Intuitive interface and clear instructions\n";
    cout << "\nSelect a game from the menu below to begin your adventure!\n";
}

void display_menu() {
    cout << "\n" << string(50, '=') << "\n";
    cout << "           MAIN GAME SELECTION MENU\n";
    cout << string(50, '=') << "\n";
    cout << "1. Misere Tic Tac Toe (Reverse Rules)\n";
    cout << "2. 5x5 Tic Tac Toe (Expanded Board) - Coming Soon\n";
    cout << "3. Four-in-a-Row (Connect Game) - Coming Soon\n";
    cout << "4. Word Tic Tac Toe (Vocabulary Challenge) - Coming Soon\n";
    cout << "5. Exit Game Collection\n";
    cout << string(50, '=') << "\n";
    cout << "Please enter your selection (1-5): ";
}

int get_user_choice() {
    int choice;

    while (!(cin >> choice)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Please enter a valid number between 1 and 5: ";
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
    cout << "Please enter a name for " << player_label << ": ";
    getline(cin, name);
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
    UI<char>* ui = new Simple_UI(board);

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
        cout << "Note: " << name1 << " is controlled by the computer\n";
    }
    if (type2 != PlayerType::HUMAN) {
        cout << "Note: " << name2 << " is controlled by the computer\n";
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