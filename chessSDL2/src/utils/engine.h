#ifndef GAMEUTILS_H
#define GAMEUTILS_H

#include <cassert>
#include <iostream>
#include <random>
#include <chrono>
#include <cmath>
#include <functional>
#include "./gui.h"
#include "consts.h"





int getPieceBitIndex(char file, int rank);
void println();
template<typename... Args>
std::string format(const std::string& fmt, Args... args);
uint64_t generateRandomU64(uint64_t seed);
double execPerft(const std::function<void()> &func, int precision);



//File(a-h) -> ID(0-7)
int parseFileID(char file);
bool isWhitePiece(char piece);
void switchTurn(int &turn);
void clearSelection(selectedPiece &selected);



//>Bitwise operations
//set ith bit from left  (assumes 0 based indexing)
void setBit(uint64_t &n, int i);

//get ith bit from left  (assumes 0 based indexing)
bool getBit(uint64_t n, int i);
//clear ith bit from left  (assumes 0 based indexing)
void clearBit(uint64_t &n, int i);
//toggle ith bit from left  (assumes 0 based indexing)
void toggle(uint64_t &n, int i);


uint64_t reverse_bits(uint64_t x);
uint64_t getFileBits(char file);
//rank = (1-8)
uint64_t getRankBits(int rank);
// diagonal in form: (/) - starts from bot right
uint64_t getDiagonalRightBits(char file, int rank);
// diagonal in form: (\) - starts from bot left
uint64_t getDiagonalLeftBits(char file, int rank);
uint64_t getEdgesMask();



//get 64bit number in bit representation (flat)
std::string get64Bits(uint64_t num);
//get 64 bit as 8x8 representation (bitboard form)
void printBitBoard(u_int64_t testboard, std::string title="");



#endif