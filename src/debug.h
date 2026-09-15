#pragma once
#include <fstream>
#include "objects.h"

extern std::ofstream debugLog;
extern std::ofstream moveLog;
extern std::ofstream passantLog;
extern std::ofstream castlingLog;

#ifdef DEBUG
    template <typename T>
    void Log(const T& msg, std::ostream& stream) 
    {
        stream << msg;
        stream.flush();
    }
    void printMoves(std::vector<Move> &moves);
#else
    template <typename T>
    inline void Log(const T&, std::ostream&) {}
    inline void printMoves(std::vector<Move>&) {}
#endif
