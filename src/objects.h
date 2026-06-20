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
    WHITE_KING, WHITE_QUEEN, WHITE_ROOKS, WHITE_BISHOPS,
    WHITE_KNIGHTS, WHITE_PAWNS,

    //  Black pieces
    BLACK_KING, BLACK_QUEEN, BLACK_ROOKS, BLACK_BISHOPS,
    BLACK_KNIGHTS, BLACK_PAWNS,

} Pieces;

constexpr const char* pieceGlyphs[12] = {
    "\u2654", "\u2655", "\u2656", "\u2657", "\u2658", "\u2659",
    "\u265A", "\u265B", "\u265C", "\u265D", "\u265E", "\u265F"
};

class Game
{
    public:
        Game();
        ~Game();
        void updateGame(const std::string& move);
        void printGame();
    private:
        std::array<uint64_t,12> board;
        uint64_t white_pieces;
        uint64_t black_pieces;
        uint64_t all_pieces;
        bool white_turn;
        bool game_over;
        const uint64_t& getWhitePieces() const;
        const uint64_t& getBlackPieces() const;
        const uint64_t& getAllPieces() const;
};
