#include <gtest/gtest.h>



// Example test case
#include "lib/boards.h"


//Piece set & unsets
BitBoardSet TESTBOARD;
TEST(PieceSet, setPiece) {
    for(int piece=0; piece<6; piece++){
        for(int rank=8; rank>=1; rank--){
            for(int file=65; file<= 72; file++){
                //check order: set, check, unset, check
                TESTBOARD.setPiece(piece, file, rank);
                EXPECT_EQ(TESTBOARD.checkBitBoard(piece, file, rank), 1);

                TESTBOARD.unsetPiece(piece, file, rank);
                EXPECT_EQ(TESTBOARD.checkBitBoard(piece, file, rank), 0);
            }
        }
    }
}

//Place all non-empty pieces (5x2) in all possible places & check 
PieceList TEST_MAINBOARD;
TEST(FullBoard, setBoardPiece){
    char pieces[] = {'K','Q','N','P','R','k','q','p','r','n'};
    for(int piece=0; piece<10; piece++){
        for(int rank=8; rank>=1; rank--){
            for(int file=65; file<= 72; file++){
                TEST_MAINBOARD.setPiece(pieces[piece], file, rank);
                EXPECT_EQ(TEST_MAINBOARD.getPiece(file, rank), pieces[piece]);

                TEST_MAINBOARD.unsetPiece(file, rank);
                EXPECT_EQ(TEST_MAINBOARD.getPiece(file, rank), 'E');
            }
        }
    }
}




int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}