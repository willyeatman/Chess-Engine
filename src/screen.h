#pragma once

#include <iostream>
#include <inttypes.h>
#include <array>

/**
 * @brief Clears the screen
 * 
 */
void clear_screen();

/**
 * @brief Prints out the game
 * 
 * @param board array of bitboards 
 */
void print_game(const std::array<u_int64_t, 16> &board);

