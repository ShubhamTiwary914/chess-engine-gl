#include "../lib/board.h"
#include "../lib/bitutills.h"
#include <cctype>

// INFO: default starting pos constant
std::string FENstartpos =
    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

///@brief generates boardset with stringboard and bitboards from given fenstring
///@param fenString a FEN notation string, default: FENstartpos
engine::BoardSet engine::initBoardSet(std::string fenString) {
  // empty the string board & bitboards
  engine::BoardSet boardset;
  engine::clearBoardSet(&boardset);
  int fenidx = 0, bitidx = 63;
  int rank, file;
  while (fenidx < fenString.length() && fenString[fenidx] != ' ') {
    char ch = fenString[fenidx];
    // skip rank separator
    if (ch == '/') {
      fenidx++;
      continue;
      // skip empty squares when /8 or /x (x=int)
    } else if (isdigit(ch)) {
      bitidx -= (int)(ch - '0');
      fenidx++;
      continue;
    }
    // set piece in stringboard
    char piece = ch;
    std::tie(rank, file) = utils::getBERF_rankfile(bitidx);
    boardset.stringboard[rank][file] = piece;
    // set piece in bitboard
    int lerfidx = utils::getLERFIndex(bitidx);
    int pieceidx = utils::getPieceIdx((char)tolower(piece));
    // black piece
    if (islower(piece))
      utils::setBit64(&boardset.bitboard[BLACK_INDEX][pieceidx], lerfidx);
    // white piece
    else
      utils::setBit64(&boardset.bitboard[WHITE_INDEX][pieceidx], lerfidx);
    bitidx--;
    fenidx++;
  }
  return boardset;
}

///@brief empties the string & bitboards of a boardset
///@returns stringboard are 'x' for empty and bitboards are 0(unset)
void engine::clearBoardSet(BoardSet *boardset) {
  for (int rank = 7; rank >= 0; rank--) {
    for (int file = 0; file <= 7; file++) {
      boardset->stringboard[rank][file] = EMPTY_CHAR;
    }
  }
  for (int sideidx = 0; sideidx < 2; sideidx++) {
    for (int pieceidx = 0; pieceidx < 6; pieceidx++) {
      boardset->bitboard[sideidx][pieceidx] = 0ULL;
    }
  }
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
