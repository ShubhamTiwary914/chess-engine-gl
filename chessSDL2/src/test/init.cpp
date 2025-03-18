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
    printBitBoard(whiteboard.getUnion(), "");
    return 0ULL;
}




void check()
{
    testingRookMask('d', 4);   
    std::cout << "\n\n";
}
