#include "screen.h"

void signalHandler(int sig) {
    const char msg[] = "Caught SIGINT\n";
    const char exit[] = "\e[?1049l";
    write(STDOUT_FILENO, exit, sizeof(exit) - 1);
    write(STDOUT_FILENO, msg, sizeof(msg) - 1);
    _exit(sig);
}

void startScreen()
{
    // 1. Enter the alternate screen buffer
    // 2. Move cursor to home (0,0) and clear the alternate screen
    constexpr char enter_alt[] = "\e[?1049h\033[2J\033[H";
    write(STDOUT_FILENO, enter_alt, sizeof(enter_alt) - 1);
 
}

void endScreen()
{
    constexpr char exit_alt[] = "\e[?1049l";
    write(STDOUT_FILENO, exit_alt, sizeof(exit_alt) - 1);
}


void clearScreen()
{
    std::cout << "\033[3J\033[2J\033[H" << std::flush;
}