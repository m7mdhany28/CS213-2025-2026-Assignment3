#pragma once
#ifndef PLAY_GAME_FUNCTIONS_H
#define PLAY_GAME_FUNCTIONS_H

#include "BoardGame_Classes.h"
#include <string>

std::string getPlayerIdentity(const std::string& playerLabel);
PlayerType choosePlayerPersonality(const std::string& playerLabel);

class PlayGameFunctions {
public:
    static void playXOGame();
    static void playNumericalGame();
    static void playSUSGame();
    static void playFourInRowGame();
    static void playInfinityGame();
    static void playMisereGame();
    static void play5x5Game();
    static void playWordGame();
    static void playMovingGame();
    static void playPyramidGame();
    static void playMemoryGame();
    static void playDiamondGame();
};

#endif