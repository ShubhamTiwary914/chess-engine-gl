#include "gamemain.h"



selectedPiece selected = {'\0', 'a', 1, 0, false};
int selectedSquare = -1;



void initializeGame(){
    //engine startup.
    initializeBoard();
    //gui
    initSDL();
    preLoadTextures(piecesCharMap);
    renderBoard(selected);
    renderPieces(mainboard);
    updateScreen();
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

    U64 fullBits = blackboard.getUnion() | whiteboard.getUnion();
    renderString(
        printBitBoard(blackboard.getUnion(), "BlackBoard", false, 'b')
        + "\n\n" +
        printBitBoard(whiteboard.getUnion(), "WhiteBoard", false, 'w')
    );
    updateScreen();
}