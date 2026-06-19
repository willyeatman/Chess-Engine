#include <iostream>
#include <string>
#include "objects.h"
#include "screen.h"


/*int main()
{
   startScreen();
} */

// todo signal handeling

int main() {
    // 1. Enter the alternate screen buffer
    std::cout << "\033[?1049h";
    
    // 2. Move cursor to home (0,0) and clear the alternate screen
    std::cout << "\033[H\033[2J" << std::flush;

    // 3. Render your full-screen UI
    std::cout << "Welcome to the C++ Alternate Screen Buffer!\n";
    std::cout << "Your primary terminal history is safely hidden.\n";
    std::cout << "Exiting in 3 seconds...\n" << std::flush;

    // 4. Exit the alternate screen buffer (restores original terminal state)
    //std::cout << "\033[?1049l" << std::flush;
    std::cin.get();
    std::cout << "\e[?1049l" << std::flush; 
    std::cout << "\033[2J" << std::flush;
    return 0;
}