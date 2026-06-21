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
#include <iostream>
#include <string>
#include <cmath>
#include <unordered_map>

typedef enum 
{
    //  White pieces
    WHITE_KING, WHITE_QUEEN, WHITE_ROOKS, WHITE_BISHOPS,
    WHITE_KNIGHTS, WHITE_PAWNS,

    //  Black pieces
    BLACK_KING, BLACK_QUEEN, BLACK_ROOKS, BLACK_BISHOPS,
    BLACK_KNIGHTS, BLACK_PAWNS,

    PIECE_COUNT

} Pieces;

typedef enum
{

    A1, B1, C1, D1, E1, F1, G1, H1,
    A2, B2, C2, D2, E2, F2, G2, H2,
    A3, B3, C3, D3, E3, F3, G3, H3,
    A4, B4, C4, D4, E4, F4, G4, H4,
    A5, B5, C5, D5, E5, F5, G5, H5,
    A6, B6, C6, D6, E6, F6, G6, H6,
    A7, B7, C7, D7, E7, F7, G7, H7,
    A8, B8, C8, D8, E8, F8, G8, H8

} Squares;

const std::unordered_map<std::string, Squares> square_map =
{
    {"A1", A1}, {"B1", B1}, {"C1", C1}, {"D1", D1}, {"E1", E1}, {"F1", F1}, {"G1", G1}, {"H1", H1},
    {"A2", A2}, {"B2", B2}, {"C2", C2}, {"D2", D2}, {"E2", E2}, {"F2", F2}, {"G2", G2}, {"H2", H2},
    {"A3", A3}, {"B3", B3}, {"C3", C3}, {"D3", D3}, {"E3", E3}, {"F3", F3}, {"G3", G3}, {"H3", H3},
    {"A4", A4}, {"B4", B4}, {"C4", C4}, {"D4", D4}, {"E4", E4}, {"F4", F4}, {"G4", G4}, {"H4", H4},
    {"A5", A5}, {"B5", B5}, {"C5", C5}, {"D5", D5}, {"E5", E5}, {"F5", F5}, {"G5", G5}, {"H5", H5},
    {"A6", A6}, {"B6", B6}, {"C6", C6}, {"D6", D6}, {"E6", E6}, {"F6", F6}, {"G6", G6}, {"H6", H6},
    {"A7", A7}, {"B7", B7}, {"C7", C7}, {"D7", D7}, {"E7", E7}, {"F7", F7}, {"G7", G7}, {"H7", H7},
    {"A8", A8}, {"B8", B8}, {"C8", C8}, {"D8", D8}, {"E8", E8}, {"F8", F8}, {"G8", G8}, {"H8", H8},
};

constexpr const char* piece_chars[12] = {
    "K","Q","R","B","N","P",   // white king queen rook bishop knight pawn
    "k","q","r","b","n","p"    // black king queen rook bishop knight pawn
};

constexpr auto BG_LIGHT = "\033[48;2;181;136;99m";   // warm light brown
constexpr auto BG_DARK  = "\033[48;2;140;100;55m";   // medium dark brown — lighter so black pieces show up
constexpr auto FG_WHITE = "\033[38;2;255;255;255m";  // white piece
constexpr auto FG_BLACK = "\033[38;2;0;0;0m";        // black piece
constexpr auto RESET    = "\033[0m";
constexpr auto FG_RESET = "\033[39m";


class Game
{
    public:
        Game();
        ~Game();
        void updateGame(const int p, const int d);
        void printGame();
        void runGame();
    private:
        std::array<uint64_t,12> board;
        uint64_t white_pieces;
        uint64_t black_pieces;
        uint64_t all_pieces;
        bool white_turn;
        bool game_over;
        const uint64_t getWhitePieces() const;
        const uint64_t getBlackPieces() const;
        const uint64_t getAllPieces() const;
        int getPiece(int index) const;
        void printPiece(int index) const;
};
