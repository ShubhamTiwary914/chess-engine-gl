#ifndef ZOBRIST_H
#define ZOBRIST_H

#include <utils/consts.h>


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


#endif