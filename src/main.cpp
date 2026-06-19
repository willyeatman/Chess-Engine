#include <iostream>
#include <string>
#include "objects.h"
#include "screen.h"
#include "logic.h"

int main() {
    // This will now successfully register because screen.h is included above it
    signal(SIGINT, signalHandler); 
    
    startScreen();
    
    while (1)
    {
        // Your CPU sleep loop works perfectly here
        usleep(100000);
    }
    
    //gameLoop();
    endScreen();

    return 0;
}
