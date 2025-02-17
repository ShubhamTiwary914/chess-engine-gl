#ifndef BOARDS_H
#define BOARDS_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include "utils/gameutils.h"


//constants.
#define KING    0
#define QUEEN   1
#define ROOK    2
#define BISHOP  3
#define KNIGHT  4
#define PAWN    5
#define NONE    6

#define WHITE_TURN  0
#define BLACK_TURN  1

//map piece char -> piece ID (ex:  k/K->0, q/Q->1, ...)
std::map<char, int> piecesCharMap = {
    {'k', KING},   {'K', KING},
    {'q', QUEEN},  {'Q', QUEEN},
    {'r', ROOK},   {'R', ROOK},
    {'b', BISHOP}, {'B', BISHOP},
    {'n', KNIGHT}, {'N', KNIGHT},
    {'p', PAWN},   {'P', PAWN},
    {'E', NONE}   
};

/*
    Bitboard:  64-bit - single piece representation. (ex: white-king, black-pawn)
    BitBoardSet:  Bitboard collection for one side (each side 6 bitboards - for 6 pieces) = 12 bitboards total.
*/
typedef struct {
    int pieceID;
    uint64_t board = 0;
} Bitboard;



class BitBoardSet {
    private: 
        Bitboard pieces[6];

    public:
    BitBoardSet(){ 
        for(int i=0; i<6; i++) { 
            Bitboard piece;
            pieces[i] = piece;
        };
    }   

    int pieceCount(int pieceID){
        assert(pieceID >= 0 && pieceID <= 6);
        int i=0;
        int buffer = pieces[pieceID].board;
        while(buffer > 0){
            i++;
            buffer = (buffer)&(buffer-1);
        }
        return i;
    }

    uint64_t getPiece(int pieceID){
        return this->pieces[pieceID].board;
    }

    //*rank = row(1-8), file = col(A-H)
    void setPiece(int pieceID, char file, int rank){
        int bitIndex = getPieceBitIndex(file, rank);
        setBit(this->pieces[pieceID].board, bitIndex);
    }

    //set piece from index (not rank/file like usual)
    void setPieceIndex(int pieceID, int bitIndex){
        BREAKPOINT(bitIndex >= 0 && bitIndex <= 63, format("Bitindex needed range: [0,63], received: %d", bitIndex));
        setBit(this->pieces[pieceID].board, bitIndex);
    }

    //*rank = row(1-8), file = col(A-H)
    bool checkBitBoard(int pieceID, char file, int rank){
        int bitIndex = getPieceBitIndex(file, rank);
        return getBit(this->pieces[pieceID].board, bitIndex);
    }

    void unsetPiece(int pieceID, char file, int rank){
        int bitIndex = getPieceBitIndex(file, rank);
        clearBit(this->pieces[pieceID].board, bitIndex);
    }

    void movePiece(int pieceID, char fromFile, int fromRank, char toFile, int toRank){
        //source should be set, target should be empty
        BREAKPOINT(checkBitBoard(pieceID, fromFile, fromRank) && !checkBitBoard(pieceID, toFile, toRank), "Source should be set, target should be empty");
        //set in new pos,
        setPiece(pieceID, toFile, toRank);
        //remove old pos.
        unsetPiece(pieceID, fromFile, fromRank);
    }

    
    u_int64_t getUnion(){
        uint64_t board = 0;
        for(int i=0;i<6;i++)
            board |= this->pieces[i].board;
        return board;
    }   


    //print the boards from pieces[]
    void printBitBoard(int pieceID);
    //print some board given as int (for test)
    void printBoard(u_int64_t testboard);
};


/*
    char[64] index = [0,1,2,3,4,..,63]  -> (default)
    indexing followed here:  [63,62,61,...,0]  (nth bit from left) -> (to normalise all game boards indexing)

    PieceList:  64-bit string board (joined BitBoardSet) 
      - black pieces: lowercase (k,q,r,n,p,b)
      - white pieces: uppercase (K,Q,E,N,P,B)
      - empty pieces: E
*/
class PieceList {
    private:
    char board[64];

    public:
    PieceList() {
        for(int i=63; i>=0; i--)
            board[i] = 'E';
    }

    void setPiece(char piece, char file, int rank){
        int bitIndex = getPieceBitIndex(file, rank);
        this->board[63-bitIndex] = piece;
    }

    void unsetPiece(char file, int rank){
        int bitIndex = getPieceBitIndex(file, rank);
        this->board[63-bitIndex] = 'E';
    }

    void movePiece(char fromFile, int fromRank, char toFile, int toRank){
        //source should be set & dest. may be unset (when capture)
        char piece = this->getPiece(fromFile, fromRank);
        assert(piece != 'E');
        this->setPiece(piece, toFile, toRank);
        this->unsetPiece(fromFile, fromRank);
    }

    void swapPieces(char f1, int r1, char f2, int r2){
        char temp = this->getPiece(f1, r1);
        this->setPiece(this->getPiece(f2, r2), f1, r1);
        this->setPiece(temp, f2, r2);
    }

    char getPiece(char file, int rank){
        int bitIndex = getPieceBitIndex(file, rank);
        return  board[63-bitIndex];
    }

     void printBoard() {
        std::string bits = "";
        for(int rank=8; rank>=1; rank--){
            for(int file=65; file<= 72; file++){
                bits += this->getPiece(file, rank);
                bits += " ";
            }
            bits += "\n";
        }
        std::cout << bits;
    }   

