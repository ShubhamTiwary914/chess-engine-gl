#include "lib/bitutills.h"
#include "lib/board.h"

int main(int argc, char **argv) {
  // utils::printBitBoard(FILE_A_BITS);

  engine::BoardSet boardset;
  engine::initBoardSet();

  engine::printStringBoard(boardset.stringboard);
  std::cout << '\n';

  u64 blackboard = engine::unionSideBitBoard(boardset.bitboard[BLACK_INDEX]);
  utils::printBitBoard(blackboard);

  std::cout << '\n';
  u64 whiteboard = engine::unionSideBitBoard(boardset.bitboard[WHITE_INDEX]);
  utils::printBitBoard(whiteboard);

  return 0;
};
