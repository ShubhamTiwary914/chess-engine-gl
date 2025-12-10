#include "board.h"
#include "state.h"

namespace engine{

extern int knightDirectionsCount;
extern int knightDirections[8];


void moves_precompute(CachedSet *cache);
void kingMoves_precompute(CachedSet *cache);
void knightMoves_precompute(CachedSet *cache);

u64 moves_fastfetch(CachedSet *cache, BoardSet *board, int rank, int file, int sideidx, int pieceidx);

u64 pawnMoves_fastfetch(CachedSet *cache, BoardSet *board, int rank, int file, int sideidx);
u64 knightMoves_fastfetch(CachedSet *cache, BoardSet *board, int rank, int file, int sideidx);
u64 kingMoves_fastfetch(CachedSet *cache, BoardSet *board, int rank, int file, int sideidx);

}
