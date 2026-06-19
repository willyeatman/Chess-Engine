#include "screen.h"

void drawGame(const std::array<u_int64_t, 16> &board)
{

}

void startScreen()
{
    std::cout << "\033[?1049h";
    std::cout << "\033[H\033[2J" << std::flush;
}

void endScreen()
{
    std::cout << "\e[? 1049l" << std::flush; 
    std::cout << "\033[2J" << std::flush;
}
