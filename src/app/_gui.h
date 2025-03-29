#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <iostream>
#include <cassert>
#include <unordered_map>
#include <vector>

#include "engine/boards.h"
#include "utils/gui.h"
#include "utils/engine.h"


//main window - chess
extern SDL_Window *window;
extern SDL_Renderer *screen;
extern SDL_Rect baseCoords;
extern std::unordered_map<char, SDL_Texture *> textures;
extern int sqBase;
extern int scaleX;
extern int scaleY;
const double pieceSpriteGap = 0.45;
//logger window
extern std::vector<SDL_Window *> logWindows;
extern std::vector<SDL_Renderer *> logScreens;
extern TTF_Font* font;
extern int fontSize;
extern std::string fontFamily;
extern bool LOG_MODE;



int initSDL();
void exitGame();
void resizeHandler();
void preLoadTextures(std::unordered_map<char, int> &piecesCharMap);
void updateScreen();



void renderBoard(selectedPiece &selected);
void renderPieces(PieceList &mainboard);

//log window manager 
int createLogWindow(const char* title, int x, int y, int w, int h);
void renderStringToWindow(int windowIndex, std::string log);
void closeLogWindow(int windowIndex);
void closeAllLogWindows();