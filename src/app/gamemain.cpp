#include "gamemain.h"

selectedPiece selected = {'\0', 'a', 1, 0, false};
int selectedSquare = -1;

int boardLogWindow = -1;
int stateLogWindow = -1;


void initializeGame(){
    //LOG_MODE = true;
    //engine startup.
    initializeBoard();
    //gui
    initSDL();
    preLoadTextures(piecesCharMap);
    
    //log windows create 
    boardLogWindow = createLogWindow("Board State", baseCoords.x + baseCoords.w*2, baseCoords.y + baseCoords.h/2, baseCoords.w, baseCoords.h*1.5);     
    stateLogWindow = createLogWindow("Game State", baseCoords.x + baseCoords.w*2, baseCoords.y + baseCoords.h*2.2, baseCoords.w, baseCoords.h);
    
    renderBoard(selected);
    renderPieces(mainboard);
    updateScreen();

    logManager();
    //event-loop & end
    gameCycle();
    exitGame();
}

void gameCycle(){
    bool running = true;
    SDL_Event ev;
    boardPos pos;
    while (running) {
        while (SDL_PollEvent(&ev) != 0) {
            if (ev.type == SDL_QUIT) {
                running = false;
            }
            //resize window
            if(ev.type == SDL_WINDOWEVENT && ev.window.event == SDL_WINDOWEVENT_RESIZED) {
                // std::cout << "resize" << std::endl;
                resizeHandler();
                renderBoard(selected);
                renderPieces(mainboard);
                updateScreen();
            }
            else if(ev.type == SDL_KEYDOWN){
                if(!keyBoardPress(ev))
                    running = false;
            }
            else if(ev.type == SDL_MOUSEBUTTONDOWN){
                mouseClicker(pos, ev);
            }
        }
    }
}



void mouseClicker(boardPos &pos, SDL_Event &ev){
    pos = mouseClickedSquare(ev, sqBase);
    //in bounds
    if(!isInvalidPos(pos)){
        //ask to move (if selected in last)
        if(selectPiece(pos, selected, state.turn)){         
            movePiece(selected.file, selected.rank, pos.file, pos.rank, mainboard, whiteboard, blackboard);
            state.move_state("k");
            clearSelection(selected);
        }
    }
    //out of bounds: un-select
    else
        clearSelection(selected);
    renderBoard(selected);
    renderPieces(mainboard); 

    if(LOG_MODE)
        logManager();
    updateScreen();
}


void logManager(){
    if (boardLogWindow >= 0)
    {
        U64 fullBits = blackboard.getUnion() | whiteboard.getUnion();
        renderStringToWindow(boardLogWindow,
            printBitBoard(blackboard.getUnion(), "BlackBoard", false, 'b') + 
            printBitBoard(whiteboard.getUnion(), "\n\nWhiteBoard", false, 'w') 
        );
    }

    if (stateLogWindow >= 0)
    {
        std::string castlingBits = printBitsU8(state.castling);
        renderStringToWindow(stateLogWindow,
            "Castling White:  " + castlingBits + "\n\n" + "Left Castling: " + intToString(state.checkCastlingAvailable(WHITE_TURN, true)) 
            + "\nRight Castling: " + intToString(state.checkCastlingAvailable(WHITE_TURN, false)) + "\n\n" +
            "\n\n\nCastling Black:  " + castlingBits + "\n\n" + "Left Castling: " + intToString(state.checkCastlingAvailable(BLACK_TURN, true)) + 
            "\nRight Castling: " + intToString(state.checkCastlingAvailable(BLACK_TURN, false)) + "\n\n"
        );
    }
}