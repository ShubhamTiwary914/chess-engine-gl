#ifndef MOVES_H
#define MOVES_H

#include <unordered_map>
#include <vector>
#include <iostream>
#include "engine/boards.h"
#include "utils/engine.h"
#include "utils/consts.h"



//pre-computed moves set for each piece
#define moveMapType std::unordered_map<int, uint64_t> 
typedef struct {
    moveMapType moves[6];  
} movesSetStruct;


extern uint64_t rookAttackTable[64][4096];   
extern uint64_t bishopAttackTable[64][4096]; 
extern int rookBits[64];    
extern int bishopBits[64];



/// @brief generate pre-computed: for jumping pieces
movesSetStruct generatePrecomputedMoves();
/// @brief  pass mask (ex: rook/bishop paths) --> get possible blockers in that path
std::vector<uint64_t> generateBlockerConfigurations(uint64_t mask);
void initializeMagicTables();


/// @brief gets pre-computed paths (not accounted for blocks) move for a piece in a square (ex: Knight in D3)
/// @param movesSet pre-computed move set arrays (source: app/_engine)
/// @param pieceID piece enum (source: utils/consts)
/// @return bitboard of possible paths
uint64_t getPrecomputedMove(movesSetStruct &movesSet, int pieceID, char file, int rank);
uint64_t getPawnMoves(char file, int rank, int turn);
uint64_t getKnightMoves(char file, int rank);
uint64_t getKingMoves(char file, int rank);
/// @brief rank + file - self  (ex:  d3 - rank 3 & file 3, but not d3 itself)
uint64_t getRookMoves(char file, int rank);
/// @brief diagonal (left + right skew) - self - edges  (ex:  d3 - diagonals through d3, but not d3, and edges)
uint64_t getBishopMoves(char file, int rank);
uint64_t getMagicMoves(char file, int rank, uint64_t occupied, bool isRook);


uint64_t getRookAttacks(char file, int rank, uint64_t blockers);
uint64_t getBishopAttacks(char file, int rank, uint64_t blockers);


uint64_t filterMoveBlocks(char file, int rank, uint64_t precompMoves, int pieceType, int turn, BitBoardSet &whiteboard, BitBoardSet &blackboard);


#endif