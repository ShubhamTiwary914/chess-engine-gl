#include <gtest/gtest.h>
#include <utils/engine.h>
#include <utils/consts.h>
#include <engine/moves.h>
#include <cassert>
#include "./force.cpp"


void check();

//>utils
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




//>Moves





int main(int argc, char **argv) {
    check();
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}




uint64_t getRookMask(char file, int rank) {
    uint64_t mask = 0ULL;
    int square = getPieceBitIndex(file, rank);
    
    // Add rank bits
    for (char f = 'a'; f <= 'h'; f++) {
        if (f != file) {  // Exclude the piece's square
            setBit(mask, getPieceBitIndex(f, rank));
        }
    }
    
    // Add file bits
    for (int r = 1; r <= 8; r++) {
        if (r != rank) {  // Exclude the piece's square
            setBit(mask, getPieceBitIndex(file, r));
        }
    }
    
    // Remove edges
    mask &= ~getEdgesMask();
    return mask;
}


void check() {
    std::cout << "\n\n";
    printBitBoard(getRookMask('c', 4));
    std::cout << "\n\n";
}