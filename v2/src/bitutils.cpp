#include "../lib/bitutills.h"

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


///@brief bitboard with 1 bit set -> find that bit's (rank,file)
std::pair<int,int> utils::fetchSinglebit_rankfile(u64 board){
  //only 1 set bit
  assert(__builtin_popcountll(board) == 1);
  int berfidx = __builtin_ctzll(board);
  int lerfidx = getLERFIndex(berfidx);
  return getLERF_rankfile(lerfidx);
}

///@brief bitboard with 1 bit set --> find that bit's LERF index
int utils::fetchSinglebit_bitidx(u64 board){
  int rank, file;
  std::tie(rank,file) = utils::fetchSinglebit_rankfile(board);
  return getLERFIndex(rank, file);
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

///@brief substract to get bitboardFrom - bitboardTargets (remove bits in from that are in targets)
///@returns bitboardFrom with the removed bits present in bitboardTargets as well
u64 utils::substractBit64(u64 bitboardFrom, u64 bitboardTargets){
  u64 mask = ~(bitboardFrom & bitboardTargets);
  return bitboardFrom & mask;
}


// ==============\/
//   OTHERS      \/
// ==============\/
///@brief top-left -> bottom right, print bit if piece in that pos exists
///(assumes LERF bitboard)
void utils::printBitBoard(u64 bitboard) {
  for (int rank = 7; rank >= 0; rank--) {
    std::cout << (rank+1) << " ";
    for (int file = 0; file < 8; file++) {
      int idx = utils::getLERFIndex(rank, file);
      std::cout << (int)checkBit64(bitboard, idx);
    }
    std::cout << '\n';
  }
  std::cout<<"  ";
  for(char file='a'; file<='h'; file++){
    std::cout << file;
  }
  std::cout<<'\n';
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


// ===================\\
//   BITRAY-GENERATION  \\
// ====================\/
///@brief generates ray cast for the file - horizontal 
///@param blockersMasked: assumes that the target board is masked to have only that file remaining
u64 utils::rayBrute_file(u64 blockersMasked, int smrank, int smfile){
  u64 resBoard = 0ULL;
  //cast down (move rank down, file is fixed)
  for(int rankidx=smrank-1; rankidx>=0; rankidx--){
    int idx = utils::getLERFIndex(rankidx, smfile);
    utils::setBit64(&resBoard, idx);
    if(utils::checkBit64(blockersMasked, idx))
      break;
  }
  //cast up (move rank up, file is fixed)
  for(int rankidx=smrank+1; rankidx<=7; rankidx++){
    int idx = utils::getLERFIndex(rankidx, smfile);
    utils::setBit64(&resBoard, idx);
    if(utils::checkBit64(blockersMasked, idx))
      break;
  }
  return resBoard;
}

u64 utils::rayBrute_file(u64 blockersMasked, int smidx){
  int smrank, smfile;
  std::tie(smrank, smfile) = utils::getLERF_rankfile(smidx);
  return utils::rayBrute_file(blockersMasked, smrank, smfile);
}


///@brief generates ray cast for the rank - horizontal (assumes that the target board is masked to have only that rank remaining)
u64 utils::rayBrute_rank(u64 blockersMasked, int smrank, int smfile){
  u64 resBoard = 0ULL;
  //cast left (move file left, rank fixed)
  for(int fileidx=smfile-1; fileidx>=0; fileidx--){
    int idx = utils::getLERFIndex(smrank, fileidx);
    utils::setBit64(&resBoard, idx);
    if(utils::checkBit64(blockersMasked, idx))
      break;
  }
  //cast left (move file right, rank fixed)
  for(int fileidx=smfile+1; fileidx<=7; fileidx++){
    int idx = utils::getLERFIndex(smrank, fileidx);
    utils::setBit64(&resBoard, idx);
    if(utils::checkBit64(blockersMasked, idx))
      break;
  }
  return resBoard;
}


u64 utils::rayBrute_rank(u64 blockersMasked, int smidx){
  int smrank, smfile;
  std::tie(smrank, smfile) = utils::getLERF_rankfile(smidx);
  return utils::rayBrute_rank(blockersMasked, smrank, smfile);
}

///@brief generates ray cast for diagonal - forward slash direction
///@param blockersMasked assumed blockers where only blockers in that diag exists, nothing else
u64 utils::rayBrute_diagonal(u64 blockersMasked, int smrank, int smfile){ // (direction: / )
  u64 resBoard = 0ULL;
  //cast top right(both rank, file moves+1)
  for(int ctr=1; ctr<8;ctr++){
    int rank=smrank+ctr, file=smfile+ctr;
    if(rank>7 || file>7)
      break;
    int idx = utils::getLERFIndex(rank, file);
    utils::setBit64(&resBoard, idx);
    if(utils::checkBit64(blockersMasked, idx))
      break;
  }
  //cast down left(both rank,file moves-1)
  for(int ctr=1; ctr<8;ctr++){
    int rank=smrank-ctr, file=smfile-ctr;
    if(rank>7 || file>7)
      break;
    int idx = utils::getLERFIndex(rank, file);
    utils::setBit64(&resBoard, idx);
    if(utils::checkBit64(blockersMasked, idx))
      break;
  }
  return resBoard;
}

u64 utils::rayBrute_diagonal(u64 blockersMasked, int smidx){ // (direction: \ )
  int smrank, smfile;
  std::tie(smrank, smfile) = utils::getLERF_rankfile(smidx);
  return utils::rayBrute_diagonal(blockersMasked, smrank, smfile);
} 

///@brief generates ray cast for diagonal - forward slash direction
///@param blockersMasked assumed blockers where only blockers in that diag exists, nothing else
u64 utils::rayBrute_antidiagonal(u64 blockersMasked, int smrank, int smfile){
  u64 resBoard = 0ULL;
  //cast top left(rank++, file--)
  for(int ctr=1; ctr<8;ctr++){
    int rank=smrank+ctr, file=smfile-ctr;
    if(rank>7 || file<0)
      break;
    int idx = utils::getLERFIndex(rank, file);
    utils::setBit64(&resBoard, idx);
    if(utils::checkBit64(blockersMasked, idx))
      break;
  }
  //cast down right(rank--, file++)
  for(int ctr=1; ctr<8;ctr++){
    int rank=smrank-ctr, file=smfile+ctr;
    if(rank<0 || file>7)
      break;
    int idx = utils::getLERFIndex(rank, file);
    utils::setBit64(&resBoard, idx);
    if(utils::checkBit64(blockersMasked, idx))
      break;
  }
  return resBoard;
}

u64 utils::rayBrute_antidiagonal(u64 blockersMasked, int smidx){ // (direction: \ )
  int smrank, smfile;
  std::tie(smrank, smfile) = utils::getLERF_rankfile(smidx);
  return utils::rayBrute_antidiagonal(blockersMasked, smrank, smfile);
}