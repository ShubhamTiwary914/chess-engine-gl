#include <gtest/gtest.h>
#include <utils/engine.h>
#include <utils/consts.h>
#include <engine/moves.h>
#include <app/_engine.h>
#include <cassert>
#include "./force.cpp"


void check();




/****************************************\

    Utils: bitwise, engine

\*****************************************/

//*Rank Mask
TEST(Utils, RankMask){
    for(int rank=1; rank<=8; rank++){
        EXPECT_EQ(getRank_bruteForce(rank), getRankBits(rank));
    }
}

//*File Mask
TEST(Utils, FileMask){
    for(char file='a'; file<='h'; file++){
        EXPECT_EQ(getFile_bruteForce(file), getFileBits(file));
    }
}


//*Diagonal Left
TEST(Utils, DiagonalLeftMask){
    // a1 --> a8
    char file = 'a';
    for(int rank=1; rank<=8; rank++){
        for(char file='a'; file<='h'; file++){
            EXPECT_EQ(getDiagonalLeftBits(file, rank), getDiagonalLeft_bruteForce(file, rank));
        }
    }
}

//*Diagonal Right
TEST(Utils, DiagonalRightMask){
    // a1 --> a8
    char file = 'a';
    for(int rank=1; rank<=8; rank++){
        for(char file='a'; file<='h'; file++){
            EXPECT_EQ(getDiagonalRightBits(file, rank), getDiagonalRight_bruteForce(file, rank));
        }
    }
}


//*Edges
TEST(Utils, EdgesMask){
    EXPECT_EQ(getEdgesMask_bruteForce(), getEdgesMask());
}




/****************************************\

        Moves for pieces

\*****************************************/








void printFancy(uint64_t board, std::string title){
    std::cout << "\n--------" + title + " --------\n";
    printBitBoard(board);
    std::cout << std::endl;
}




int main(int argc, char **argv) {
    check();
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

void check() {
    std::cout << "\n\n";


    char file = 'a';
    int rank = 3;


    //D3 rook
    uint64_t d3rook = getRookMoves(file, rank);
    printFancy(d3rook, "A3");


    //Blockers
    std::vector<uint64_t> blockers = generateBlockerConfigurations(d3rook);
    printFancy(blockers[100], "Blocker[100]");

    printFancy(getRookAttacks(file, rank, blockers[100]), "A3 attacks");


    std::cout << "\n\n";
}
