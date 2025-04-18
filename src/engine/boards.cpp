#include "./boards.h"


//#region bitboardset
//> BitboardSet ---------------------------------------------------------

BitBoardSet::BitBoardSet(){
    for(int i=0; i<6; i++) { 
        Bitboard piece;
        pieces[i] = piece;
    };
}

int BitBoardSet::pieceCount(int pieceID){
    assert(pieceID >= 0 && pieceID <= 6);
    int i=0;
    int buffer = pieces[pieceID].board;
    while(buffer > 0){
        i++;
        buffer = (buffer)&(buffer-1);
    }
    return i;
}


uint64_t BitBoardSet::getPiece(int pieceID){
    return this->pieces[pieceID].board;
}


void BitBoardSet::setPiece(int pieceID, char file, int rank){
    int bitIndex = getPieceBitIndex(file, rank);
    setBit(this->pieces[pieceID].board, bitIndex);
}

void BitBoardSet::setPieceIndex(int pieceID, int bitIndex){
    std::stringstream error_msg;
    error_msg << "Bitindex needed range: [0,63], received: " << bitIndex;
    BREAKPOINT(bitIndex >= 0 && bitIndex <= 63, error_msg.str());
    setBit(this->pieces[pieceID].board, bitIndex);
}


bool BitBoardSet::checkBitBoard(int pieceID, char file, int rank){
    int bitIndex = getPieceBitIndex(file, rank);
    return getBit(this->pieces[pieceID].board, bitIndex);
}


void BitBoardSet::unsetPiece(int pieceID, char file, int rank){
    int bitIndex = getPieceBitIndex(file, rank);
    clearBit(this->pieces[pieceID].board, bitIndex);
}


void BitBoardSet::movePiece(int pieceID, char fromFile, int fromRank, char toFile, int toRank){
    //set in new pos,
    setPiece(pieceID, toFile, toRank);
    //remove old pos.
    unsetPiece(pieceID, fromFile, fromRank);
}

u_int64_t BitBoardSet::getUnion(){
        uint64_t board = 0;
        for(int i=0;i<6;i++)
            board |= this->pieces[i].board;
        return board;
} 





//#region piecelist
//> PieceList ------------------------------------------------------------

PieceList::PieceList() {
    for(int i=63; i>=0; i--)
        board[i] = 'E';
}

void PieceList::setPiece(char piece, char file, int rank){
    int bitIndex = getPieceBitIndex(file, rank);
    this->board[63-bitIndex] = piece;
}


void PieceList::unsetPiece(char file, int rank){
    int bitIndex = getPieceBitIndex(file, rank);
    this->board[63-bitIndex] = 'E';
}

void PieceList::movePiece(char fromFile, int fromRank, char toFile, int toRank){
    //source should be set & dest. may be unset (when capture)
    char piece = this->getPiece(fromFile, fromRank);
    assert(piece != 'E');
    this->setPiece(piece, toFile, toRank);
    this->unsetPiece(fromFile, fromRank);
}

void PieceList::swapPieces(char f1, int r1, char f2, int r2){
    char temp = this->getPiece(f1, r1);
    this->setPiece(this->getPiece(f2, r2), f1, r1);
    this->setPiece(temp, f2, r2);
}

char PieceList::getPiece(char file, int rank){
    int bitIndex = getPieceBitIndex(file, rank);
    return  board[63-bitIndex];
}

void PieceList::printBoard() {
    std::string bits = "";
    for(int rank=8; rank>=1; rank--){
        for(int file=65; file<= 72; file++){
            bits += this->getPiece(file, rank);
            bits += " ";
        }
        bits += "\n";
    }
    std::cout << bits << std::endl;
}   


void PieceList::translateBitboard_toPieceList(BitBoardSet &whiteboard, BitBoardSet &blackboard) {
    for(int rank = 8; rank >=1; rank--) {
        for(char file = 'A'; file <= 'H'; file++) {
            for(int pieceID = 0; pieceID <= 6; pieceID++) {
                if(whiteboard.checkBitBoard(pieceID, file, rank)) {
                    //uppercase -> white
                    char piece = PIECES_WHITE_STR[pieceID];
                    setPiece(piece, file, rank);
                    break;
                }
                else if(blackboard.checkBitBoard(pieceID, file, rank)) {
                    // lowercase -> black
                    char piece = PIECES_BLACK_STR[pieceID];
                    setPiece(piece, file, rank);
                    break;
                }
            }
        }
    }
}





//#region utils
//> Extended Utilities ----------------------------------------------------

void FENParser(std::string fenString, BitBoardSet &whiteboard, BitBoardSet &blackboard) {
    int index = 0;  // Current position in FEN string
    int bitIndex = 63;  // Current board position (0-63)
    while(fenString[index] != ' ' && index < fenString.length()) {
        char c = fenString[index];
        if (c == '/') {
            // Skip rank separator
            index++;
            continue;
        }
        else if (isdigit(c)) {
            // Skip empty squares
            bitIndex -= (c - '0');
        }
        else {
            // Place piece
            if (islower(c)) 
                blackboard.setPieceIndex(piecesCharMap[c], bitIndex);
            else 
                // White piece
                whiteboard.setPieceIndex(piecesCharMap[c], bitIndex);
            bitIndex--;
        }
        index++;
    }
}
