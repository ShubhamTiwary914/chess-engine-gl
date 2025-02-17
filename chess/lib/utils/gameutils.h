#ifndef GAMEUTILS_H
#define GAMEUTILS_H

#include <cassert>
#include <iostream>
#include <random>


inline void breakpoint(bool condition, const std::string &message) {
    if (!condition) {

        std::cerr << "Breakpoint: " << message << std::endl;
        assert(condition);
    }
}

#define BREAKPOINT(condition, message) breakpoint(condition, message)
#define isLower(ch) (ch>='a' && ch<='z');


void println(){
  std::cout << std::endl;
}



template<typename... Args>
std::string format(const std::string& fmt, Args... args) {
    int size = snprintf(nullptr, 0, fmt.c_str(), args...) + 1; //+1: null (/0) terminator
    if (size <= 0) { return ""; }
    std::vector<char> buf(size);
    snprintf(buf.data(), size, fmt.c_str(), args...);
    return std::string(buf.data(), buf.data() + size - 1);
}


uint64_t generateRandomU64(uint64_t seed = std::random_device{}()) {
    static std::mt19937_64 gen(seed);
    return gen();
}


//File -> ID
int parseFileID(char file){
  //File = A, B, C, ..., H   (lower or upper)
  assert(file >= 'a' && file <= 'h' || file >= 'A' && file <= 'H'); 
  
  if(file >= 'a' && file <= 'h')
    return (int)(file - 'a');
  else if(file >= 'A' && file <= 'H')
    return (int)(file - 'A');
  return -1;  
}



//>Bitwise operations
//set ith bit from left 
void setBit(uint64_t &n, int i){
   n = (n | (1ULL<<(i-1)));
}

bool getBit(uint64_t n, int i){
   return (n & (1ULL << (i - 1))) != 0;
}

void clearBit(uint64_t &n, int i){
  n = (n & ~(1ULL << (i-1)));
}

void toggle(uint64_t &n, int i){
  n = (n ^ (1ULL << (i-1)));
}


//convert (rank, file) in 2D -> 1D bit index (for boards)
//rank = row(1-8), file = col(A-H)
int getPieceBitIndex(char file, int rank){
    assert(rank >= 1 && rank <= 8);
    int fileId = parseFileID(file);
    int bitIndex = (8*(rank)-1)-fileId; 
    return bitIndex;
}


#endif