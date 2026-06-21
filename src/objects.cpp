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

void Game::runGame()
{
    while (!game_over)
    {
        printGame();
        int piece = 0;
        int destination = 0;
        std::string p, d;
        bool inputValid = false;

        while (!inputValid)
        {

            std::cin >> p >> d;
            

            for (int i = 0; i < 2; i++)
            {
                p[i] = toupper(p[i]);
                d[i] = toupper(d[i]);
            }
            try
            {
                piece = square_map.at(p);
                destination = square_map.at(d);
                inputValid = true;
            }
            catch(const std::exception& e)
            {
                std::cerr << "Invalid Move" << '\n';
            }
        }
        
        updateGame(piece, destination);
    }
}

void Game::updateGame(const int p, const int d)
{
    uint64_t destination = 1Ull << d;
    uint64_t peice = 1Ull << p;

    if (!(destination & getAllPieces()))
    {
        for (int i = WHITE_KING; i < PIECE_COUNT; i++)
        {
            if (peice & board[i])
            {
                board[i] ^= peice;
                board[i] |= destination;
                break;
            }
        }
    }
}

void Game::printGame()
{
    if(white_turn)
    {
        for (int y = 7; y >= 0; --y)
        {
            std::cout << ' '<< (y+1) << ' ';
            for (int x = 0; x < 8; ++x)
            {
                int index = (8*y) + x;
                bool isLight = ((x + y) % 2) != 0;
                std::cout << (isLight ? BG_LIGHT : BG_DARK);
                printPiece(index);
            }
            std::cout << "\n";
        } 
        std::cout << "    a  b  c  d  e  f  g  h\n";
    }
    else
    {
        for (int y = 0; y <= 7; ++y)
        {
            std::cout << ' '<< (y+1) << ' ';
            for (int x = 7; x >= 0; --x)
            {
                int index = (8*y) + x;
                bool isLight = ((x + y) % 2) != 0;
                std::cout << (isLight ? BG_LIGHT : BG_DARK);
                printPiece(index);
            }
            std::cout << "\n";
        } 
        std::cout << "    h  g  f  e  d  c  b  a\n";
    }


}

int Game::getPiece(int index) const
{
    uint64_t binary_location = 1ULL << index;

    bool occupied =  (binary_location & getAllPieces()) != 0;
    if (!occupied)
    {
        return -1;
    }
    for (int piece = WHITE_KING; piece < PIECE_COUNT; piece++)
    {
        if ((binary_location & board[piece]) != 0) return piece;
    }
    
    std::cerr << " This shouldnt Happen, You have problem if this prints" << std::endl;
    return -1;
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

void Game::printPiece(int index) const
{
    int piece = getPiece(index);

    if (piece == -1) 
    {
        std::cout << "   ";
        std::cout << RESET;
        return;
    }
    bool light = piece < 6;
    std::cout << (light ? FG_WHITE : FG_BLACK);
    std::cout << " " << piece_chars[piece] << " ";
    std::cout << RESET;
}

