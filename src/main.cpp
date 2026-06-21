#include <iostream>
#include <string>
#include "objects.h"
#include "screen.h"

int main() {
    // This will now successfully register because screen.h is included above it
    signal(SIGINT, signalHandler); 
    Game game;
    startScreen();
    game.runGame();
    endScreen();

    return 0;
}
