#include "imports.h"
#include "types.h"

namespace utils{
 
void printBitBoard(u64 bitboard);

bool checkBit64(u64 bitboard,int idx);
void setBit64(u64 *bitboard, int idx);
void clearBit64(u64 *bitboard);

};
