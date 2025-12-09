#include "imports.h"
#include "types.h"
#include<vector>

namespace utils {

void printBitBoard(u64 bitboard);

std::pair<int, int> getLERF_rankfile(int smidx);
std::pair<int, int> getLERF_rankfile(int bigrank, int bigfile);
int getLERFIndex(int bigidx);
int getLERFIndex(int smrank, int smfile);
int getPieceIdx(char ch);

bool checkBit64(u64 bitboard, int idx);
void setBit64(u64 *bitboard, int idx);
void clearBit64(u64 *bitboard);

u64 maskRanks(std::vector<int> ranks);
u64 maskFiles(std::vector<int> bigfiles);

}; // namespace utils
