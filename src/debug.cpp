#include "debug.h"

std::ofstream debugLog("docs/log.txt", std::ios::trunc);

#ifdef DEBUG

    void printMoves(std::vector<Move> &moves)
    {
        int count = 0;
        Log("Move\n");
        for (auto& m : moves)
        {
            Log("\t");
            Log(++count);
            Log(":\n");
            Log("\tFrom ");
            Log(squareNames[m.from]);
            Log(" -> ");
            Log(squareNames[m.to]);
            Log('\n');
        }
    }
#endif
