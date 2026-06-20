#include "objects.h"


Game::Game()
{
    white_turn = 1;
    game_over = false;

    board[WHITE_PAWNS] = 0x000000000000FF00;
    board[WHITE_BISHOPS] = 0x0000000000000024;
    board[WHITE_ROOKS] = 0x0000000000000081;
    board[WHITE_KNIGHTS] = 0x0000000000000042;
    board[WHITE_QUEEN] = 0x0000000000000008;
    board[WHITE_KING] = 0x0000000000000010;

    board[BLACK_PAWNS] = 0x00FF000000000000;
    board[BLACK_BISHOPS] = 0x2400000000000000;
    board[BLACK_ROOKS] = 0x8100000000000000; 
    board[BLACK_KNIGHTS] = 0x4200000000000000;
    board[BLACK_QUEEN] = 0x0800000000000000;
    board[BLACK_KING] = 0x1000000000000000; 

}
Game::~Game(){};

void Game::updateGame(const std::string& move)
{

}

void Game::printGame()
{
    for (int y = 7; y >= 0; --y)
    {
        std::cout << ' '<< (y+1) << ' ';
        for (int x = 0; x < 8; ++x)
        {
            int index = (8*y) + x;
            bool isLight = ((x + y) % 2) != 0;
            std::cout << (isLight ? BG_LIGHT : BG_DARK);

            std::cout << RESET;
        }
        std::cout << "\n";
    } 
    std::cout << "    a  b  c  d  e  f  g  h\n";
}

const int Game::getPiece(int& index) const
{
    uint64_t binary_location = 1ULL << index;

    bool occupied =  (binary_location & getAllPieces()) != 0;

    if (!occupied)
    {
        return -1;
    }
    
    return -1; // <-remove this 
}


const uint64_t Game::getWhitePieces() const
{
    return board[WHITE_PAWNS] | board[WHITE_BISHOPS] |
           board[WHITE_ROOKS] | board[WHITE_KNIGHTS] |
           board[WHITE_QUEEN] | board[WHITE_KING];
}
const uint64_t Game::getBlackPieces() const
{
    return board[BLACK_PAWNS] | board[BLACK_BISHOPS] |
           board[BLACK_ROOKS] | board[BLACK_KNIGHTS] |
           board[BLACK_QUEEN] | board[BLACK_KING];
}
const uint64_t Game::getAllPieces() const
{
    return getWhitePieces() | getBlackPieces();
}

/*
        static int games;
        std::array<uint64_t,12> board;
        uint64_t white_pieces;
        uint64_t black_pieces;
        uint64_t all_pieces;
        bool white_turn;
        bool game_over = false;
*/
