#include "./_engine.h"


BitBoardSet whiteboard;
BitBoardSet blackboard;
PieceList mainboard;
movesSetStruct movesSet;
GameState state;


void initializeBoard(){
    FENParser(FENstart, whiteboard, blackboard);
    mainboard.translateBitboard_toPieceList(whiteboard, blackboard);
    movesSet = generatePrecomputedMoves();
    initializeMagicTables();
}


//select piece & request to move(1)
int selectPiece(boardPos pos, selectedPiece &selected, bool turn){
    char piece = mainboard.getPiece(pos.file, pos.rank);
    //*selected already -> now move
    if(selected.isSelected) {
        int moveIndex = getPieceBitIndex(pos.file, pos.rank);
        // Check if clicked square is a valid move
        if(getBit(selected.moves, moveIndex)) { 
            return 1; //ask to move
        }
    }
    //*click in piece whose turn is not now
    //lower = black, upper = white
    if( (isLower(piece) && state.turn == WHITE_TURN) || (!isLower(piece) && state.turn == BLACK_TURN) )
        return 0;

    //*click in empty tile or out of bound
    if(piece == 'E' || (selected.isSelected && pos.file == selected.file && pos.rank == selected.rank)) {
        selected.isSelected = false;
        return 0;
    }
    selected.moves = 0;
    
    //> move generation 
    int pieceType = piecesCharMap[piece];
    selected.piece = piece; 
    selected.file = pos.file; 
    selected.rank = pos.rank;
    //pre-compute (only for jumping pieces)
    if(pieceType >= KING && pieceType <= KNIGHT)
        selected.moves = getPrecomputedMove(movesSet, pieceType, pos.file, pos.rank, state.turn);
    //special case -> for pawn (promotions, attacks & moves are different, en passant)
    selected.moves = filterPawnMoves(pos.file, pos.rank, selected.moves, turn, whiteboard, blackboard);
    //dynamic gen -> for sliding 
    selected.moves = filterMoveBlocks(pos.file, pos.rank, selected.moves, pieceType, turn, whiteboard, blackboard);
    selected.isSelected = true;
    //*don't move piece
    return 0; 
}



void movePiece(char fromFile, int fromRank, char toFile, 
    int toRank, PieceList &mainboard, BitBoardSet &whiteboard, BitBoardSet &blackboard){
    char srcPiece = mainboard.getPiece(fromFile, fromRank);
    char destPiece = mainboard.getPiece(toFile, toRank);
    int srcPieceID = piecesCharMap[srcPiece];
    int attackedPieceID = piecesCharMap[destPiece];
    
    if(isWhitePiece(srcPiece)){
        whiteboard.movePiece(srcPieceID, fromFile, fromRank, toFile, toRank); 
        blackboard.unsetPiece(attackedPieceID, toFile, toRank);
    }
    else{
        blackboard.movePiece(srcPieceID, fromFile, fromRank, toFile, toRank);
        whiteboard.unsetPiece(attackedPieceID, toFile, toRank);
    }
    
    mainboard.movePiece(fromFile, fromRank, toFile, toRank);
}

