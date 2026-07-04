#pragma once
#include <fstream>
#include "objects.h"

extern std::ofstream debugLog;

#ifdef DEBUG
    template <typename T>
    void Log(const T& msg) 
    {
        debugLog << msg;
        debugLog.flush();
    }
    void printMoves(std::vector<Move> &moves);
#else
    template <typename T>
    inline void Log(const T&) {}
    inline void printMoves(std::vector<Move>&) {}
#endif
