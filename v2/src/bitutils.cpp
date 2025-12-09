#include "../lib/bitutills.h"
#include <cassert>

// ==============\\
//   INDEXES     \\
// ==============\/
///@brief Little Endian Rank-File Mapping Index (ref: docs-board.h)
///@param rank - range: 0 to 7
///@param file - range: 0 to 7
///@returns LERF index (int)
int utils::getLERFIndex(int rank, int file) { return rank * 8 + file; }


///@brief Little Endian Rank-File Mapping Index (ref: docs-board.h)
///@param bigidx - BERF index
///@returns LERF index (int)
int utils::getLERFIndex(int bigidx) {
  int rank, file;
  std::tie(rank, file) = utils::getLERF_rankfile(bigidx);
  return rank * 8 + file;
}

///@brief from index (63) -> map to <rank,file> of LERF mapping
std::pair<int, int> utils::getLERF_rankfile(int smidx) {
  int rank = smidx / 8;
  int file = smidx % 8;
  return std::pair<int, int>(rank, file);
}

///@brief convert BERF rank,file indexes to LERF rank,file indexes
///@returns: rank same, file is reversed: 0-7, 1-6, ...., 7-0
std::pair<int,int> utils::getLERF_rankfile(int bigrank, int bigfile){
  return std::pair<int,int>(bigrank, (7-bigfile));
}

///@brief map piece char to piece index (ex: 'p' or 'P' --> 0)
int utils::getPieceIdx(char ch) {
  ch = (char)tolower(ch);
  assert(
    ch==PAWN_BLACK_CHAR || ch==KNIGHT_BLACK_CHAR || ch==KING_BLACK_CHAR || 
    ch==BISHOP_BLACK_CHAR || ch==ROOK_BLACK_CHAR || ch==QUEEN_BLACK_CHAR
  );
  switch (ch) {
  case 'p':
    return PAWN_INDEX;
  case 'k':
    return KING_INDEX;
  case 'n':
    return KNIGHT_INDEX;
  case 'r':
    return ROOK_INDEX;
  case 'b':
    return BISHOP_INDEX;
  case 'q':
    return QUEEN_INDEX;

  default:
    return -1;
  }
  return -1;
}


// ==============\\
//   BIT OPS     \\
// ==============\/

bool utils::checkBit64(u64 bitboard, int idx) {
  u64 mask = 1ULL << idx;
  return mask & bitboard;
}

void utils::setBit64(u64 *bitboard, int idx) {
  u64 mask = 1ULL << idx;
  *bitboard = (*bitboard) | mask;
}



// ==============\/
//   OTHERS      \/
// ==============\/
///@brief top-left -> bottom right, print bit if piece in that pos exists
///(assumes LERF bitboard)
void utils::printBitBoard(u64 bitboard) {
  for (int rank = 7; rank >= 0; rank--) {
    for (int file = 0; file < 8; file++) {
      int idx = utils::getLERFIndex(rank, file);
      std::cout << (int)checkBit64(bitboard, idx);
    }
    std::cout << '\n';
  }
}



// ==============\\
//   BIT-MASKS   \\
// ==============\/

///@brief mask the {f1,f2,...,fn} files (LERF)
///@param bigfiles: files needed from normal chessboard POV(a to h) as (0 to 7)
u64 utils::maskFiles(std::vector<int> bigfiles){
  u64 masked = 0ULL;
  for(int bigfile: bigfiles){
    assert(bigfile>=0 && bigfile<=7);
    masked |= (FILE_A_BITS << bigfile);
  }
  return masked;
}

///@brief mask the {r1,r2,...,r2} ranks (LERF)
///@param ranks: ranks needed from 0(bottom) to 7(high)
u64 utils::maskRanks(std::vector<int> ranks){
  u64 masked = 0ULL;
  for(int rank: ranks){
    assert(rank>=0 && rank<=7);
    masked |= (RANK_1_BITS << (1ULL)*(rank*8));
  }
  return masked;
}