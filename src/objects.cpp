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

    setupLookups();

    while (!game_over)
    {
        clearScreen();
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


uint64_t Game::getWhitePieces() const
{
    return board[WHITE_PAWNS] | board[WHITE_BISHOPS] |
           board[WHITE_ROOKS] | board[WHITE_KNIGHTS] |
           board[WHITE_QUEEN] | board[WHITE_KING];
}
uint64_t Game::getBlackPieces() const
{
    return board[BLACK_PAWNS] | board[BLACK_BISHOPS] |
           board[BLACK_ROOKS] | board[BLACK_KNIGHTS] |
           board[BLACK_QUEEN] | board[BLACK_KING];
}
uint64_t Game::getAllPieces() const
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

void Game::getPawnMoves(std::vector<Move>& m, bool isWhite)
{
    uint64_t pawns = isWhite ? board[WHITE_PAWNS] : board[BLACK_PAWNS];
    uint64_t enemyPieces = isWhite ? getBlackPieces() : getWhitePieces();
    uint64_t empty = ~getAllPieces();


    if (isWhite)
    {   
        uint64_t pawn_pushes = (pawns << 8) & empty;
        uint64_t pp = pawn_pushes;
        uint64_t dpp = ((pawn_pushes & RANK_3) << 8) & empty;
        while (pp)
        {
            int to = __builtin_ctzll(pp);
            m.push_back(Move{to-8, to, 0});
            pp &= pp - 1;  
        }
        while (dpp)
        {
            int to = __builtin_ctzll(dpp);
            m.push_back(Move{to-16, to, 0});
            dpp &= dpp - 1;  
        }
    }
    else
    {
        uint64_t pawn_pushes = (pawns >> 8) & empty;
        uint64_t pp = pawn_pushes;
        uint64_t dpp = ((pawn_pushes & RANK_6) >> 8) & empty;
        while (pp)
        {
            int to = __builtin_ctzll(pp);
            m.push_back(Move{to+8, to, 0});
            pp &= pp - 1;  
        }
        while (dpp)
        {
            int to = __builtin_ctzll(dpp);
            m.push_back(Move{to+16, to, 0});
            dpp &= dpp - 1;  
        }
    }

    while (pawns)
    {
        int from = __builtin_ctzll(pawns);
        uint64_t attacks = pawnAttacks[isWhite][from] & enemyPieces;
        
        while (attacks)
        {
            int to = __builtin_ctzll(attacks);
            m.push_back(Move{from, to, 0});
            attacks &= attacks - 1;                // clear that destination bit
        }
        pawns &= pawns - 1; 
    }
}

void Game::getKingMoves(std::vector<Move>& m, bool isWhite)
{
    uint64_t king = isWhite ? board[WHITE_KING] : board[BLACK_KING];
    uint64_t ownPieces = isWhite ? getWhitePieces() : getBlackPieces();

    while (king)
    {
        int from = __builtin_ctzll(king);
        uint64_t attacks = kingAttacks[from] & ~ownPieces;
        
        while (attacks)
        {
            int to = __builtin_ctzll(attacks);
            m.push_back(Move{from, to, 0});
            attacks &= attacks - 1;                // clear that destination bit
        }
        king &= king - 1; 
    }
}

void Game::getKnightMoves(std::vector<Move>& m, bool isWhite)
{
    uint64_t knights = isWhite ? board[WHITE_KNIGHTS] : board[BLACK_KNIGHTS];
    uint64_t ownPieces = isWhite ? getWhitePieces() : getBlackPieces();

    while (knights)
    {
        int from = __builtin_ctzll(knights);
        uint64_t attacks = knightAttacks[from] & ~ownPieces;
        
        while (attacks)
        {
            int to = __builtin_ctzll(attacks);
            m.push_back(Move{from, to, 0});
            attacks &= attacks - 1;                // clear that destination bit
        }
        knights &= knights - 1; 
    }
}

void Game::getRookMoves(std::vector<Move>& m, bool isWhite)
{
    uint64_t rooks = isWhite ? board[WHITE_ROOKS] : board[BLACK_ROOKS];
    uint64_t enemyPieces = isWhite ? getBlackPieces() : getWhitePieces();
    uint64_t ownPieces = isWhite ? getWhitePieces() : getBlackPieces();

    Directions directions[] = {NORTH, EAST, SOUTH, WEST};

    while (rooks)
    {
        int from = __builtin_ctzll(rooks);
        int x = from % 8;
        int y = from / 8;
        for (auto d : directions)
        {
            int dx;
            int dy;
            switch (d)
            {
            case NORTH:
                dx = 0;
                dy = 1;
                break;
            case EAST:
                dx = 1;
                dy = 0;
                break;
            case SOUTH:
                dx = 0;
                dy = -1;
                break;
            case WEST:
                dx = -1;
                dy = 0;
                break;
            default:
                std::cout << "getRookMoves() Error - Switch" << std::endl;
                throw;
            }
            int nx = x + dx;
            int ny = y + dy;

            while (nx >= 0 && nx < 8 && ny >= 0 && ny < 8)
            {
                int index = (8*ny) + nx;
                uint64_t location = 1ULL << index;
                if (location & enemyPieces)
                {   
                    m.push_back(Move{from, index, 0});
                    break;
                } 
                else if (location & ownPieces)
                {
                    break;
                }
                else
                {
                    m.push_back(Move{from, index, 0});
                }
                nx += dx;
                ny += dy;
            }
        }
        rooks &= rooks - 1; 
    }
}

void Game::getBishopMoves(std::vector<Move>& m, bool isWhite)
{
    uint64_t bishops = isWhite ? board[WHITE_BISHOPS] : board[BLACK_BISHOPS];
    uint64_t enemyPieces = isWhite ? getBlackPieces() : getWhitePieces();
    uint64_t ownPieces = isWhite ? getWhitePieces() : getBlackPieces();

    Directions directions[] = {NORTH_EAST, NORTH_WEST_KAYNE_REFERENCE_MAYBE, SOUTH_EAST, SOUTH_WEST};

    while (bishops)
    {
        int from = __builtin_ctzll(bishops);
        int x = from % 8;
        int y = from / 8;
        for (auto d : directions)
        {
            int dx;
            int dy;
            switch (d)
            {
            case NORTH_EAST:
                dx = 1;
                dy = 1;
                break;
            case NORTH_WEST_KAYNE_REFERENCE_MAYBE:
                dx = -1;
                dy = 1;
                break;
            case SOUTH_EAST:
                dx = 1;
                dy = -1;
                break;
            case SOUTH_WEST:
                dx = -1;
                dy = -1;
                break;
            default:
                std::cout << "getBishopMoves() Error - Switch" << std::endl;
                throw;
            }
            int nx = x + dx;
            int ny = y + dy;

            while (nx >= 0 && nx < 8 && ny >= 0 && ny < 8)
            {
                int index = (8*ny) + nx;
                uint64_t location = 1ULL << index;
                if (location & enemyPieces)
                {   
                    m.push_back(Move{from, index, 0});
                    break;
                } 
                else if (location & ownPieces)
                {
                    break;
                }
                else
                {
                    m.push_back(Move{from, index, 0});
                }
                nx += dx;
                ny += dy;
            }
        }
        bishops &= bishops - 1; 
    }
}

void Game::getQueenMoves(std::vector<Move>& m, bool isWhite)
{
    uint64_t queen = isWhite ? board[WHITE_QUEEN] : board[BLACK_QUEEN];
    uint64_t enemyPieces = isWhite ? getBlackPieces() : getWhitePieces();
    uint64_t ownPieces = isWhite ? getWhitePieces() : getBlackPieces();

    Directions directions_r[] = {NORTH, EAST, SOUTH, WEST};
    Directions directions_b[] = {NORTH_EAST, NORTH_WEST_KAYNE_REFERENCE_MAYBE, SOUTH_EAST, SOUTH_WEST};

    uint64_t queen_copy = queen;
    while (queen_copy)
    {
        int from = __builtin_ctzll(queen_copy);
        int x = from % 8;
        int y = from / 8;
        for (auto d : directions_r)
        {
            int dx;
            int dy;
            switch (d)
            {
            case NORTH:
                dx = 0;
                dy = 1;
                break;
            case EAST:
                dx = 1;
                dy = 0;
                break;
            case SOUTH:
                dx = 0;
                dy = -1;
                break;
            case WEST:
                dx = -1;
                dy = 0;
                break;
            default:
                std::cout << "getRookMoves() Error - Switch" << std::endl;
                throw;
            }
            int nx = x + dx;
            int ny = y + dy;

            while (nx >= 0 && nx < 8 && ny >= 0 && ny < 8)
            {
                int index = (8*ny) + nx;
                uint64_t location = 1ULL << index;
                if (location & enemyPieces)
                {   
                    m.push_back(Move{from, index, 0});
                    break;
                } 
                else if (location & ownPieces)
                {
                    break;
                }
                else
                {
                    m.push_back(Move{from, index, 0});
                }
                nx += dx;
                ny += dy;
            }
        }
        queen_copy &= queen_copy - 1; 
    }

    queen_copy = queen;
    while (queen_copy)
    {
        int from = __builtin_ctzll(queen_copy);
        int x = from % 8;
        int y = from / 8;
        for (auto d : directions_b)
        {
            int dx;
            int dy;
            switch (d)
            {
            case NORTH_EAST:
                dx = 1;
                dy = 1;
                break;
            case NORTH_WEST_KAYNE_REFERENCE_MAYBE:
                dx = -1;
                dy = 1;
                break;
            case SOUTH_EAST:
                dx = 1;
                dy = -1;
                break;
            case SOUTH_WEST:
                dx = -1;
                dy = -1;
                break;
            default:
                std::cout << "getBishopMoves() Error - Switch" << std::endl;
                throw;
            }
            int nx = x + dx;
            int ny = y + dy;

            while (nx >= 0 && nx < 8 && ny >= 0 && ny < 8)
            {
                int index = (8*ny) + nx;
                uint64_t location = 1ULL << index;
                if (location & enemyPieces)
                {   
                    m.push_back(Move{from, index, 0});
                    break;
                } 
                else if (location & ownPieces)
                {
                    break;
                }
                else
                {
                    m.push_back(Move{from, index, 0});
                }
                nx += dx;
                ny += dy;
            }
        }
        queen_copy &= queen_copy - 1; 
    }
}

void Game::setupKnightLookup()
{
    const int dx[] = {-1, -1, -2, -2, 2, 2, 1, 1};
    const int dy[] = {-2, 2, 1, -1, 1, -1, 2, -2};

    for (int sq = 0; sq < 64; sq++)
    {
        int y = sq % 8;
        int x = sq / 8;
        uint64_t attacks = 0;

        for (int m = 0; m < 8; m++)
        {
            int nx = x + dx[m];
            int ny = y + dy[m];

            if (ny >= 0 && ny < 8 && nx >= 0 && ny < 8)
            {
                int index = nx + (ny*8);
                attacks |= (1ULL << index);
            }

        }
        knightAttacks[sq] = attacks;
    }
}
void Game::setupKingLookup()
{
    const int dx[] = {0, 0, 1, 1, 1, -1, -1, -1};
    const int dy[] = {-1, 1, 1, -1, 0, 1, -1, 0};

    for (int sq = 0; sq < 64; sq++)
    {
        int y = sq % 8;
        int x = sq / 8;
        uint64_t attacks = 0;

        for (int m = 0; m < 8; m++)
        {
            int nx = x + dx[m];
            int ny = y + dy[m];

            if (ny >= 0 && ny < 8 && nx >= 0 && ny < 8)
            {
                int index = nx + (ny*8);
                attacks |= (1ULL << index);
            }

        }
        knightAttacks[sq] = attacks;
    }
}
void Game::setupPawnLookups()
{
    const int dx_b[] = {1, -1};
    const int dy_b[] = {-1, -1}; 

    const int dx_w[] = {1, -1};
    const int dy_w[] = {1, 1}; 

    for (int sq = 0; sq < 64; sq++)
    {

        int y = sq % 8;
        int x = sq / 8;
        uint64_t attacks = 0;

        for (int m = 0; m < 2; m++)
        {
            int nx = x + dx_b[m];
            int ny = y + dy_b[m];

            if (ny >= 0 && ny < 8 && nx >= 0 && ny < 8)
            {
                int index = nx + (ny*8);
                attacks |= (1ULL << index);
            }

        }
        pawnAttacks[0][sq] = attacks;
    }

    for (int sq = 0; sq < 64; sq++)
    {
        int y = sq % 8;
        int x = sq / 8;
        uint64_t attacks = 0;

        for (int m = 0; m < 2; m++)
        {
            int nx = x + dx_w[m];
            int ny = y + dy_w[m];

            if (ny >= 0 && ny < 8 && nx >= 0 && ny < 8)
            {
                int index = nx + (ny*8);
                attacks |= (1ULL << index);
            }

        }
        pawnAttacks[1][sq] = attacks;
    }
}
void Game::setupLookups()
{
    setupKnightLookup();
    setupKingLookup();
    setupPawnLookups();
}

void Game::getMoves(std::vector<Move>& m, bool isWhite) // Must Be called after setup lookups
{
    getPawnMoves(m, isWhite);
    getKingMoves(m, isWhite);
    getQueenMoves(m, isWhite);
    getRookMoves(m, isWhite);
    getBishopMoves(m, isWhite);
    getKnightMoves(m, isWhite);
}
void Game::getAllMoves(std::vector<Move>& m)
{
    getMoves(m, true);
    getMoves(m, false);
}