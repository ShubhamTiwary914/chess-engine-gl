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
std::pair<int,int> fetchSinglebit_rankfile(u64 board);
int fetchSinglebit_bitidx(u64 board);

bool checkBit64(u64 bitboard, int idx);
void setBit64(u64 *bitboard, int idx);
void clearBit64(u64 *bitboard);
u64 substractBit64(u64 bitboardFrom, u64 bitboardTargets);


u64 maskRanks(std::vector<int> ranks);
u64 maskFiles(std::vector<int> bigfiles);




}; // namespace utils
