#ifndef CONSTS_H
#define CONSTS_H

#include <iostream>
#include <cassert>
#include <unordered_map>
#include <cstdint>

#define isLower(ch) (ch>='a' && ch<='z');
#define KING    0
#define QUEEN   1
#define ROOK    2
#define BISHOP  3
#define KNIGHT  4
#define PAWN    5
#define NONE    6

#define WHITE_TURN  0
#define BLACK_TURN  1


#define FENstart "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"



/**
 * @brief Assertion with message
 * @param condition boolean expression to validate
 * @param message error message if condition fails
 */
void BREAKPOINT(bool condition, const std::string &message);



//map piece char -> piece ID (ex:  k/K->0, q/Q->1, ...)
extern std::unordered_map<char, int> piecesCharMap;



extern const uint64_t FILE_A_BITS;
extern const uint64_t RANK_1_BITS;
extern const uint64_t DIAGONAL_RIGHT_BASE;
extern const uint64_t DIAGONAL_LEFT_BASE;


// uint64_t testingRookMask(char file, int rank){
//     initializeBoard();
//     uint64_t empty_board = 0ULL;

//     uint64_t blockers = empty_board;
//     setBit(blockers, getPieceBitIndex('b', 4));  
//     setBit(blockers, getPieceBitIndex('d', 7));
//     setBit(blockers, getPieceBitIndex('d', 8));
//     uint64_t rookBit = empty_board; setBit(rookBit, getPieceBitIndex(file,rank));

//     printBitBoard(rookBit, "Rook at d4"); 
//     printBitBoard(blockers, "Blockers: ");
//     uint64_t difference = blockers - 2*rookBit;
//     printBitBoard(difference, "Difference: ");

//     uint64_t change = difference ^ blockers;
//     printBitBoard(change, "Change: ");

//     uint64_t rookMask = getRookMask(file, rank, false); 
//     setBit(rookMask, getPieceBitIndex(file,rank));  
//     printBitBoard(rookMask, "Rook Mask: ");

//     uint64_t attack = change & rookMask;
//     printBitBoard(attack, "Attack Path: ");

//     return 0ULL;
// }

extern const int KNIGHT_OFFSETS[8];
extern const int KING_OFFSETS[8]; 
extern const int PAWN_CAPTURE_OFFSETS[2];


extern const uint64_t ROOK_MAGICS[64];
extern const uint64_t BISHOP_MAGICS[64];



#endif