    void translateBitboard_toPieceList(BitBoardSet &whiteboard, BitBoardSet &blackboard);
};


#endif



//>-----------------------------------------------------------------------------------------------------------------
//>-----------------------------------------------------------------------------------------------------------------



#ifndef ZOBRIST_H
#define ZOBRIST_H

/*
    *Map all possible move sets -> 64 bit integer. performing XOR operations to keep move repetations in check.
    Distributed into 4 zobrist tables for move types:
        - 64 square x 2 sides x 6 pieces = 768 keys
        - 4 castling positions (2 white + 2 black) = 4 ways
        - En Passant: [8 white + 8 black + 1 "none"] 
        - 2 turns (0=white, 1=black).

    Use (after each move):
        - Initially: hash = 0;
        - hash ^= pieceKeys[turn][piece][square]
        - hash ^= castleKeys[ID], ...
 */

class ZobristKeys{
    private:
        uint64_t pieceKeys[2][6][64]; //[turn, piece, squareID]
        uint64_t castleKeys[4];
        uint64_t enPeassantkeys[17];  
        uint64_t turnKeys[2];   

        uint64_t zobristSeed;
        //curr game key.
        uint64_t zobristHash;

    public:
        ZobristKeys(uint64_t keySeed = std::random_device{}()){
            this->zobristSeed = keySeed;
            this->generateRandom_zobristKeys();
        }

        u_int64_t getZobristHash(){
            return this->zobristHash;
        }

        //key from current -> iterate all once (on-startup)
        void computeKey(){

        }

        //each move -> update only the move done (set/unset -> any update)
        void updateKey(){

        }

        void generateRandom_zobristKeys(){
            //Piece Keys
            for(int i=0; i<2; i++){
                for(int j=0; j<6; j++){
                     for(int k=0; k<64; k++){
                        this->pieceKeys[i][j][k] = generateRandomU64(this->zobristSeed);
                     }
                }
            }
            //Castle Keys
            for(int i=0; i<4; i++)
                this->castleKeys[i] = generateRandomU64(this->zobristSeed);
            //en-passant keys.
            for(int i=0; i<17; i++)
                this->enPeassantkeys[i] = generateRandomU64(this->zobristSeed);
            //turn keys (0/1)
            for(int i=0; i<2; i++)
                this->turnKeys[i] = generateRandomU64(this->zobristSeed);
        }
};


#endif



//>-----------------------------------------------------------------------------------------------------------------
//>-----------------------------------------------------------------------------------------------------------------


typedef struct {
    uint8_t activeColor = WHITE_TURN;   
    uint8_t castling = 0;
    //turns since last capture/pawn push.
    uint8_t halfMoveClock = 0;
    uint8_t enPassant = 0;
    //2x full moves. (1 full cycle)
    uint8_t fullMoveClock = 0;
    uint64_t zobristHash;
    uint64_t currMove;
} GameState;


class GameHistory {
    private:
        std::vector<GameState> states;
        uint8_t count;

    public:
        GameHistory(){
            this->count = 0;
        }

        void clearStates(){
            this->states.clear();
            this->count = 0;
        }

        void push(GameState newState){
            this->states.push_back(newState);
            this->count++;
        }

        void pop(){
            BREAKPOINT(this->count>=1, "states count <=0, can't pop");
            this->states.pop_back();
            this->count--;
        }

        std::vector<GameState> *getRef(){
            return &states;
        }

        int currStateID(){
            return this->count;
        }
};


//* Main class -> controls all history, states.
class Board {
    public:
        BitBoardSet whiteboard; 
        BitBoardSet blackboard; 
        PieceList fullboard;
        GameState state;
        GameHistory history;
        ZobristKeys zobrist;
};




//*Extended Utilities (shared)
void BitBoardSet::printBitBoard(int pieceID) {
        std::string bits = "";
        for(int rank=8; rank>=1; rank--){
            for(int file=65; file<= 72; file++){
                if(checkBitBoard(pieceID, (char)file, rank)){
                    bits += "1 ";
                }else {
                    bits += "0 ";
                }   
            }
            bits += "\n";
        }
        std::cout << bits;
}   


void BitBoardSet::printBoard(u_int64_t testboard){
        std::string bits = "";
        for(int rank=8; rank>=1; rank--){
            for(int file=65; file<= 72; file++){
                int bitIndex = getPieceBitIndex(file, rank);
                if(getBit(testboard, bitIndex)){
                    bits += "1 ";
                }else {
                    bits += "0 ";
                }   
            }
            bits += "\n";
        }
        std::cout << bits;
} 


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


void PieceList::translateBitboard_toPieceList(BitBoardSet &whiteboard, BitBoardSet &blackboard) {
    for(int rank = 8; rank >=1; rank--) {
        for(char file = 'A'; file <= 'H'; file++) {
            for(int pieceID = KING; pieceID <= PAWN; pieceID++) {
                if(whiteboard.checkBitBoard(pieceID, file, rank)) {
                    //uppercase -> white
                    char piece = "KQRBNP"[pieceID];
                    setPiece(piece, file, rank);
                    break;
                }
                else if(blackboard.checkBitBoard(pieceID, file, rank)) {
                    // lowercase -> black
                    char piece = "kqrbnp"[pieceID];
                    setPiece(piece, file, rank);
                    break;
                }
            }
        }
    }
}