#include "engine.h"



void println(){
    std::cout << std::endl;
}

std::string itoa(int val, int base){
  std::string res = "";
  assert(base >= 2 && base <= 36);
  //neg-val for base-10 only
  bool isNeg = (val < 0 && base == 10);
  val = (isNeg) ? -val : val;

  do{
    int digit = val % base;
    res = (digit > 9) ? ((char)('A' + digit - 10) + res) : ((char)('0' + digit) + res);
    val /= base;
  }while(val);
  //add neg-val symbol back + null-end
  if(isNeg) res = '-' + res;
  res += '\0';
  return res;
}


// template<typename... Args>
// std::string format(const std::string& fmt, Args... args) {
//     int size = snprintf(nullptr, 0, fmt.c_str(), args...) + 1; //+1: null (/0) terminator
//     if (size <= 0) { return ""; }
//     std::vector<char> buf(size);
//     snprintf(buf.data(), size, fmt.c_str(), args...);
//     return std::string(buf.data(), buf.data() + size - 1);
// }


uint64_t generateRandomU64(uint64_t seed = std::random_device{}()) {
    static std::mt19937_64 gen(seed);
    return gen();
}


double execPerft(const std::function<void()> &func, int precision=2){
  auto start = std::chrono::high_resolution_clock::now();
  func();
  auto end = std::chrono::high_resolution_clock::now();
  
  std::chrono::duration<double> timer = (end - start);
  double factor = std::pow(10, precision);
  return std::round(timer.count()*factor)/factor; 
}



int parseFileID(char file){
  //File = A, B, C, ..., H   (lower or upper)
  assert(file >= 'a' && file <= 'h' || file >= 'A' && file <= 'H'); 
  if(file >= 'a' && file <= 'h')
    return (int)(file - 'a');
  else if(file >= 'A' && file <= 'H')
    return (int)(file - 'A');
  return -1;  
}

bool isWhitePiece(char piece){
  assert(piece >= 'a' && piece <= 'z' || piece >= 'A' && piece <= 'Z');
  if(piece >= 'a' && piece <= 'z') 
    return false;
  return true;
}





void switchTurn(int &turn){
  turn = ((turn == 0) ? 1 : 0);
}

void clearSelection(selectedPiece &selected){
  selected.isSelected = false;
  selected.moves = 0;
}



//>Bitwise operations
//set ith bit from left  (assumes 0 based indexing)
void setBit(uint64_t &n, int i){
   n = (n | (1ULL<<(i)));
}

//get ith bit from left  (assumes 0 based indexing)
bool getBit(uint64_t n, int i){
   return (n & (1ULL << (i))) != 0;
}
//clear ith bit from left  (assumes 0 based indexing)
void clearBit(uint64_t &n, int i){
  n = (n & ~(1ULL << (i)));
}
//toggle ith bit from left  (assumes 0 based indexing)
void toggle(uint64_t &n, int i){
  n = (n ^ (1ULL << (i)));
}


uint64_t reverse_bits(uint64_t x) {
    x = ((x >> 1)  & 0x5555555555555555) | ((x & 0x5555555555555555) << 1);
    x = ((x >> 2)  & 0x3333333333333333) | ((x & 0x3333333333333333) << 2);
    x = ((x >> 4)  & 0x0F0F0F0F0F0F0F0F) | ((x & 0x0F0F0F0F0F0F0F0F) << 4);
    x = ((x >> 8)  & 0x00FF00FF00FF00FF) | ((x & 0x00FF00FF00FF00FF) << 8);
    x = ((x >> 16) & 0x0000FFFF0000FFFF) | ((x & 0x0000FFFF0000FFFF) << 16);
    x = (x >> 32) | (x << 32);
    return x;
}


uint64_t getFileBits(char file){
  int fileID = parseFileID(file); 
  return FILE_A_BITS >> (fileID);
}

//rank = (1-8)
uint64_t getRankBits(int rank){
  return RANK_1_BITS << (8*(rank-1));
}

// diagonal in form: (/) - starts from bot right
uint64_t getDiagonalRightBits(char file, int rank){
  int fileID = parseFileID(file);
  int diagID = (7-fileID) + rank;
  BREAKPOINT(diagID >= 1 && diagID <= 15, "Diagonal bounds err for: /");
  if(diagID <= 8){
    return (DIAGONAL_RIGHT_BASE >> (8*abs(8-diagID)) );
  }else{
    return (DIAGONAL_RIGHT_BASE << (8*abs(8-diagID)) );
  }
}
// diagonal in form: (\) - starts from bot left
uint64_t getDiagonalLeftBits(char file, int rank){
  int fileID = parseFileID(file);
  int diagID = fileID + rank; 
  BREAKPOINT(diagID >= 1 && diagID <= 15, "Diagonal bounds err for left-diag");
  if(diagID <= 8){
    return (DIAGONAL_LEFT_BASE >> (8*abs(8-diagID)) );
  }else{
    return (DIAGONAL_LEFT_BASE << (8*abs(8-diagID)) );
  }
}

/// @brief corners bits: (first & last) ranks + files
uint64_t getEdgesMask() {
    return (RANK_1_BITS | (RANK_1_BITS << 56) | FILE_A_BITS | (FILE_A_BITS >> 7));
}




//convert (rank, file) in 2D -> 1D bit index (for boards)
//rank = row(1-8), file = col(A-H)
int getPieceBitIndex(char file, int rank){
    assert(rank >= 1 && rank <= 8);
    int fileId = parseFileID(file);
    int bitIndex = (8*(rank)-1)-fileId; 
    return bitIndex;
}



//get 64bit number in bit representation (flat)
std::string get64Bits(uint64_t num){
  std::string bits = "";
  for(int i=0; i<64; i++){
     if(num&1) bits = "1" + bits; 
     else  bits = "0" + bits;
     num>>=1;
  }
  return bits;
}



//get 64 bit as 8x8 representation (bitboard form)
void printBitBoard(u_int64_t testboard){
    std::string bits = "";
    for(int rank=8; rank>=1; rank--){
        bits += itoa(rank, 10) + "   ";
        for(int file=65; file<= 72; file++){
            int bitIndex = getPieceBitIndex(file, rank);
            if(getBit(testboard, bitIndex)){
                bits += "1 ";
            }else {
                bits += "0 ";
            }   
        }
        bits += "\n";
    }
    bits += "\n    a b c d e f g h";
    std::cout << bits;
} 
