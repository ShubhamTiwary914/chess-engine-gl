#include "imports.h"
#include "types.h"

namespace utils {

void printBitBoard(u64 bitboard);
int getLERFIndex(int rank, int file);

// std::pair<int, int> getBERF_rankfile(int bigidx);
std::pair<int, int> getLERF_rankfile(int smidx);
std::pair<int, int> getLERF_rankfile(int bigrank, int bigfile);
int getLERFIndex(int bigidx);

int getPieceIdx(char ch);

bool checkBit64(u64 bitboard, int idx);
void setBit64(u64 *bitboard, int idx);
void clearBit64(u64 *bitboard);

}; // namespace utils
