#ifndef GAMEMAIN_H
#define GAMEMAIN_H

#include <iostream>
#include "app/_engine.h"
#include "app/_gui.h"
#include "engine/boards.h"
#include "utils/gui.h"
#include "utils/engine.h"


extern selectedPiece selected;
extern int selectedSquare;
extern int turn;


void initializeGame();
void gameCycle();
void mouseClicker(boardPos &pos, SDL_Event &ev);

void logManager();


#endif