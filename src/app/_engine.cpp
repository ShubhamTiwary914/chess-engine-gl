#include "./_engine.h"


BitBoardSet whiteboard;
BitBoardSet blackboard;
PieceList mainboard;
movesSetStruct movesSet;
GameState state;


void initializeBoard(){
    FENParser(FENstart, whiteboard, blackboard);
    // logger(DEBUG, "Done with FEN parsing!");
    // printBitBoard(blackboard.getPiece(KING), "KING white: ", true, 'k');
    // printBitBoard(blackboard.getPiece(QUEEN), "QUEEN white: ", true, 'q');
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
        if(getBit(selected.moves, moveIndex)){
            //en-passant if double jump
            if((selected.piece == 'p' || selected.piece == 'P') && abs(pos.rank - selected.rank)==2)
                state.set_enPassant(turn, parseFileID(pos.file));
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
    //pawn --> (promotions, attacks & moves are different, en passant)
    if(pieceType == PAWN)
        selected.moves = filterPawnMoves(pos.file, pos.rank, selected.moves, turn, whiteboard, blackboard, movesSet, state);
    //king --> castling
    if(pieceType == KING)
        selected.moves |= getKingCastlingCheck(pos.file, pos.rank, whiteboard, blackboard, state);
    //dynamic gen -> for sliding 
    selected.moves = filterMoveBlocks(pos.file, pos.rank, selected.moves, pieceType, turn, whiteboard, blackboard);
    selected.isSelected = true;
    //*don't move piece
    return 0; 
}


//* Captures
void movePiece(char fromFile, int fromRank, char toFile, 
    int toRank, PieceList &mainboard, BitBoardSet &whiteboard, BitBoardSet &blackboard){
    char srcPiece = mainboard.getPiece(fromFile, fromRank);
    char destPiece = mainboard.getPiece(toFile, toRank);
    int srcPieceID = piecesCharMap[srcPiece];
    int attackedPieceID = piecesCharMap[destPiece];
    
    //>change in bitboard
    if(isWhitePiece(srcPiece)){
        whiteboard.movePiece(srcPieceID, fromFile, fromRank, toFile, toRank); 
        blackboard.unsetPiece(attackedPieceID, toFile, toRank);
    }
    else{
        blackboard.movePiece(srcPieceID, fromFile, fromRank, toFile, toRank);
        whiteboard.unsetPiece(attackedPieceID, toFile, toRank);
    }

    //> en-passant capture: pawn catures a empty tile in different file
    if((state.is_enPassant()) && (srcPieceID == PAWN) && (attackedPieceID == NONE) && (toFile != fromFile)){
        int passantRank = (state.turn == WHITE_TURN) ? (toRank-1) : (toRank+1);
        (state.turn == WHITE_TURN) ? blackboard.unsetPiece(PAWN, toFile, passantRank): 
                                     whiteboard.unsetPiece(PAWN, toFile, passantRank);
        mainboard.unsetPiece(toFile, passantRank); 
    }
    //>castling: 1) unset if king/rook move + 2) swap king/rook if selected 
    //1) unset if king/rook moves
    if(srcPieceID == ROOK && (fromRank == 1 || fromRank == 8) && (fromFile == 'a' || fromFile == 'h')){
        bool leftSide = (fromFile == 'a') ? true : false;
        state.markCastling(state.turn, leftSide);
    }  
    //2) swap rook/king if selected  
    else if(srcPieceID == KING){
        int step = parseFileID(toFile)-parseFileID(fromFile);
        if(abs(step) == 2){
            bool rookLeftSide = (step == -2);
            //logger(DEBUG, "left SIDE: ", printBitsU8());
            if(state.checkCastlingAvailable(state.turn, rookLeftSide)){ 
                char rookFile = (rookLeftSide) ? 'a' : 'h';
                int rookRank = (state.turn == WHITE_TURN) ? 1 : 8;
                int rookStep = (rookLeftSide) ? 1 : -1;
                mainboard.movePiece(fromFile, fromRank, toFile, toRank);
                mainboard.movePiece(rookFile, rookRank, (char)(toFile+rookStep), toRank);
                if(state.turn == WHITE_TURN){
                    whiteboard.movePiece(KING, fromFile, fromRank, toFile, toRank);
                    whiteboard.movePiece(ROOK, rookFile, rookRank, (char)(toFile+rookStep), toRank);
                }
                else{
                    blackboard.movePiece(KING, fromFile, fromRank, toFile, toRank);
                    blackboard.movePiece(ROOK, rookFile, rookRank, (char)(toFile+rookStep), toRank);
                }
            }
        }
        state.markCastling(state.turn, false, true);
        return;
    }  
    mainboard.movePiece(fromFile, fromRank, toFile, toRank);
}

