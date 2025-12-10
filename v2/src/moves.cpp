#include "../lib/moves.h"

//constants - directions for jump pieces
int engine::knightDirectionsCount = 8;
int engine::knightDirections[8] = {
    15, 17, 6, 10,
    -15, -17, -6, -10
};

///@brief given current board condirtion --> generate moves possible for all pieces on the CachedSet
///@param cache store for all precomputed moves;
void engine::moves_precompute(engine::CachedSet *cache){
    //mark uncached first (except pawn, no cache needed)
    for(int pieceidx=0; pieceidx<6; pieceidx++)
        cache->isCached[pieceidx]=false;
    cache->isCached[PAWN_INDEX]=true;
    //jumping pieces: don't care about blockers
    engine::kingMoves_precompute(cache);
    engine::knightMoves_precompute(cache); 
    //sliding pieces: care about blockers
}


// =========================\/
//   MOVES PRE-COMPUTATION  \/ 
// =========================\/
///Storing moves in CachedSet for O(1) calls during runtime (except pawn special case)
///@brief moves generation for king (8 adjacent sides)
void engine::kingMoves_precompute(engine::CachedSet *cache){

}

///@brief moves generation for knight (8 cells, 2 step in one dirn., then 1 step left/right) 
void engine::knightMoves_precompute(engine::CachedSet *cache){
    for(int rank=7; rank>=0; rank--){
        for(int file=0; file<=7; file++){
            u64 moves = 0ULL;
            int bitidx = utils::getLERFIndex(rank, file);
            //mark cells where the knight attackable cells(8 possible ones) are in bounds 
            for(int diridx=0; diridx<engine::knightDirectionsCount; diridx++){
                int newidx = bitidx + engine::knightDirections[diridx];
                if(newidx>=0 && newidx<=63){
                    utils::setBit64(&moves, newidx);
                }
            }
            cache->knight[bitidx] = moves;
        }
    }
    cache->isCached[KNIGHT_INDEX]=true;
}

// ========================\\
//   MOVES RUNTIME FETCH   \\
// ========================\/
///Fetching cached move from CachedSet + Filtering moves (based on piece properties)  

u64 engine::moves_fastfetch(CachedSet *cache, BoardSet *board, int rank, int file, int sideidx, int pieceidx){
    //valid piece index + is pre-computed moves for that piece
    assert(pieceidx>=0 && pieceidx<=6);
    assert(cache->isCached[pieceidx]);
    u64 moves = 0ULL;
    switch (pieceidx) {
        case PAWN_INDEX:
            moves = pawnMoves_fastfetch(cache, board, rank, file, sideidx);
            break;
        case KNIGHT_INDEX:
            moves = knightMoves_fastfetch(cache, board, rank, file, sideidx);
            break;
        case KING_INDEX:
            moves = kingMoves_fastfetch(cache, board, rank, file, sideidx);
            break;
        default:
            return 0ULL;
    }
    //all pieces can't attack same side piece
    moves  = utils::substractBit64(moves, 
        unionSideBitBoard(board->bitboard[sideidx]));
    return moves;
}

u64 engine::pawnMoves_fastfetch(CachedSet *cache, BoardSet *board, int rank, int file, int sideidx){
    return 0ULL;
}

u64 engine::knightMoves_fastfetch(CachedSet *cache, BoardSet *board, int rank, int file, int sideidx){
    int bitidx = utils::getLERFIndex(rank, file);
    return cache->knight[bitidx];
}

u64 engine::kingMoves_fastfetch(CachedSet *cache, BoardSet *board, int rank, int file, int sideidx){
    return 0ULL;
}