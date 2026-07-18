#include "debug.h"

std::ofstream debugLog("docs/log.txt", std::ios::trunc);
std::ofstream moveLog("docs/moves.txt", std::ios::trunc);

#ifdef DEBUG

    void printMoves(std::vector<Move> &moves)
    {
        int count = 0;
        
        Log2("Move:");
        for (auto& m : moves)
        {
            Log2("\t");
            Log2(++count);
            Log2(":\n");
            Log2("\tFrom ");
            Log2(squareNames[m.from]);
            Log2(" -> ");
            Log2(squareNames[m.to]);
            Log2(" Type = ");
            Log2(m.type);
            Log2('\n');
        }
    }

#endif
