#include <iostream>
#include <string>
#include "objects.h"
#include "screen.h"
#include "debug.h"


int main() {

    Log("Starting Signal Handler ... ", debugLog);
    signal(SIGINT, signalHandler); 
    Log("Okay\n", debugLog);

    Game Chess;

    startScreen();
    Chess.runGame();
    endScreen();

    return 0;
}