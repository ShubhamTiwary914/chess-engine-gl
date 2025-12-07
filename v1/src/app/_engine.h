#ifndef ENGINEMAIN_H
#define ENGINEMAIN_H

#include <iostream>
#include <cstring>
#include <string>
#include "engine/boards.h"
#include "engine/state.h"
#include "engine/moves.h"
#include "utils/gui.h"
#include "utils/engine.h"

extern BitBoardSet whiteboard;
extern BitBoardSet blackboard;
extern PieceList mainboard;
extern movesSetStruct movesSet;
extern GameState state;


void initializeBoard();
//select piece & request to move(1)
int selectPiece(boardPos pos, selectedPiece &selected, bool turn);

/// @brief once move selected, move piece
/// @details handle captures, assess checks, etc.
void movePiece(char fromFile, int fromRank, char toFile, int toRank,
    PieceList &mainboard, BitBoardSet &whiteboard, BitBoardSet &blackboard);





#endif