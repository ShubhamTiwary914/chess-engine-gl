#include "../lib/bitutills.h"

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
  std::tie(rank, file) = utils::getBERF_rankfile(bigidx);
  return rank * 8 + file;
}

///@brief from big index (63) -> map to <rank,file> of BERF mapping
std::pair<int, int> utils::getBERF_rankfile(int bigidx) {
  int rank = bigidx / 8;
  int file = bigidx % 8;
  return std::pair<int, int>(rank, file);
}

bool utils::checkBit64(u64 bitboard, int idx) {
  u64 mask = 1ULL << idx;
  return mask & bitboard;
}

void utils::setBit64(u64 *bitboard, int idx) {
  u64 mask = 1ULL << idx;
  *bitboard = (*bitboard) | mask;
}

///@brief map piece char to piece index (ex: 'p' or 'P' --> 0)
int utils::getPieceIdx(char ch) {
  ch = tolower(ch);
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

  // TODO: assert FEN string is invalid and exit
  default:
    return -1;
  }
  return -1;
}
