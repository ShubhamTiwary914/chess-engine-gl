#ifndef BOARDS_H
#define BOARDS_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include "utils/engine.h"
#include "utils/consts.h"


typedef struct {
    int pieceID;
    uint64_t board = 0;
} Bitboard;



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






class BitBoardSet {
    private: 
        Bitboard pieces[6];
    public:
        BitBoardSet();

        int pieceCount(int pieceID);
        uint64_t getPiece(int pieceID);
        void setPiece(int pieceID, char file, int rank);
        void setPieceIndex(int pieceID, int bitIndex);
        void unsetPiece(int pieceID, char file, int rank);
        void movePiece(int pieceID, char fromFile, int fromRank, char toFile, int toRank);
        
        bool checkBitBoard(int pieceID, char file, int rank);
        u_int64_t getUnion();  
};





class PieceList {
    private:
        char board[64];

    public:
        PieceList();
        void setPiece(char piece, char file, int rank);
        void unsetPiece(char file, int rank);
        void movePiece(char fromFile, int fromRank, char toFile, int toRank);

        void swapPieces(char f1, int r1, char f2, int r2);
        char getPiece(char file, int rank);

        void printBoard();
        void translateBitboard_toPieceList(BitBoardSet &whiteboard, BitBoardSet &blackboard);
};





/**
 * @brief Maps all possible move sets to 64-bit integers, performing XOR operations 
 * to keep move repetitions in check.
 *
 * Distributed into 4 zobrist tables for move types:
 * - 64 square x 2 sides x 6 pieces = 768 keys
 * - 4 castling positions (2 white + 2 black) = 4 ways
 * - En Passant: [8 white + 8 black + 1 "none"]
 * - 2 turns (0=white, 1=black)
 *
 * Usage (after each move):
 * - Initially: hash = 0
 * - hash ^= pieceKeys[turn][piece][square]
 * - hash ^= castleKeys[ID], ...
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
        ZobristKeys(uint64_t keySeed);
        u_int64_t getZobristHash();
        void generateRandom_zobristKeys();
};




class GameHistory {
    private:
        std::vector<GameState> states;
        uint8_t count;

    public:
        GameHistory();
        std::vector<GameState> *getRef();
        int currStateID();
        void clearStates();
        void push(GameState newState);
        void pop();
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


void FENParser(std::string fenString, BitBoardSet &whiteboard, BitBoardSet &blackboard);

#endif