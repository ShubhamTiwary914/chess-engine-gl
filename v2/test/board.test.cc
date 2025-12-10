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
    engine::CachedSet cache;
    engine::moves_precompute(&cache);
    int r = 4;
    int f = 3;
    utils::setBit64(
        &B.bitboard[WHITE_INDEX][KNIGHT_INDEX],
        utils::getLERFIndex(r, f)
    );
    engine::clearBoardSet(&B);
    utils::setBit64(&B.bitboard[WHITE_INDEX][KNIGHT_INDEX],
                    utils::getLERFIndex(r, f));
    // u64 m1 = engine::movesGeneration(&B, r, f, WHITE_INDEX, KNIGHT_INDEX);
    u64 m1 = engine::moves_fastfetch(&cache, &B, r, f, 
        WHITE_INDEX, KNIGHT_INDEX);
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
    engine::CachedSet cache;
    engine::moves_precompute(&cache);
    utils::setBit64(&B.bitboard[WHITE_INDEX][KNIGHT_INDEX],
                    utils::getLERFIndex(r, f));
    utils::setBit64(&B.bitboard[WHITE_INDEX][PAWN_INDEX],
                    utils::getLERFIndex(6,4));
    utils::setBit64(&B.bitboard[WHITE_INDEX][PAWN_INDEX],
                    utils::getLERFIndex(3,1));
    u64 m2 = engine::moves_fastfetch(&cache, &B, r, f, WHITE_INDEX, KNIGHT_INDEX);
    u64 e2 =
        (1ULL << utils::getLERFIndex(6,2)) |
        (1ULL << utils::getLERFIndex(5,5)) |
        (1ULL << utils::getLERFIndex(5,1)) |
        (1ULL << utils::getLERFIndex(3,5)) |
        (1ULL << utils::getLERFIndex(2,4)) |
        (1ULL << utils::getLERFIndex(2,2)); 
    EXPECT_EQ(m2, e2);
}


TEST(Moves, KingMoves_NoBlocks) {
    engine::BoardSet B;
    engine::CachedSet cache;
    engine::moves_precompute(&cache);
    engine::clearBoardSet(&B);
    int r = 4, f = 4;
    utils::setBit64(&B.bitboard[WHITE_INDEX][KING_INDEX],
                    utils::getLERFIndex(r,f));
    u64 m = engine::moves_fastfetch(&cache, &B, r, f,
                                   WHITE_INDEX, KING_INDEX);
    u64 e =
        (1ULL << utils::getLERFIndex(5,4)) |
        (1ULL << utils::getLERFIndex(5,5)) |
        (1ULL << utils::getLERFIndex(4,5)) |
        (1ULL << utils::getLERFIndex(3,5)) |
        (1ULL << utils::getLERFIndex(3,4)) |
        (1ULL << utils::getLERFIndex(3,3)) |
        (1ULL << utils::getLERFIndex(4,3)) |
        (1ULL << utils::getLERFIndex(5,3));
    EXPECT_EQ(m, e);
}

TEST(Moves, KingMoves_SideBlocks) {
    engine::BoardSet B;
    engine::CachedSet cache;
    engine::moves_precompute(&cache);
    engine::clearBoardSet(&B);
    int r = 4, f = 4;
    utils::setBit64(&B.bitboard[WHITE_INDEX][KING_INDEX],
                    utils::getLERFIndex(r,f));
    utils::setBit64(&B.bitboard[WHITE_INDEX][PAWN_INDEX],
                    utils::getLERFIndex(5,4));
    utils::setBit64(&B.bitboard[WHITE_INDEX][PAWN_INDEX],
                    utils::getLERFIndex(3,3));
    u64 m = engine::moves_fastfetch(&cache, &B, r, f,
                                   WHITE_INDEX, KING_INDEX);
    u64 e =
        (1ULL << utils::getLERFIndex(5,5)) |
        (1ULL << utils::getLERFIndex(4,5)) |
        (1ULL << utils::getLERFIndex(3,5)) |
        (1ULL << utils::getLERFIndex(3,4)) |
        (1ULL << utils::getLERFIndex(4,3)) |
        (1ULL << utils::getLERFIndex(5,3));
    EXPECT_EQ(m, e);
}


TEST(Moves, KingMoves_OppKingNearby) {
    engine::BoardSet B;
    engine::CachedSet cache;
    engine::moves_precompute(&cache);
    engine::clearBoardSet(&B);
    int r = 4, f = 4;
    utils::setBit64(&B.bitboard[WHITE_INDEX][KING_INDEX],
                    utils::getLERFIndex(r,f));
    utils::setBit64(&B.bitboard[BLACK_INDEX][KING_INDEX],
                    utils::getLERFIndex(5,5));
    u64 m = engine::moves_fastfetch(&cache, &B, r, f,
                                   WHITE_INDEX, KING_INDEX);
    // remove opponent king square + all adjacent squares to opponent king
    u64 e =
        (1ULL << utils::getLERFIndex(3,5)) |
        (1ULL << utils::getLERFIndex(3,4)) |
        (1ULL << utils::getLERFIndex(3,3)) |
        (1ULL << utils::getLERFIndex(4,3)) |
        (1ULL << utils::getLERFIndex(5,3));
    EXPECT_EQ(m, e);
}


