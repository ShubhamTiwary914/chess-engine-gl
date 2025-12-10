#include <gtest/gtest.h>
#include "../lib/board.h"
#include "../lib/moves.h"
#include "../lib/bitutills.h"

///@brief generating stringboard with initBoardSet method
TEST(Board, StringBoard_Generation){
    char expectedBoard[8][9] = {
        "RNBKQBNR",
        "PPPPPPPP",
        "xxxxxxxx",
        "xxxxxxxx",
        "xxxxxxxx",
        "xxxxxxxx",
        "pppppppp",
        "rnbkqbnr"
    };
    engine::BoardSet boardset = engine::initBoardSet();
    for(int rank=7; rank>=0; rank--){
        for(int file=0; file<=7; file++){
            EXPECT_EQ(
                boardset.stringboard[rank][file],
                expectedBoard[rank][file]
            );
        }
    }
}

TEST(Moves, KnightMoves_NoBlocks) {
    engine::BoardSet B;
    engine::clearBoardSet(&B);
    int r = 4;
    int f = 3;
    utils::setBit64(
        &B.bitboard[WHITE_INDEX][KNIGHT_INDEX],
        utils::getLERFIndex(r, f)
    );
    engine::clearBoardSet(&B);
    utils::setBit64(&B.bitboard[WHITE_INDEX][KNIGHT_INDEX],
                    utils::getLERFIndex(r, f));
    u64 m1 = engine::movesGeneration(&B, r, f, WHITE_INDEX, KNIGHT_INDEX);
    u64 e1 =
        (1ULL << utils::getLERFIndex(6,4)) |
        (1ULL << utils::getLERFIndex(6,2)) |
        (1ULL << utils::getLERFIndex(5,5)) |
        (1ULL << utils::getLERFIndex(5,1)) |
        (1ULL << utils::getLERFIndex(3,5)) |
        (1ULL << utils::getLERFIndex(3,1)) |
        (1ULL << utils::getLERFIndex(2,4)) |
        (1ULL << utils::getLERFIndex(2,2));
    
    EXPECT_EQ(m1, e1); 
}

TEST(Moves, KnightMoves_SidePiecesBlock){
    engine::BoardSet B;
    engine::clearBoardSet(&B);
    engine::clearBoardSet(&B); 
    int r = 4, f = 3;

    utils::setBit64(&B.bitboard[WHITE_INDEX][KNIGHT_INDEX],
                    utils::getLERFIndex(r, f));
    utils::setBit64(&B.bitboard[WHITE_INDEX][PAWN_INDEX],
                    utils::getLERFIndex(6,4));
    utils::setBit64(&B.bitboard[WHITE_INDEX][PAWN_INDEX],
                    utils::getLERFIndex(3,1));
    u64 m2 = engine::movesGeneration(&B, r, f, WHITE_INDEX, KNIGHT_INDEX);
    u64 e2 =
        (1ULL << utils::getLERFIndex(6,2)) |
        (1ULL << utils::getLERFIndex(5,5)) |
        (1ULL << utils::getLERFIndex(5,1)) |
        (1ULL << utils::getLERFIndex(3,5)) |
        (1ULL << utils::getLERFIndex(2,4)) |
        (1ULL << utils::getLERFIndex(2,2)); 
    EXPECT_EQ(m2, e2);
}
