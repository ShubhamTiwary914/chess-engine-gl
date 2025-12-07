#ifndef GUIUTILS_H
#define GUIUTILS_H

#include <SDL2/SDL.h>
#include <filesystem>


struct boardPos {
    char file;  // a-h
    int rank;   // 1-8 
};

struct selectedPiece {
    char piece;
    char file;
    int rank;
    //selected piece's moves
    uint64_t moves;  
    bool isSelected;
};


//file: [0,7], rank: [0,7]
void setSquareCoords(SDL_Rect &obj, int sqBase, int file, int rank);
std::string getFullPath(std::string subPath="");
const char* getFullPath(const char* path);
bool isInvalidPos(boardPos pos);

int keyBoardPress(SDL_Event &ev);
boardPos mouseClickedSquare(SDL_Event &ev, int sqBase);


#endif