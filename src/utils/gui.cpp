#include "gui.h"



//file: [0,7], rank: [0,7]
void setSquareCoords(SDL_Rect &obj, int sqBase, int file, int rank){
    obj.x = sqBase*file;
    obj.y = sqBase*(8-rank);
}


std::string getFullPath(std::string subPath) {
    return (std::filesystem::current_path() / subPath).string();
}

const char* getFullPath(const char* path) {
    static std::string fullPath = (std::filesystem::current_path() / path).string();
    return fullPath.c_str();
}


int keyBoardPress(SDL_Event &ev){
    switch(ev.key.keysym.sym){
        case SDLK_ESCAPE: //esc
            return 0;
            break;
    }
    return 1;
}



boardPos mouseClickedSquare(SDL_Event &ev, int sqBase) {
    boardPos pos;
    if(ev.type == SDL_MOUSEBUTTONDOWN) {
        int mouseX = ev.button.x;
        int mouseY = ev.button.y;
        int file = mouseX / sqBase;
        int rank = 8 - (mouseY / sqBase);  
        // if click valid (on square)
        if(file >= 0 && file < 8 && rank >= 1 && rank <= 8) {
            pos.file = (char)('a' + file);
            pos.rank = rank;
            return pos;
        }
    }
    //invalid type
    pos.file = '\0';  
    pos.rank = 0;     
    return pos;
}

bool isInvalidPos(boardPos pos) {
    return (pos.file == '\0' || pos.rank == 0);
}