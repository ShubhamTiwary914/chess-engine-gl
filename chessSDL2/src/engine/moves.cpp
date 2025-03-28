#include "./moves.h"
#include <bitset>

uint64_t rookAttackTable[64][4096];   
uint64_t bishopAttackTable[64][4096]; 
//bits in rook-mask/bishop-mask
int rookBits[64];    
int bishopBits[64];


//#region Precomputation

movesSetStruct generatePrecomputedMoves() {
    movesSetStruct  movesSet; 
    for(int rank=1; rank<=8; rank++) {
        for(int file='a'; file<='h'; file++) {
            int pos = getPieceBitIndex(file, rank);
            movesSet.moves[KING][pos] = getKingMoves(file, rank);
            movesSet.moves[PAWN][pos] = getPawnPrecomputedMoves(file, rank, WHITE_TURN);
            movesSet.blackPawn[pos] = getPawnPrecomputedMoves(file, rank, BLACK_TURN);
            movesSet.moves[KNIGHT][pos] = getKnightMoves(file, rank);
        }
    }
    return movesSet;
}


uint64_t getPrecomputedMove(movesSetStruct &movesSet, int pieceID, char file, int rank, bool turn) {
    assert(pieceID >= 0 && pieceID <= 6);  // Validate piece ID
    //special case: black pawn
    int index = getPieceBitIndex(file, rank);
    if(turn == BLACK_TURN && pieceID == PAWN)
        return movesSet.blackPawn[index];
    uint64_t move = movesSet.moves[pieceID][index];
    return move;
}


uint64_t filterMoveBlocks(char file, int rank, uint64_t precompMoves, int pieceType, int turn, 
    BitBoardSet &whiteboard, BitBoardSet &blackboard) {
    uint64_t sidePieces = (turn == WHITE_TURN) ? whiteboard.getUnion() : blackboard.getUnion();
    uint64_t oppPieces = (turn == WHITE_TURN) ? blackboard.getUnion() : whiteboard.getUnion();
    uint64_t allPieces = sidePieces | oppPieces;
    // jumping pieces (king, knight, pawn)
    if(pieceType == KING || pieceType == KNIGHT || pieceType == PAWN) {
        return (precompMoves & (~sidePieces));
    }
    uint64_t moves = 0;
    // Get moves using magic bitboards (for diagonals)
    if(pieceType == BISHOP || pieceType == QUEEN) {
        moves |= getMagicMoves(file, rank, allPieces, false);
    }
    if(pieceType == ROOK || pieceType == QUEEN) {
        //bruteforce for edges
        if(file == 'a' || file == 'h' || rank == 1 || rank == 8)
        moves |= getRookAttacks(file, rank, allPieces);
        //for non edges --> magic bitboards
        else
        moves |= getMagicMoves(file, rank, allPieces, true);
    }
    //can't capture self pieces or opponent's king 
    if(turn == WHITE_TURN)
        moves &= (~blackboard.getPiece(KING));
    else
        moves &= (~whiteboard.getPiece(KING));
    return moves & (~sidePieces);
}


U64 filterPawnMoves(char file, int rank, uint64_t precomp, int turn, 
    BitBoardSet &whiteboard, BitBoardSet &blackboard){
       return precomp; 
}



uint64_t getPawnPrecomputedMoves(char file, int rank, int turn=WHITE_TURN) {
    uint64_t moves = 0;
    int pos = getPieceBitIndex(file, rank);
    if(turn == WHITE_TURN) {
        // One square forward
        int newPos = pos + 8;
        if(newPos < 64) {
            setBit(moves, newPos);
            // Two squares if on starting rank (rank 2)
            if(rank == 2) {
                int newPos2 = newPos + 8;
                if (newPos2 < 64) setBit(moves, newPos2); 

            }
        }
    }
    else {  // BLACK_TURN
        //one square back
        int newPos = pos - 8;
        if(newPos >= 0) {
            setBit(moves, newPos);
            //two squares on start rank (rank 7)
            if(rank == 7) {
                int newPos2 = newPos - 8;
                if (newPos2 >= 0) setBit(moves, newPos2); 
            }
        }
    }
    return moves;
}



