#pragma once
#include <fstream>
#include "objects.h"

extern std::ofstream debugLog;
extern std::ofstream moveLog;

#ifdef DEBUG
    template <typename T>
    void Log(const T& msg) 
    {
        debugLog << msg;
        debugLog.flush();
    }
    template <typename T>
    void Log2(const T& msg)
    {
        // this function is making me angry :((((
        moveLog << msg;
        moveLog.flush();
    }
    void printMoves(std::vector<Move> &moves);
#else
    template <typename T>
    inline void Log(const T&) {}
    template <typename T>
    inline void Log2(const T&);
    inline void printMoves(std::vector<Move>&) {}
#endif
