#include "objects.h"
#include "debug.h"
#include <cstdint>

Game::Game()
{
    white_turn = true;
    game_over = false;
    turnNum = 1;

    setupLookups();

    game.board[WHITE_PAWNS] = 0x000000000000FF00;
    game.board[WHITE_BISHOPS] = 0x0000000000000024;
    game.board[WHITE_ROOKS] = 0x0000000000000081;
    game.board[WHITE_KNIGHTS] = 0x0000000000000042;
    game.board[WHITE_QUEEN] = 0x0000000000000008;
    game.board[WHITE_KING] = 0x0000000000000010;

    game.board[BLACK_PAWNS] = 0x00FF000000000000;
    game.board[BLACK_BISHOPS] = 0x2400000000000000;
    game.board[BLACK_ROOKS] = 0x8100000000000000;
    game.board[BLACK_KNIGHTS] = 0x4200000000000000;
    game.board[BLACK_QUEEN] = 0x0800000000000000;
    game.board[BLACK_KING] = 0x1000000000000000;

    game.white_castling_rights = {true, true, true};
    game.black_castling_rights = {true, true, true};
    game.enPassantSquare = -1;

    startScreen();
    runGame();
    endScreen();

}

Game::~Game(){};

void Game::runGame()
{
    MoveState state;

    while (!game_over)
    {
        Log("Turn: ", debugLog);
        Log(turnNum, debugLog);
        Log(" (", debugLog);
        Log((white_turn ? "white" : "black"), debugLog);
        Log(") \n", debugLog);

        Log("Turn: ", passantLog);
        Log(turnNum, passantLog);
        Log(" (", passantLog);
        Log((white_turn ? "white" : "black"), passantLog);
        Log(") \n", passantLog);
        Log(" En Passant Square = ", passantLog);
        Log(game.enPassantSquare, passantLog);
        Log("\n", passantLog);

        /*
            Need to update logic so castling works
        */

        Log("Turn: ", castlingLog);
        Log(turnNum, castlingLog);
        Log(" (", castlingLog);
        Log((white_turn ? "white" : "black"), castlingLog);
        Log(") \n", castlingLog);
        Log(" Castling rights = ", castlingLog);
        Log(getCastlingRights(white_turn ? true : false), castlingLog);
        Log("\n", castlingLog);

        clearScreen();
        printGame();
        moves.clear();
        getMoves(moves, white_turn);
        printMoves(moves);
        int piece = 0;
        int destination = 0;
        std::string p, d;
        bool inputValid = false;

        while (!inputValid)
        {
            Log("Waiting for input ... ", debugLog);
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
                Log("Valid Input\n", debugLog);
            }
            catch(const std::exception& e)
            {
                std::cerr << "Invalid Input" << '\n';
                Log(" Invalid Input\n", debugLog);
            }
        }
        Log("Calling updateGame():\n", debugLog);
        state = updateGame(piece, destination);
        if (state == CHECKMATE | state == STALEMATE)
        {
            game_over = true;
        }
    }
    clearScreen();
    std::cout << "Game over - " << ((state == CHECKMATE)? "Checkmate" : "Stalemate") << std::endl;
    Log((state == CHECKMATE)? "Checkmate" : "Stalemate", debugLog);
    std::cin.get();
}

bool operator == (Move m1, Move m2)
{
    if ((m1.from == m2.from) && (m1.to == m2.to))
    {
        return true;
    }
    else
    {
        return false;
    }
}

[[nodiscard]] MoveState Game::updateGame(const int p, const int d)
{
    Move move = {p,d, DEFAULT};
    std::vector<Move> legalMoves;
    Log("\tCalling getMoves()\n", debugLog);
    getMoves(legalMoves, white_turn);

    Log("\tTesting moves against possible moves\n", debugLog);
    for (auto& m : legalMoves)
    {
        if (m == move)
        {
            Log("\tTrying move\n", debugLog);
            GameState game_copy = makeMove(m);
            if (isKinginCheck(white_turn))
            {
                std::cerr << "King in Check" << std::endl;
                Log("\t\tKing in Check\n", debugLog);
                unmakeMove(game_copy);
                break;
            }
            else
            {
                white_turn = !white_turn;
                if (white_turn)
                {
                    turnNum++;
                }
                Log("\t\tValid Move\n", debugLog);
                /*
                Okay so this is checking the loss conditions for
                the side has had a move made against it
                */
                return checkGameEnd(white_turn);
            }
        }
    }
    return ERROR;
}

