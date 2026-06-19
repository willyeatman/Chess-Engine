#include "screen.h"

void signalHandler(int sig) {
    const char msg[] = "Caught SIGINT\n";
    const char exit[] = "\e[?1049l";
    write(STDOUT_FILENO, exit, sizeof(exit) - 1);
    write(STDOUT_FILENO, msg, sizeof(msg) - 1);
    _exit(sig);
}

void drawGame(const std::array<u_int64_t, 16> &board)
{

};

void startScreen()
{
    // 1. Enter the alternate screen buffer
    // 2. Move cursor to home (0,0) and clear the alternate screen
    const char enter_alt[] = "\e[?1049h";
    write(STDOUT_FILENO, enter_alt, sizeof(enter_alt) - 1);
 
}

void endScreen()
{
    const char exit_alt[] = "\e[?1049l";
    write(STDOUT_FILENO, exit_alt, sizeof(exit_alt) - 1);
}
