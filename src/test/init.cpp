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

#define breakln std::cout << std::endl


using U64 = uint64_t;


//> utils
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

// //*Diagonal Left
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

// //*Diagonal Right
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
int main(int argc, char **argv){
    check();
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}



void check()
{
    initializeBoard(); 
    std::cout << piecesCharMap['Q'] << std::endl;
    std::cout << piecesCharMap['q'] << std::endl;
    std::cout << piecesCharMap['k'] << std::endl;
    std::cout << piecesCharMap['K'] << std::endl;
}