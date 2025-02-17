#ifndef ENGINEMAIN_H
#define ENGINEMAIN_H

#include <iostream>
#include <cstring>
#include <string>
#include "engine/boards.h"
#include "engine/moves.h"
#include "utils/gui.h"
#include "utils/engine.h"



extern BitBoardSet whiteboard;
extern BitBoardSet blackboard;
extern PieceList mainboard;
extern movesSetStruct movesSet;


int chessEngine();
void initializeBoard();
//select piece & request to move(1)
int selectPiece(boardPos pos, selectedPiece &selected, int &turn);





#endif