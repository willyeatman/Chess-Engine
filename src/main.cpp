#include <iostream>
#include <string>
#include "objects.h"
#include "screen.h"

int main() {
    // This will now successfully register because screen.h is included above it
    signal(SIGINT, signalHandler); 
    startScreen();
    std::cout << "frame one\n...board...\n";
    std::cin.get();                       // look

    std::cout << "\033[2J\033[H";         // clear + home — THIS is your repaint
    std::cout << "frame two\n...board...\n";
    std::cin.get(); 

    endScreen();

    return 0;
}
