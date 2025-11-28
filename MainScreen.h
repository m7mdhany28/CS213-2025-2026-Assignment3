#pragma once
#ifndef MAIN_SCREEN_H
#define MAIN_SCREEN_H

class MainScreen {
private:
    enum class UserChoice {
        MISERE = 1,
        FIVE_BY_FIVE = 2,
        WORD = 3,
        INFINITY_GAME = 4,    // ← RENAME TO INFINITY_GAME
        MOVING = 5,
        PYRAMID = 6,
        EXIT = 7
    };

public:
    static void implementUserChoice(UserChoice choice);
    static void printMainMenuScreen();
    static void startBoardGameProgram();
};

#endif