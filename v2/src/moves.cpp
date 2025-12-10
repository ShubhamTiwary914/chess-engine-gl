#include "../lib/moves.h"

// =========================\/
//   DIRECTION CONSTANTS    \/ 
// =========================\/
///constants - directions for jump pieces (pawn,king,knight)
int engine::knightDirectionsCount = 8;
int engine::knightDirections[8] = {
    15, 17, 6, 10,
    -15, -17, -6, -10
};
int engine::kingDirectionsCount = 8;
int engine::kingDirections[8] = {
    7,8,9,
    -1, 1, 
    -9, -8, -7
};
int engine::pawnAttackDirectionsCount = 2;
///@brief move forward for white, black
int engine::pawnMoveDirection[2] = {
    +8, -8
};
///@brief starter rank for white, black pawn (where it can move +2)
int engine::pawnStarterRank[2] = {
    1, 6
};
///@brief jump forward (starter rank only) for white, black
int engine::pawnStarterJumpDirection[2] = {
    +16, -16
};
///@brief pawn attackable directions for white, black
int engine::pawnAttackDirections[2][2]={
    {7,9},
    {-7, -9}
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
    for(int rank=7; rank>=0; rank--){
        for(int file=0; file<=7; file++){
            u64 moves = 0ULL;
            int bitidx = utils::getLERFIndex(rank, file);
            for(int diridx=0; diridx<engine::kingDirectionsCount; diridx++){
                int newidx = bitidx + engine::kingDirections[diridx];
                if(newidx>=0 && newidx<=63)
                    utils::setBit64(&moves, newidx);
            }
            cache->king[bitidx] = moves;
        }
    }
    cache->isCached[KING_INDEX]=true;
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
                if(newidx>=0 && newidx<=63)
                    utils::setBit64(&moves, newidx);
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
    //valid piece index, side index, rank, file + is pre-computed moves for that piece
    assert(pieceidx>=0 && pieceidx<=6);
    assert(sideidx == WHITE_INDEX || sideidx == BLACK_INDEX);
    assert((rank>=0 && rank<=7) && (file>=0 && file<=7));
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
    //piece can't "capture" opp's king
    moves = utils::substractBit64(moves, 
        board->bitboard[!sideidx][KING_INDEX]);
    return moves;
}

///@brief fetch the pawn move (special case) looking at conditions:
///@details normal: move +1, jump: move x2 if starter rank, attack +1(then left/right) if opponent piece exists there    
u64 engine::pawnMoves_fastfetch(CachedSet *cache, BoardSet *board, int rank, int file, int sideidx){
    u64 moves = 0ULL; 
    int bitidx = utils::getLERFIndex(rank, file);
    u64 bitboardFull = 
        unionSideBitBoard(board->bitboard[sideidx]) |
        unionSideBitBoard(board->bitboard[!sideidx]);
    //move forward direction
    int moveidx = bitidx + pawnMoveDirection[sideidx];
    if(moveidx>=0 && moveidx<=63)
        utils::setBit64(&moves, moveidx);
    //jump (+2 move) only if in starter rank & nothing blocking the jump
    if(rank == pawnStarterRank[sideidx]){
        int jumpidx = bitidx + pawnStarterJumpDirection[sideidx];
        if((jumpidx>=0 && jumpidx<=63) && (!utils::checkBit64(bitboardFull, moveidx)))
            utils::setBit64(&moves, jumpidx);
    }
    //attack direction(if opp piece exists there)
    u64 oppSidePieces = unionSideBitBoard(board->bitboard[!sideidx]);
    for(int diridx=0; diridx<engine::pawnAttackDirectionsCount; diridx++){
        //pawn attack direction is in bounds & only when opp piece exists 
        int attackidx = bitidx + pawnAttackDirections[sideidx][diridx];
        if(attackidx>=0 && attackidx<=63){
            if(utils::checkBit64(oppSidePieces, attackidx))
                utils::setBit64(&moves, attackidx);
        }
    }
    /// TODO: pawn en-pessant support 
    /// TODO: pawn promotion at end rank reached (black: rank-0) (white: rank-7)  
    return moves;
}

u64 engine::knightMoves_fastfetch(CachedSet *cache, BoardSet *board, int rank, int file, int sideidx){
    int bitidx = utils::getLERFIndex(rank, file);
    return cache->knight[bitidx];
}

u64 engine::kingMoves_fastfetch(CachedSet *cache, BoardSet *board, int rank, int file, int sideidx){
    int bitidx = utils::getLERFIndex(rank, file);
    u64 moves = cache->king[bitidx];
    //can't go in tile where opp king is also targeting (if opp king doesn't exist --> skip)
    u64 oppking_bitboard = board->bitboard[!sideidx][KING_INDEX];
    if(__builtin_popcountll(oppking_bitboard)>=1){
        int oppking_bitidx = utils::fetchSinglebit_bitidx(board->bitboard[!sideidx][KING_INDEX]);
        u64 oppkingmoves = cache->king[oppking_bitidx];
        moves = utils::substractBit64(moves, oppkingmoves);
    }
    return moves;
}