TEST(Moves, PawnMoves_NoBlocks_StarterRank_White) {
    engine::BoardSet B;
    engine::CachedSet cache;
    engine::moves_precompute(&cache);
    engine::clearBoardSet(&B);
    int r = 1, f = 4;  // white pawn starting rank
    utils::setBit64(&B.bitboard[WHITE_INDEX][PAWN_INDEX],
                    utils::getLERFIndex(r,f));
    u64 m = engine::moves_fastfetch(&cache,&B,r,f,
                                    WHITE_INDEX,PAWN_INDEX);
    u64 e = 0;
    e |= 1ULL << utils::getLERFIndex(2,4);
    e |= 1ULL << utils::getLERFIndex(3,4);
    EXPECT_EQ(m,e);
}

TEST(Moves, PawnMoves_NoBlocks_StarterRank_Black) {
    engine::BoardSet B;
    engine::CachedSet cache;
    engine::moves_precompute(&cache);
    engine::clearBoardSet(&B);
    int r = 6, f = 4;  // black pawn starting rank
    utils::setBit64(&B.bitboard[BLACK_INDEX][PAWN_INDEX],
                    utils::getLERFIndex(r,f));
    u64 m = engine::moves_fastfetch(&cache,&B,r,f,
                                    BLACK_INDEX,PAWN_INDEX);
    u64 e = 0;
    e |= 1ULL << utils::getLERFIndex(5,4);
    e |= 1ULL << utils::getLERFIndex(4,4);
    EXPECT_EQ(m,e);
}

TEST(Moves, PawnMoves_Blocked_White) {
    engine::BoardSet B;
    engine::CachedSet cache;
    engine::moves_precompute(&cache);
    engine::clearBoardSet(&B);
    int r = 1, f = 4;
    utils::setBit64(&B.bitboard[WHITE_INDEX][PAWN_INDEX],
                    utils::getLERFIndex(r,f));
    utils::setBit64(&B.bitboard[WHITE_INDEX][KNIGHT_INDEX],
                    utils::getLERFIndex(2,4));  // block
    u64 m = engine::moves_fastfetch(&cache,&B,r,f,
                                    WHITE_INDEX,PAWN_INDEX);
    u64 e = 0;
    EXPECT_EQ(m,e);
}

TEST(Moves, PawnMoves_Blocked_Black) {
    engine::BoardSet B;
    engine::CachedSet cache;
    engine::moves_precompute(&cache);
    engine::clearBoardSet(&B);
    int r = 6, f = 4;
    utils::setBit64(&B.bitboard[BLACK_INDEX][PAWN_INDEX],
                    utils::getLERFIndex(r,f));

    utils::setBit64(&B.bitboard[BLACK_INDEX][KNIGHT_INDEX],
                    utils::getLERFIndex(5,4));  // block
    u64 m = engine::moves_fastfetch(&cache,&B,r,f,
                                    BLACK_INDEX,PAWN_INDEX);
    u64 e = 0;
    EXPECT_EQ(m,e);
}


TEST(Moves, PawnMoves_Capture_White) {
    engine::BoardSet B;
    engine::CachedSet cache;
    engine::moves_precompute(&cache);
    engine::clearBoardSet(&B);
    int r = 3, f = 3;
    utils::setBit64(&B.bitboard[WHITE_INDEX][PAWN_INDEX],
                    utils::getLERFIndex(r,f));

    utils::setBit64(&B.bitboard[BLACK_INDEX][PAWN_INDEX],
                    utils::getLERFIndex(4,4));
    utils::setBit64(&B.bitboard[BLACK_INDEX][KNIGHT_INDEX],
                    utils::getLERFIndex(4,2));
    u64 m = engine::moves_fastfetch(&cache,&B,r,f,
                                    WHITE_INDEX,PAWN_INDEX);
    u64 e = 0;
    e |= 1ULL << utils::getLERFIndex(4,3);  // forward
    e |= 1ULL << utils::getLERFIndex(4,4);  // capture right
    e |= 1ULL << utils::getLERFIndex(4,2);  // capture left
    EXPECT_EQ(m,e);
}

TEST(Moves, PawnMoves_Capture_Black) {
    engine::BoardSet B;
    engine::CachedSet cache;
    engine::moves_precompute(&cache);
    engine::clearBoardSet(&B);
    int r = 4, f = 3;
    utils::setBit64(&B.bitboard[BLACK_INDEX][PAWN_INDEX],
                    utils::getLERFIndex(r,f));
    utils::setBit64(&B.bitboard[WHITE_INDEX][PAWN_INDEX],
                    utils::getLERFIndex(3,4));
    utils::setBit64(&B.bitboard[WHITE_INDEX][KNIGHT_INDEX],
                    utils::getLERFIndex(3,2));
    u64 m = engine::moves_fastfetch(&cache,&B,r,f,
                                    BLACK_INDEX,PAWN_INDEX);
    u64 e = 0;
    e |= 1ULL << utils::getLERFIndex(3,3);  // forward
    e |= 1ULL << utils::getLERFIndex(3,4);  // capture right
    e |= 1ULL << utils::getLERFIndex(3,2);  // capture left
    EXPECT_EQ(m,e);
}
