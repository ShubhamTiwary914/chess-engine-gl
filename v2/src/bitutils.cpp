#include "../lib/bitutills.h"
#include <bitset>
#include <cwchar>


void utils::printBitBoard(u64 bitboard){
    for (int rank = 7; rank >= 0; rank--) {
        for (int file = 0; file < 8; file++) {
            int idx = rank*8 + file;
            std::cout << (int)checkBit64(bitboard, idx);
        }
        std::cout << '\n';
    }
}


bool utils::checkBit64(u64 bitboard,int idx){
    u64 mask = 1ULL << idx;
    return mask & bitboard;        
}

void utils::setBit64(u64 *bitboard, int idx){
    int mask = 1LL << idx;
    *bitboard = (*bitboard) | mask;  
}
