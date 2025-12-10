#pragma once
#ifndef PLAY_GAME_FUNCTIONS_H
#define PLAY_GAME_FUNCTIONS_H

#include <string>

enum class PlayerType;

class PlayGameFunctions {
public:
    static void playXOGame();
    static void playNumericalGame();
    static void playSUSGame();
    static void playFourInRowGame();
    static void playMemoryGame();
    static void playMisereGame();
    static void play5x5Game();
    static void playWordGame();
    static void playInfinityGame();
    static void playMovingGame();
    static void playPyramidGame();
    static void playDiamondGame();
    static void playUltimateGame(); // Add this line
};

#endif