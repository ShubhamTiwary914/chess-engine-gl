#include "../lib/board.h"
#include "../lib/bitutills.h"
#include <cctype>

// INFO: default starting pos constant
std::string FENstartpos =
    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

///@brief generate the
engine::BoardSet engine::initBoardSet(std::string fenString) {
  engine::BoardSet boardset;
  // empty the string board
  for (int rank = 7; rank >= 0; rank--) {
    for (int file = 0; file <= 7; file++) {
      boardset.stringboard[rank][file] = EMPTY_CHAR;
    }
  }

  int fenidx = 0, bitidx = 63;
  int rank, file;
  // set bits for bitboards + char for string board
  // BUG:  stringboard is fine, bitboard aren't showing up okay
  while (fenidx < fenString.length() && fenString[fenidx] != ' ') {
    char ch = fenString[fenidx];
    std::tie(rank, file) = utils::getBERF_rankfile(bitidx);
    if (ch == '/') {
      // skip rank separator
      fenidx++;
      continue;
    } else if (isdigit(ch)) {
      // skip empty squares when /8 or /x (x=int)
      bitidx -= (ch - '0');
    } else {
      // set piece in stringboard
      boardset.stringboard[rank][file] = ch;
      // set piece in bitboard
      int lerfidx = utils::getLERFIndex(bitidx);
      char pieceidx = utils::getPieceIdx(ch);
      // black piece
      if (islower(ch)) {
        utils::setBit64(&boardset.bitboard[BLACK_INDEX][pieceidx], lerfidx);
      }
      // white piece
      else {
        utils::setBit64(&boardset.bitboard[WHITE_INDEX][pieceidx], lerfidx);
      }
      bitidx--;
    }
    fenidx++;
  }

  return boardset;
}

void engine::printStringBoard(char stringboard[8][8]) {
  for (int rank = 7; rank >= 0; rank--) {
    for (int file = 0; file <= 7; file++) {
      std::cout << stringboard[rank][file];
    }
    std::cout << std::endl;
  }
}

///@brief make union of one side, white or black to get full side bitboard
u64 engine::unionSideBitBoard(u64 bitboard_sidepieces[6]) {
  // TODO: check one cell doesn't have two entires -> invalid

  u64 sidebitboard = 0ULL;
  // union with biwise OR(|)
  for (int pieceidx = 0; pieceidx < 6; pieceidx++)
    sidebitboard |= bitboard_sidepieces[pieceidx];
  return sidebitboard;
}
