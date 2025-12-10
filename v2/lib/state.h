#include "imports.h"
#include "types.h"
#include "board.h"

namespace engine{

 ///@brief precomputed move set for pieces evaluation to load
typedef struct {
    bool isCached[6];
    ///@param knight [knight-bit-index]cached moved for each possible tile 
    u64 knight[64];
    ///@param king [king-bit-index] cached moved for each possible tile
    u64 king[64];
    ///@param rook cached move for [rank=0,file=1][rank/file][rook's rank/file][blockers bits in that rank/file]
    u64 rook[2][8][8][128];
} CachedSet;   


};
