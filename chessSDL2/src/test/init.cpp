#include <gtest/gtest.h>
//
#include <utils/consts.h>
#include <utils/engine.h>
#include <engine/moves.h>
#include <cassert>
#include <app/_engine.h>
//
#include "./force.h"
#include "./init.h"

//>utils
//*Rank Mask
TEST(Utils, RankMask)
{
    for (int rank = 1; rank <= 8; rank++)
    {
        EXPECT_EQ(getRank_bruteForce(rank), getRankBits(rank));
    }
}

//*File Mask
TEST(Utils, FileMask)
{
    for (char file = 'a'; file <= 'h'; file++)
    {
        EXPECT_EQ(getFile_bruteForce(file), getFileBits(file));
    }
}

//*Diagonal Left
TEST(Utils, DiagonalLeftMask)
{
    // a1 --> a8
    char file = 'a';
    for (int rank = 1; rank <= 8; rank++)
    {
        for (char file = 'a'; file <= 'h'; file++)
        {
            EXPECT_EQ(getDiagonalLeftBits(file, rank), getDiagonalLeft_bruteForce(file, rank));
        }
    }
}

//*Diagonal Right
TEST(Utils, DiagonalRightMask)
{
    // a1 --> a8
    char file = 'a';
    for (int rank = 1; rank <= 8; rank++)
    {
        for (char file = 'a'; file <= 'h'; file++)
        {
            EXPECT_EQ(getDiagonalRightBits(file, rank), getDiagonalRight_bruteForce(file, rank));
        }
    }
}

//*Edges
TEST(Utils, EdgesMask)
{
    EXPECT_EQ(getEdgesMask_bruteForce(), getEdgesMask());
}

//>Moves

int main(int argc, char **argv)
{
    check();
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}




uint64_t testingRookMask(char file, int rank){
    initializeBoard();
    uint64_t blockers = 0ULL;
    setBit(blockers, getPieceBitIndex('b', 4));
    setBit(blockers, getPieceBitIndex('d', 7));
    setBit(blockers, getPieceBitIndex('d', 8));
    printBitBoard(blockers, "Blockers: ");
    std::cout << std::endl;

    uint64_t rookD4 = 0ULL; setBit(rookD4, getPieceBitIndex('d', 4));
    printBitBoard(rookD4, "rook at d4: ");

    uint64_t forwardBits = getForwardBitsMask('d', 4);
    printBitBoard(forwardBits, "Forward Bits for d4: ");

    uint64_t rookmask = getRookMask(file, rank, false);
    rookmask &= forwardBits;
    printBitBoard(rookmask, "Rook Mask Forward: ");

    uint64_t diff = (blockers - 2*rookD4);
    printBitBoard(diff, "Difference: ");

    uint64_t forwardRes = (diff ^ (blockers | rookD4)) & rookmask;
    printBitBoard(forwardRes, "Forward's Attack Pattern for RD4: ");
    return 0ULL;
}


// uint64_t testingRookMask(char file, int rank){
//     initializeBoard();
//     uint64_t empty_board = 0ULL;

//     uint64_t blockers = empty_board;
//     setBit(blockers, getPieceBitIndex('b', 4));  
//     setBit(blockers, getPieceBitIndex('d', 7));
//     setBit(blockers, getPieceBitIndex('d', 8));
//     uint64_t rookBit = empty_board; setBit(rookBit, getPieceBitIndex(file,rank));

//     printBitBoard(rookBit, "Rook at d4"); 
//     printBitBoard(blockers, "Blockers: ");
//     uint64_t difference = blockers - 2*rookBit;
//     printBitBoard(difference, "Difference: ");

//     uint64_t change = difference ^ blockers;
//     printBitBoard(change, "Change: ");

//     uint64_t rookMask = getRookMask(file, rank, false); 
//     setBit(rookMask, getPieceBitIndex(file,rank));  
//     printBitBoard(rookMask, "Rook Mask: ");

//     uint64_t attack = change & rookMask;
//     printBitBoard(attack, "Attack Path: ");

//     return 0ULL;
// }






void check()
{
    testingRookMask('d', 4);   
    std::cout << "\n\n";
}
