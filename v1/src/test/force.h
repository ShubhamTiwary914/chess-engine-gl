#ifndef FORCE_H
#define FORCE_H

#include <iostream>
#include <utils/engine.h>
#include <cstdint>

uint64_t getRank_bruteForce(int rank);
uint64_t getFile_bruteForce(char file);
uint64_t getDiagonalLeft_bruteForce(char file, int rank);
uint64_t getDiagonalRight_bruteForce(char file, int rank);
uint64_t getEdgesMask_bruteForce();



#endif 
