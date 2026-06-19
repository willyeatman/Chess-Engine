/**
 * @file objects.h
 * @author William Yeatman (will.yeatman24@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2026-06-19
 * 
 * @copyright Copyright (c) 2026
 * 
 */


#pragma once

#include <inttypes.h>
#include <array>
#include <string>

typedef enum 
{
    //  White pieces
    WHITE_PAWN, WHITE_ROOK, WHITE_KNIGHT, WHITE_QUEEN,
    WHITE_KING, WHITE_BISHOP,

    //  Black pieces
    BLACK_PAWN, BLACK_ROOK, BLACK_KNIGHT, BLACK_QUEEN,
    BLACK_KING, BLACK_BISHOP

} Pieces;

class Game
{
    public:
        Game();
        ~Game();
        std::string updateGame(const std::string& move);
        std::string getGame();

    private:
        static int games;
        std::array<uint64_t,12> board;
        uint64_t white_pieces;
        uint64_t black_pieces;
        uint64_t all_pieces;
        bool white_turn;
};