bool Game::isKinginCheck(bool isWhite)
{
    std::vector<Move> enemyMoves;
    getMoves(enemyMoves, !isWhite);
    uint64_t king = isWhite ? game.board[WHITE_KING] : game.board[BLACK_KING];
    int kingSquare = __builtin_ctzll(king);

    for (auto m : enemyMoves)
    {
        if (m.to == kingSquare)
        {
            return true;
        }
    }
    return false;
}

[[nodiscard]] GameState Game::makeMove(Move m)
{
    GameState game_copy = game;
    uint64_t to_bit = 1Ull << m.to;
    uint64_t from_bit = 1Ull << m.from;

    // THis works
    if (m.type == PAWN_DBL_PUSH)
    {
        game.enPassantSquare = m.to;
    }
    else
    {
        game.enPassantSquare = -1;
    }

    // This removes the pawn that got en passanted
    if (m.type == EN_PASSANT)
    {
        if (white_turn)
        {
            game.board[BLACK_PAWNS] &= ~(1ULL<<(m.to+8));
        }
        else
        {
            game.board[WHITE_PAWNS] &= ~(1ULL<<(m.to-8));
        }
    }

    for (auto& x : game.board)
    {
        if (x & from_bit)
        {
            x &= ~from_bit; // Unplaces piece
            x |= to_bit; // Places piece
            for (auto& y : game.board)
            {
                if (&y != &x)
                {
                    y &= ~to_bit; // Unplaces rival piece
                    // Maybe where en passant stuff dies
                }
            }
            break;
        }
    }
    return game_copy;
}

void Game::unmakeMove(GameState g)
{
    game = g;
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
        if ((binary_location & game.board[piece]) != 0) return piece;
    }

    std::cerr << " This shouldnt Happen, You have problem if this prints" << std::endl;
    return -1;
}

uint64_t Game::getWhitePieces() const
{
    return game.board[WHITE_PAWNS] | game.board[WHITE_BISHOPS] |
           game.board[WHITE_ROOKS] | game.board[WHITE_KNIGHTS] |
           game.board[WHITE_QUEEN] | game.board[WHITE_KING];
}
uint64_t Game::getBlackPieces() const
{
    return game.board[BLACK_PAWNS] | game.board[BLACK_BISHOPS] |
           game.board[BLACK_ROOKS] | game.board[BLACK_KNIGHTS] |
           game.board[BLACK_QUEEN] | game.board[BLACK_KING];
}
uint64_t Game::getAllPieces() const
{
    return getWhitePieces() | getBlackPieces();
}

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
    uint64_t pawns = isWhite ? game.board[WHITE_PAWNS] : game.board[BLACK_PAWNS];
    uint64_t enemyPieces = isWhite ? getBlackPieces() : getWhitePieces();
    uint64_t empty = ~getAllPieces();

    int row_shift = isWhite ? 8 : -8;

    // Should refactor later cos this func takes "bool isWhite"
    if (game.enPassantSquare != -1)
    {
        int sq = game.enPassantSquare; // Square which can be attacked via En Passant
        int y = sq / 8; // Row of the square that the pawn double pushed to
        int x = sq % 8; // Column of the square that the pawn double pushed to

        int dx[] = {-1,1};

        if (sq > -1)
        {
            for (auto d : dx) // for each dx
            {
                int new_x = x + d;

                if ((new_x >= 0) & (new_x < 8)) // check if x+dx is bounded
                {
                    int index = (8*y) + new_x; // calculate index of square
                    if (pawns & (1ULL << index)) // If enemy pawn is present at index
                    {
                        m.push_back(Move{index, sq+row_shift, EN_PASSANT});
                    }
                }
            }
        }
    }

    if (isWhite)
    {
        uint64_t pawn_pushes = (pawns << 8) & empty;
        uint64_t pp = pawn_pushes;
        uint64_t dpp = ((pawn_pushes & RANK_3) << 8) & empty;
        while (pp)
        {
            int to = __builtin_ctzll(pp);
            m.push_back(Move{to-8, to, DEFAULT});
            pp &= pp - 1;
        }
        while (dpp)
        {
            int to = __builtin_ctzll(dpp);
            m.push_back(Move{to-16, to, PAWN_DBL_PUSH});
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
            m.push_back(Move{to+8, to, DEFAULT});
            pp &= pp - 1;
        }
        while (dpp)
        {
            int to = __builtin_ctzll(dpp);
            m.push_back(Move{to+16, to, PAWN_DBL_PUSH});
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
            m.push_back(Move{from, to, DEFAULT});
            attacks &= attacks - 1;                // clear that destination bit
        }
        pawns &= pawns - 1;
    }
}

