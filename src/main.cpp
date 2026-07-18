#include <iostream>
#include <string>
#include "objects.h"
#include "screen.h"
#include "debug.h"

int main() {
    // This will now successfully register because screen.h is included above it

    Log("Starting Signal Handler ... ");
    signal(SIGINT, signalHandler); 
    Log("Okay\n");

    Game Chess;

    startScreen();
    Chess.runGame();
    endScreen();

    return 0;
}