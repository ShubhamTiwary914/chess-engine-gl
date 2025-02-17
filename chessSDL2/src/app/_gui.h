#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <cassert>
#include <unordered_map>

#include "engine/boards.h"
#include "utils/gui.h"
#include "utils/engine.h"



extern SDL_Window *window;
extern SDL_Renderer *screen;
extern SDL_Rect baseCoords;
extern std::unordered_map<char, SDL_Texture *> textures;
extern int sqBase;
extern int scaleX;
extern int scaleY;
const double pieceSpriteGap = 0.45;



int initSDL();
void exitGame();
void resizeHandler();
void preLoadTextures(std::unordered_map<char, int> &piecesCharMap);
void updateScreen();



void renderBoard(selectedPiece &selected);
void renderPieces(PieceList &mainboard);
void movePiece(char fromFile, int fromRank, char toFile, int toRank,
    PieceList &mainboard, BitBoardSet &whiteboard, BitBoardSet &blackboard);