void Game::getKingMoves(std::vector<Move>& m, bool isWhite)
{
    uint64_t king = isWhite ? game.board[WHITE_KING] : game.board[BLACK_KING];
    uint64_t ownPieces = isWhite ? getWhitePieces() : getBlackPieces();

    while (king)
    {
        int from = __builtin_ctzll(king);
        uint64_t attacks = kingAttacks[from] & ~ownPieces;

        while (attacks)
        {
            int to = __builtin_ctzll(attacks);
            m.push_back(Move{from, to, DEFAULT});
            attacks &= attacks - 1;                // clear that destination bit
        }
        king &= king - 1;
    }
}

void Game::getKnightMoves(std::vector<Move>& m, bool isWhite)
{
    uint64_t knights = isWhite ? game.board[WHITE_KNIGHTS] : game.board[BLACK_KNIGHTS];
    uint64_t ownPieces = isWhite ? getWhitePieces() : getBlackPieces();

    while (knights)
    {
        int from = __builtin_ctzll(knights);
        uint64_t attacks = knightAttacks[from] & ~ownPieces;

        while (attacks)
        {
            int to = __builtin_ctzll(attacks);
            m.push_back(Move{from, to, DEFAULT});
            attacks &= attacks - 1;                // clear that destination bit
        }
        knights &= knights - 1;
    }
}

