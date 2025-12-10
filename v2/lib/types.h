#include "imports.h"

#ifndef TYPES_H
#define TYPES_H

typedef uint64_t u64;

// Little endian rank-file (LERF) mapping
enum INDEXES {
  a1,
  b1,
  c1,
  d1,
  e1,
  f1,
  g1,
  h1,
  a2,
  b2,
  c2,
  d2,
  e2,
  f2,
  g2,
  h2,
  a3,
  b3,
  c3,
  d3,
  e3,
  f3,
  g3,
  h3,
  a4,
  b4,
  c4,
  d4,
  e4,
  f4,
  g4,
  h4,
  a5,
  b5,
  c5,
  d5,
  e5,
  f5,
  g5,
  h5,
  a6,
  b6,
  c6,
  d6,
  e6,
  f6,
  g6,
  h6,
  a7,
  b7,
  c7,
  d7,
  e7,
  f7,
  g7,
  h7,
  a8,
  b8,
  c8,
  d8,
  e8,
  f8,
  g8,
  h8
};

extern std::string FENstartpos;

#define WHITE_INDEX 0
#define BLACK_INDEX 1

#define PAWN_INDEX 0
#define KNIGHT_INDEX 1
#define KING_INDEX 2
#define ROOK_INDEX 3
#define BISHOP_INDEX 4
#define QUEEN_INDEX 5

#define PAWN_WHITE_CHAR 'P'
#define KNIGHT_WHITE_CHAR 'N'
#define KING_WHITE_CHAR 'K'
#define ROOK_WHITE_CHAR 'R'
#define BISHOP_WHITE_CHAR 'B'
#define QUEEN_WHITE_CHAR 'Q'

#define PAWN_BLACK_CHAR 'p'
#define KNIGHT_BLACK_CHAR 'n'
#define KING_BLACK_CHAR 'k'
#define ROOK_BLACK_CHAR 'r'
#define BISHOP_BLACK_CHAR 'b'
#define QUEEN_BLACK_CHAR 'q'

#define EMPTY_CHAR 'x'

// file consts
extern u64 FILE_A_BITS;
extern u64 FILE_H_BITS;

extern u64 RANK_1_BITS;
extern u64 RANK_8_BITS;

extern u64 MASK_LIGHTSQUARES;
extern u64 MASK_DARKSQUARES;

extern u64 DIAGONAL_A1_H8;
extern u64 DIAGONAL_H1_A8;


//macros for debugging -----------

///@brief print bit board with message
#define bbdebug(msg,board) std::cout<<"\n" << msg << "\n"; utils::printBitBoard(board);


#endif
