#pragma once
#ifndef MAIN_SCREEN_H
#define MAIN_SCREEN_H

class MainScreen {
private:
    enum class UserChoice {
        XOGAME = 1,
        NUMERICALGAME = 2,
        SUSGAME = 3,
        FOURINROWGAME = 4,
        INFINITYGAME = 5,
        MISERE = 6,
        FIVE_BY_FIVE = 7,
        WORD = 8,
        MOVING = 9,
        PYRAMID = 10,
        MEMORYGAME = 11,
        DIAMONDGAME = 12,
        ULTIMATE = 13,  
        EXIT = 14       
    };

public:
    static void implementUserChoice(UserChoice choice);
    static void printMainMenuScreen();
    static void startBoardGameProgram();
};

#endif