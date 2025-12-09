#include <gtest/gtest.h>
#include "../lib/board.h"
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