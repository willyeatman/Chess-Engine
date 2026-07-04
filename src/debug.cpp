#include "debug.h"

std::ofstream debugLog("docs/log.txt", std::ios::trunc);

#ifdef DEBUG

    void printMoves(std::vector<Move> &moves)
    {
        int count = 0;
        for (auto& m : moves)
        {
            Log("Move: ");
            Log(++count);
            Log("\n");
            Log("\tFrom ");
            Log(m.from);
            Log(" -> ");
            Log(m.to);
            Log('\n');
        }
    }
#endif