void Game::getRookMoves(std::vector<Move>& m, bool isWhite)
{
    uint64_t rooks = isWhite ? game.board[WHITE_ROOKS] : game.board[BLACK_ROOKS];
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
                    m.push_back(Move{from, index, DEFAULT});
                    break;
                }
                else if (location & ownPieces)
                {
                    break;
                }
                else
                {
                    m.push_back(Move{from, index, DEFAULT});
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
    uint64_t bishops = isWhite ? game.board[WHITE_BISHOPS] : game.board[BLACK_BISHOPS];
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
                    m.push_back(Move{from, index, DEFAULT});
                    break;
                }
                else if (location & ownPieces)
                {
                    break;
                }
                else
                {
                    m.push_back(Move{from, index, DEFAULT});
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
    uint64_t queen = isWhite ? game.board[WHITE_QUEEN] : game.board[BLACK_QUEEN];
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
                    m.push_back(Move{from, index, DEFAULT});
                    break;
                }
                else if (location & ownPieces)
                {
                    break;
                }
                else
                {
                    m.push_back(Move{from, index, DEFAULT});
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
                    m.push_back(Move{from, index, DEFAULT});
                    break;
                }
                else if (location & ownPieces)
                {
                    break;
                }
                else
                {
                    m.push_back(Move{from, index, DEFAULT});
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
        int y = sq / 8;
        int x = sq % 8;
        uint64_t attacks = 0;

        for (int m = 0; m < 8; m++)
        {
            int nx = x + dx[m];
            int ny = y + dy[m];

            if (ny >= 0 && ny < 8 && nx >= 0 && nx < 8)
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
        int y = sq / 8;
        int x = sq % 8;
        uint64_t attacks = 0;

        for (int m = 0; m < 8; m++)
        {
            int nx = x + dx[m];
            int ny = y + dy[m];

            if (ny >= 0 && ny < 8 && nx >= 0 && nx < 8)
            {
                int index = nx + (ny*8);
                attacks |= (1ULL << index);
            }

        }
        kingAttacks[sq] = attacks;
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

            if (ny >= 0 && ny < 8 && nx >= 0 && nx < 8)
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

            if (ny >= 0 && ny < 8 && nx >= 0 && nx < 8)
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
    getCastlingMoves(m, isWhite);
}
void Game::getAllMoves(std::vector<Move>& m)
{
    getMoves(m, true);
    getMoves(m, false);
}

MoveState Game::checkGameEnd(bool isWhiteTurn)
{
    if (checkCheckmate(isWhiteTurn))
    {
        return CHECKMATE;
    }
    else if (checkStalemate(isWhiteTurn))
    {
        return STALEMATE;
    }

    return VALID_MOVE;
}

// All stuff under here isnt finished

/**
 * @param bitboard
 * @return returns board stalemate status
 */
bool Game::checkStalemate(bool isWhiteTurn)
{
    if (isKinginCheck(isWhiteTurn))
    {
        return false;
    }

    std::vector<Move> moves_cpy;
    getMoves(moves_cpy, isWhiteTurn);
    bool legal_move = false;

    for (auto& m : moves_cpy)
    {
        GameState game_copy = makeMove(m);

        if (!isKinginCheck(isWhiteTurn))
        {
            legal_move = true;
            unmakeMove(game_copy);
            break;

        }
        unmakeMove(game_copy);
    }

    if (legal_move)
    {
        return false;
    }

    return true;
}

/**
 * @param bitboard
 * @return returns board checkmate status
 */
bool Game::checkCheckmate(bool isWhiteTurn)
{

    if (!isKinginCheck(isWhiteTurn))
    {
        return false;
    }

    std::vector<Move> moves_cpy;
    getMoves(moves_cpy, isWhiteTurn);
    bool legal_move = false;

    for (auto& m : moves_cpy)
    {
        GameState game_copy = makeMove(m);

        if (!isKinginCheck(isWhiteTurn))
        {
            legal_move = true;
            unmakeMove(game_copy);
            break;

        }
        unmakeMove(game_copy);
    }

    if (legal_move)
    {
        return false;
    }

    return true;
}

MoveType Game::getCastlingRights(bool isWhite)
{
    CastlingRights rights = isWhite ? game.white_castling_rights : game.black_castling_rights;

    if (!rights.KingFirstMove)
    {
        return NO_CASTLE;
    }
    else if (rights.LongRookFirstMove && rights.ShortRookFirstMove)
    {
        return B_CASTLE;
    }
    else if (rights.LongRookFirstMove && !rights.ShortRookFirstMove)
    {
        return Q_CASTLE;
    }
    else if (!rights.LongRookFirstMove && rights.ShortRookFirstMove)
    {
        return K_CASTLE;
    }

    return NO_CASTLE; // If both rooks have moved
}


void Game::getCastlingMoves(std::vector<Move>& m, bool isWhite) // I dont like this function, it seems like it is bloated
{
    uint64_t homeRank = isWhite ? RANK_1 : RANK_8;
    MoveType rights = getCastlingRights(isWhite);
    uint64_t queenCastleBetween = isWhite ? CASTLE_WQ_BETWEEN : CASTLE_BQ_BETWEEN;
    uint64_t kingCastleBetween = isWhite ? CASTLE_WK_BETWEEN : CASTLE_BK_BETWEEN;



    if (!isKinginCheck(isWhite))
    {
        return;
    }

    switch (rights)
    {
        // Need to check is a square it attacked
        case B_CASTLE:
            if (!(queenCastleBetween && getAllPieces()))// Checking path is clear
            {
                if (!(queenCastleBetween && getTargetedSqaures(m))) // Check that none of the squares it passes through are attacked
                {
                    m.push_back(Move(Q_CASTLE)); // Need to change this, this wont work with my current move selection
                }
            }
            if (!(kingCastleBetween & getAllPieces()))
            {
                if (!(kingCastleBetween && getTargetedSqaures(m)))
                {
                    m.push_back(Move(K_CASTLE));
                }
            }
            return;
        case K_CASTLE:
            if (!(kingCastleBetween & getAllPieces()))
            {
                if (!(kingCastleBetween && getTargetedSqaures(m)))
                {
                    m.push_back(Move(K_CASTLE));
                }
            }
            return;
        case Q_CASTLE:
            if (!(queenCastleBetween && getAllPieces()))
            {
                if (!(queenCastleBetween && getTargetedSqaures(m)))
                {
                    m.push_back(Move(Q_CASTLE));
                }
            }
            return;
        default: return;
    }
}

uint64_t Game::getTargetedSqaures(std::vector<Move>& m) //
{
    // go thru all moves, |= with uint64_t, then this gives a bitbaord of targed squares
    uint64_t targetedSquares = 0ULL;
    for (auto& move : m)
    {
        targetedSquares |= 1ULL << move.to;
    }
    return targetedSquares;
}