uint64_t getKnightMoves(char file, int rank) {
    uint64_t moves = 0;
    int pos = getPieceBitIndex(file, rank);
    for(int offset : KNIGHT_OFFSETS) {
        int newPos = pos + offset;
        if(newPos >= 0 && newPos < 64) {
            int oldFile = 7 - (pos % 8);
            int newFile = 7 - (newPos % 8);
            if(abs(oldFile - newFile) <= 2) {
                setBit(moves, newPos);
            }
        }
    }
    return moves;
}

uint64_t getKingMoves(char file, int rank) {
    uint64_t moves = 0;
    int pos = getPieceBitIndex(file, rank);
    
    for(int offset : KING_OFFSETS) {
        int newPos = pos + offset;
        if(newPos >= 0 && newPos < 64) {
            int oldFile = 7 - (pos % 8);
            int newFile = 7 - (newPos % 8);
            if(abs(oldFile - newFile) <= 1) {
                setBit(moves, newPos);
            }
        }
    }
    return moves;
}



//#region Magic Bitboards
void initializeMagicTables() {
    // Initialize tables to 0
    for(int rank = 1; rank <= 8; rank++) {
        for(char file = 'a'; file <= 'h'; file++) {
            int square = getPieceBitIndex(file, rank);
            for(int i = 0; i < 4096; i++) {
                rookAttackTable[square][i] = 0ULL;
                bishopAttackTable[square][i] = 0ULL;
            }
        }
    }
    // Initialize for each square
    for(int rank = 1; rank <= 8; rank++) {
        for(char file = 'a'; file <= 'h'; file++) {
            int square = getPieceBitIndex(file, rank);
            uint64_t rookMask = getRookMask(file, rank);
            uint64_t bishopMask = getBishopMask(file, rank);
            // rook attacks
            auto rookConfigs = generateBlockerConfigurations(rookMask);
            for(uint64_t blockers : rookConfigs) {
                uint64_t attacks = getRookAttacks(file, rank, blockers);
                uint64_t magicIndex = ((blockers) * ROOK_MAGICS[square]) >> (64 - 12);
                rookAttackTable[square][magicIndex] = attacks;
            }
            // bishop attacks        
            auto bishopConfigs = generateBlockerConfigurations(bishopMask);
            for(uint64_t blockers : bishopConfigs) {
                uint64_t attacks = getBishopAttacks(file, rank, blockers);
                uint64_t magicIndex = (blockers * BISHOP_MAGICS[square]) >> (64 - 12);
                bishopAttackTable[square][magicIndex] = attacks;
            }

        }
    }
}


uint64_t getMagicMoves(char file, int rank, uint64_t occupied, bool isRook) {
    int square = getPieceBitIndex(file, rank);
    uint64_t magic = isRook ? ROOK_MAGICS[square] : BISHOP_MAGICS[square];
    uint64_t mask = isRook ? getRookMask(file, rank) : getBishopMask(file, rank);
    uint64_t relevantBits = occupied & mask;
    uint64_t magicIndex = (relevantBits * magic) >> (64 - 12);
    return isRook ? rookAttackTable[square][magicIndex] : bishopAttackTable[square][magicIndex];
}





//#region Masks & Brute-Force


uint64_t getRookMask(char file, int rank, bool maskEdges) {
    int square = getPieceBitIndex(file, rank);
    uint64_t mask = getFileBits(file) | getRankBits(rank);
    if(maskEdges)
        mask &= ~getEdgesMask();
    clearBit(mask, square);
    return mask;
}


uint64_t getBishopMask(char file, int rank) {
    int square = getPieceBitIndex(file, rank);
    uint64_t mask = getDiagonalRightBits(file, rank) | getDiagonalLeftBits(file, rank);
    uint64_t edgeMask = getEdgesMask();
    mask &= ~edgeMask;
    clearBit(mask, square);
    return mask;
}


std::vector<uint64_t> generateBlockerConfigurations(uint64_t mask) {
    std::vector<uint64_t> configurations;
    int bits = __builtin_popcountll(mask);
    uint64_t limit = 1ULL << bits;
    
    for (uint64_t i = 0; i < limit; i++) {
        uint64_t blockers = 0ULL;
        uint64_t temp = i;
        uint64_t tempMask = mask;
        
        while (tempMask) {
            int lsb = __builtin_ctzll(tempMask);
            if (temp & 1) blockers |= (1ULL << lsb);
            temp >>= 1;
            tempMask &= tempMask - 1;
        }
        configurations.push_back(blockers);
    }
    return configurations;
}


