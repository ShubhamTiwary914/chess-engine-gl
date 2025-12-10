#include "board.h"

namespace engine{

extern int knightDirectionsCount;
extern int knightDirections[8];


u64 movesGeneration(engine::BoardSet *boardset, int rank, int file, int side, int pieceidx);

u64 pawnMovesGeneration(engine::BoardSet *boardset, int rank, int file, int currside);
u64 kingMovesGeneration(engine::BoardSet *boardset, int rank, int file, int currside);
u64 knightMovesGeneration(engine::BoardSet *boardset, int rank, int file, int currside);



}
