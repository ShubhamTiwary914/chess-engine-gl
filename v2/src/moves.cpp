#include "../lib/moves.h"

int engine::knightDirectionsCount = 8;
int engine::knightDirections[8] = {
    15, 17, 6, 10,
    -15, -17, -6, -10
};

///@brief given current board condirtion --> generate moves possible for piece at pos:(rank,file) with side
///@param boardset current boardset struct ref
///@param side WHITE_SIDE or BLACK_SIDE?
///@param pieceidx (like PAWN_INDEX, ROOK_INDEX)
u64 engine::movesGeneration(engine::BoardSet *boardset, int rank, int file, int side, int pieceidx){
    assert(pieceidx>=0 && pieceidx<=6); 
    switch(pieceidx){
        case PAWN_INDEX:
            return engine::pawnMovesGeneration(boardset, rank, file, side);
        case KNIGHT_INDEX:
            return engine::knightMovesGeneration(boardset, rank, file, side);
        case KING_INDEX:
            return engine::kingMovesGeneration(boardset, rank, file, side);
        default:
            return 0ULL;
    }
    return 0ULL;
}

// ===================\/
//   JUMPING PIECES   \/ 
// ===================\/
//Jumping pieces: pawn,king, knight are generated at run-time in O(1), no precomp

///@brief moves generation for pawn - attack, move, en-passant conditions
u64 engine::pawnMovesGeneration(engine::BoardSet *boardset, int rank, int file, int currside){
    return 0ULL;
}

///brief moves generation for king (8 adjacent sides)
u64 engine::kingMovesGeneration(engine::BoardSet *boardset, int rank, int file, int currside){
    return 0ULL;
}

///@brief moves generation for knight (8 cells, 2 step in one dirn., then 1 step left/right) 
u64 engine::knightMovesGeneration(engine::BoardSet *boardset, int rank, int file, int currside){
    u64 oppsSideBitBoard = engine::getOppsSide_bitBoard(boardset, currside);
    u64 currentSideBitBoard = engine::getCurrentSide_bitBoard(boardset, currside);
    u64 moves = 0ULL;
    //mark cells where the knight attackable cells are in bounds 
    for(int idx=0; idx<engine::knightDirectionsCount; idx++){
        int bitidx = utils::getLERFIndex(rank, file);
        int newidx = bitidx + engine::knightDirections[idx];
        if(newidx>=0 && newidx<=63){
            utils::setBit64(&moves, newidx);
        }
    }
    //can't attack same side pieces
    return utils::substractBit64(moves, currentSideBitBoard);
}

// ===================\\
//   SLIDING PIECES   \\
// ===================\/
//Sliding pieces: rook,bishop,queen(OR combined of r,b) are generated at compile-time ~O(n^3), precomp O(1)

