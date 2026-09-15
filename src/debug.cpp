#include "debug.h"

std::ofstream debugLog("docs/log.txt", std::ios::trunc);
std::ofstream moveLog("docs/moves.txt", std::ios::trunc);
std::ofstream passantLog("docs/enpassant.txt", std::ios::trunc);
std::ofstream castlingLog("docs/castling.txt", std::ios::trunc);

#ifdef DEBUG

    void printMoves(std::vector<Move> &moves)
    {
        int count = 0;
        
        Log("Move:", moveLog);
        for (auto& m : moves)
        {
            Log("\t", moveLog);
            Log(++count, moveLog);
            Log(":\n", moveLog);
            Log("\tFrom ", moveLog);
            Log(squareNames[m.from], moveLog);
            Log(" -> ", moveLog);
            Log(squareNames[m.to], moveLog);
            Log(" Type = ", moveLog);
            Log(m.type, moveLog);
            Log('\n', moveLog);
        }
    }

#endif
