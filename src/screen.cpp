#include "screen.h"

void clear_screen()
{
    std::cout << "\033[2J\033[H";
}


void print_game(const std::array<u_int64_t, 16> &board)
{

}