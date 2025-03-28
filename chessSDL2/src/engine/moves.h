#ifndef MOVES_H
#define MOVES_H

#include <unordered_map>
#include <vector>
#include <iostream>
#include "engine/boards.h"
#include "utils/engine.h"
#include "utils/consts.h"


#define moveMapType std::unordered_map<int, uint64_t> 
/// @brief precomputed moves for jumping pieces + special(black pawn)
typedef struct {
    moveMapType moves[3]; 
    moveMapType blackPawn;
} movesSetStruct;


extern uint64_t rookAttackTable[64][4096];   
extern uint64_t bishopAttackTable[64][4096]; 
extern int rookBits[64];    
extern int bishopBits[64];




movesSetStruct generatePrecomputedMoves();
std::vector<uint64_t> generateBlockerConfigurations(uint64_t mask);
void initializeMagicTables();


/// @brief gets pre-computed paths (not accounted for blocks) move for a piece in a square (ex: Knight in D3)
/// @param movesSet pre-computed move set arrays (source: app/_engine)
/// @param pieceID piece enum (source: utils/consts)
/// @return bitboard of possible paths
uint64_t getPrecomputedMove(movesSetStruct &movesSet, int pieceID, char file, int rank, bool turn=WHITE_TURN);
uint64_t getPawnPrecomputedMoves(char file, int rank, int turn);
/// @brief handles special cases for pawn: move & attack are different, en passant, promotions.
/// @return 
U64 filterPawnMoves(char file, int rank, uint64_t precomp, int turn, 
    BitBoardSet &whiteboard, BitBoardSet &blackboard);
uint64_t getKnightMoves(char file, int rank);
uint64_t getKingMoves(char file, int rank);


/// @brief rank + file - self  (ex:  d3 - rank 3 & file 3, but not d3 itself)
uint64_t getRookMask(char file, int rank, bool maskEdges=true);
/// @brief diagonal (left + right skew) - self - edges  (ex:  d3 - diagonals through d3, but not d3, and edges)
uint64_t getBishopMask(char file, int rank);
uint64_t getRookAttacks(char file, int rank, uint64_t blockers);
uint64_t getBishopAttacks(char file, int rank, uint64_t blockers);
uint64_t getMagicMoves(char file, int rank, uint64_t occupied, bool isRook);


/// @brief bitmasks to get rook attacks in log(n) 
/// @param occ occupancy (total board union)
/// @return attack path of rook
U64 rookAttacks_bitScan(U64 occ, char file, int rank);


uint64_t filterMoveBlocks(char file, int rank, uint64_t precompMoves, int pieceType, int turn, BitBoardSet &whiteboard, BitBoardSet &blackboard);


#endif