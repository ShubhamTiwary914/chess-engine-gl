#include "types.h"
#include "bitutills.h"

#ifndef BOARDS_H
#define BOARDS_H

/// board indexes =========
// Rank 1 .. Rank 8 -> 0..7
// A-File .. H-File -> 0..7
namespace engine {

///@brief board structure with stringboard and bitboards set
///@param bitboard u64: [color][pieceType]
///@param stringboard char: [rank][file], rank,file=[0-7]
typedef struct {
  u64 bitboard[2][6];
  /// String board: [rank][file], null-terminated (\0) per rank
  char stringboard[8][9];
} BoardSet;

BoardSet initBoardSet(std::string fenString = FENstartpos);
void clearBoardSet(BoardSet *boardset);
void printStringBoard_LERF(char stringboard[8][9]);
u64 unionSideBitBoard(u64 bitboard_sidepieces[6]);

u64 getCurrentSide_bitBoard(BoardSet *boardset, int currside);
u64 getOppsSide_bitBoard(BoardSet *boardset, int currside);


}; // namespace engine

#endif
