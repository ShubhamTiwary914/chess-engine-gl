#ifndef CONSTS_H
#define CONSTS_H

#include <iostream>
#include <cassert>
#include <unordered_map>
#include <cstdint>

#define isLower(ch) (ch>='a' && ch<='z')
//jumping
#define KING    0
#define PAWN   1
#define KNIGHT    2
//sliding
#define BISHOP  3
#define ROOK  4
#define QUEEN    5
#define NONE    6

#define WHITE_TURN  0
#define BLACK_TURN  1

//same order as above (in string)
#define PIECES_WHITE_STR "KPNBRQ"
#define PIECES_BLACK_STR "kpnbrq"



#define FENstart "rnbkqbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBKQBNR w KQkq - 0 1"
using U64 = uint64_t;
using U8 = uint8_t;

/**
 * @brief Assertion with message
 * @param condition boolean expression to validate
 * @param message error message if condition fails
 */
void BREAKPOINT(bool condition, const std::string &message);

enum LogLevel {DEBUG, INFO, WARN, ERROR};


//map piece char -> piece ID (exusing U64 = uint64_t;:  k/K->0, q/Q->1, ...)
extern std::unordered_map<char, int> piecesCharMap;
enum SQUARE {
    a1, b1, c1, d1, e1, f1, g1, h1,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a8, b8, c8, d8, e8, f8, g8, h8
  };
  


extern const uint64_t FILE_H_BITS;
extern const uint64_t RANK_1_BITS;
extern const uint64_t DIAGONAL_RIGHT_BASE;
extern const uint64_t DIAGONAL_LEFT_BASE;


/// @brief mask between king & rook in rank 1 (left side)
extern const uint64_t KING_LEFT_ROOK_RANK_1;
/// @brief mask between king & rook in rank 1 (right side)
extern const uint64_t KING_RIGHT_ROOK_RANK_1;
/// @brief mask between king & rook in rank 8 (left side)
extern const uint64_t KING_LEFT_ROOK_RANK_8;
/// @brief mask between king & rook in rank 8 (right side)
extern const uint64_t KING_RIGHT_ROOK_RANK_8; 



extern const uint64_t RANK_MASKS[8];
extern const uint64_t FILE_MASKS[8];

extern const int KNIGHT_OFFSETS[8];
extern const int KING_OFFSETS[8]; 
extern const int PAWN_CAPTURE_OFFSETS[2];


extern const uint64_t ROOK_MAGICS[64];
extern const uint64_t BISHOP_MAGICS[64];



#endif