uint64_t getRookAttacks(char file, int rank, uint64_t blockers) {
    uint64_t attacks = 0ULL;
    int square = getPieceBitIndex(file, rank);
    int r = rank;
    int f = parseFileID(file);

    // up
    for (int i = r + 1; i <= 8; i++) {
        int sq = getPieceBitIndex(file, i);
        attacks |= (1ULL << sq);
        if (blockers & (1ULL << sq)) break;
    }
    // down
    for (int i = r - 1; i >= 1; i--) {
        int sq = getPieceBitIndex(file, i);
        attacks |= (1ULL << sq);
        if (blockers & (1ULL << sq)) break;
    }
    // right
    for (char ff = file + 1; ff <= 'h'; ff++) {    
        int sq = getPieceBitIndex(ff, rank);
        attacks |= (1ULL << sq);
        if (blockers & (1ULL << sq)) break;
    }
    // left
    for (char ff = file - 1; ff >= 'a'; ff--) {    
        int sq = getPieceBitIndex(ff, rank);
        attacks |= (1ULL << sq);
        if (blockers & (1ULL << sq)) break;
    }
    return attacks;
}

uint64_t getBishopAttacks(char file, int rank, uint64_t blockers) {
    uint64_t attacks = 0ULL;
    int square = getPieceBitIndex(file, rank);
    int r = rank;  //rank's buffer
    int f = parseFileID(file);
    
    // Northeast
    for (int i = 1; f + i <= 7 && r + i <= 8; i++) {
        int sq = getPieceBitIndex((char)('a' + f + i), r + i);
        attacks |= (1ULL << sq);
        if (blockers & (1ULL << sq)) break;
    }
    // Northwest
    for (int i = 1; f - i >= 0 && r + i <= 8; i++) {
        int sq = getPieceBitIndex((char)('a' + f - i), r + i);
        attacks |= (1ULL << sq);
        if (blockers & (1ULL << sq)) break;
    }
    // Southeast
    for (int i = 1; f + i <= 7 && r - i >= 1; i++) {
        int sq = getPieceBitIndex((char)('a' + f + i), r - i);
        attacks |= (1ULL << sq);
        if (blockers & (1ULL << sq)) break;
    }
    // Southwest
    for (int i = 1; f - i >= 0 && r - i >= 1; i++) {
        int sq = getPieceBitIndex((char)('a' + f - i), r - i);
        attacks |= (1ULL << sq);
        if (blockers & (1ULL << sq)) break;
    }
    
    return attacks;
}


U64 rookAttacks_bitScan(U64 occ, char file, int rank){
    U64 rookFile_occ = getFileBits(file) & occ;
    U64 rookRank_occ = getRankBits(rank) & occ;
    int fileID = parseFileID(file);
    //directions [4x log(n)]
    int top = getFirstSetBit(rookFile_occ & ~RANK_MASKS[rank-1]); 
    int bot = getLastSetBit(rookFile_occ & (~(~RANK_MASKS[rank-1])) & (~getRankBits(rank)));
    int left = getLastSetBit(rookRank_occ & ~((~FILE_MASKS[fileID])|getFileBits(file)));
    int right = getFirstSetBit(rookRank_occ & (~FILE_MASKS[fileID]));

    U64 fullRay = getRookMask(file, rank, false);
    // under top
    if((rookFile_occ & ~RANK_MASKS[rank-1]))
        fullRay &= (RANK_MASKS[getrankInverseID(top)-1]);  
    //above bot
    if((rookFile_occ & (~(~RANK_MASKS[rank-1])) & (~getRankBits(rank))) && (getrankInverseID(bot) >= 2))
        fullRay &= (~RANK_MASKS[getrankInverseID(bot)-2]);
    //after left
    if((rookRank_occ & ~((~FILE_MASKS[fileID])|getFileBits(file))) && (getfileInverseID(left) >= 1))
        fullRay &= (~FILE_MASKS[getfileInverseID(left-1)]);
    //before right
    if(rookRank_occ & (~FILE_MASKS[fileID]))
        fullRay &= (FILE_MASKS[getfileInverseID(right)]); 
  
    return fullRay;
}