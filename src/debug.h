#pragma once
#include <fstream>

extern std::ofstream debugLog;

#ifdef DEBUG
    template <typename T>
    void Log(const T& msg) 
    {
        debugLog << msg;
        debugLog.flush();
    }
#else
    template <typename T>
    inline void Log(const T&) {}
#endif
