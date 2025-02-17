#include "./_engine.h"


BitBoardSet whiteboard;
BitBoardSet blackboard;
PieceList mainboard;
movesSetStruct movesSet;


int chessEngine(){
    return 0;
}


void initializeBoard(){
    FENParser(FENstart, whiteboard, blackboard);
    mainboard.translateBitboard_toPieceList(whiteboard, blackboard);
    movesSet = generatePrecomputedMoves();
    initializeMagicTables();
}


//select piece & request to move(1)
int selectPiece(boardPos pos, selectedPiece &selected, int &turn){
    char piece = mainboard.getPiece(pos.file, pos.rank);
    //*selected already -> now move
    if(selected.isSelected) {
        int moveIndex = getPieceBitIndex(pos.file, pos.rank);
        // Check if clicked square is a valid move
        if(getBit(selected.moves, moveIndex)) {
            //>TODO:  switchTurn(turn);
            return 1; //ask to move
        }
    }

    //*click in empty tile or out of bound
    if(piece == 'E' || (selected.isSelected && pos.file == selected.file && pos.rank == selected.rank)) {
        selected.isSelected = false;
        selected.moves = 0;
        return 0;
    }
    
    //*new piece selected -> select but don't move yet
    int pieceType = piecesCharMap[piece];
    selected.piece = piece; 
    selected.file = pos.file; 
    selected.rank = pos.rank;
    //pre-compute, then filter blockages
    selected.moves = getPrecomputedMove(movesSet, pieceType, pos.file, pos.rank);
    selected.moves = filterMoveBlocks(pos.file, pos.rank, selected.moves, pieceType, turn, whiteboard, blackboard);
    selected.isSelected = true;
    //don't move piece
    return 0; 
}