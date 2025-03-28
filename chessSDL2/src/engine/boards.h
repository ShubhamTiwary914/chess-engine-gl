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


void FENParser(std::string fenString, BitBoardSet &whiteboard, BitBoardSet &blackboard);






#endif