#ifndef ENGINEMAIN_H
#define ENGINEMAIN_H

#include <iostream>
#include <cstring>
#include <string>
#include "./boards.h"



std::string FENstart = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

BitBoardSet whiteboard;
BitBoardSet blackboard;
PieceList mainboard;

int chessEngine(){
    return 0;
}


void initializeBoard(){
    FENParser(FENstart, whiteboard, blackboard);
    mainboard.translateBitboard_toPieceList(whiteboard, blackboard);
    mainboard.printBoard();
    // mainboard.printBoard();
    //set Bitboards
    //set PieceList.
    // generate zobrist tables & keys.
    //set new Game state.
}


void makeTurn(){
    
}





#endif