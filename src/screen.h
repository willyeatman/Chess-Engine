#pragma once

#include <iostream>
#include <inttypes.h>
#include <array>

/**
 * @brief Prints out the game
 * 
 * @param board array of bitboards 
 */
void drawGame(const std::array<u_int64_t, 16> &board);

void startScreen();

void endScreen();

