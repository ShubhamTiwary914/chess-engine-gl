#include "types.h"

#ifndef BOARDS_H
#define BOARDS_H

/// board indexes =========
// Rank 1 .. Rank 8 -> 0..7
// A-File .. H-File -> 0..7
namespace engine {

typedef struct {
  //<row=white,black, cols=bitboards>
  u64 bitboard[2][6];
  //<row=rank, cols=files>
  char stringboard[8][8];
} BoardSet;

BoardSet initBoardSet(std::string fenString = FENstartpos);
void printStringBoard(char stringboard[8][8]);
u64 unionSideBitBoard(u64 bitboard_sidepieces[6]);

}; // namespace engine

#endif
