#include "./moves.h"
#include <bitset>

uint64_t rookAttackTable[64][4096];   
uint64_t bishopAttackTable[64][4096]; 
//bits in rook-mask/bishop-mask
int rookBits[64];    
int bishopBits[64];
//{{white}, {black}}
bool pawnAttacksSet = false;
int pawnAttackDirections[2][2] = {{7, 9},{-9,-7}};


//#region Precomputation
movesSetStruct generatePrecomputedMoves() {
    movesSetStruct  movesSet; 
    for(int rank=1; rank<=8; rank++) {
        for(int file='a'; file<='h'; file++) {
            int pos = getPieceBitIndex(file, rank);
            movesSet.moves[KING][pos] = getKingMoves(file, rank);
            //pawn
            movesSet.moves[PAWN][pos] = getPawnPrecomputedMoves(file, rank, WHITE_TURN);
            movesSet.blackPawn[pos] = getPawnPrecomputedMoves(file, rank, BLACK_TURN);
            movesSet.pawnAttacks[WHITE_TURN][pos] = getPawnPrecomputedAttacks(file, rank, WHITE_TURN);
            movesSet.pawnAttacks[BLACK_TURN][pos] = getPawnPrecomputedAttacks(file, rank, BLACK_TURN);
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
    BitBoardSet &whiteboard, BitBoardSet &blackboard, movesSetStruct &movesSet, GameState &state){
    int pos = getPieceBitIndex(file, rank);
    int srcfile = parseFileID(file);
    int blocker;  
    //captures -> if target tiles has opp piece (non king) 
    if(turn == WHITE_TURN){
        precomp |= (blackboard.getUnion() & movesSet.pawnAttacks[turn][pos]);
        blocker = pos + 8;
    }
    else if(turn == BLACK_TURN){
        precomp |= (whiteboard.getUnion() & movesSet.pawnAttacks[turn][pos]);
        blocker = pos - 8;
    }
    //forward piece blocker -> don't capture
    if(blocker >= 0 && blocker <= 63){
        if(
            (turn == WHITE_TURN && getBit(blackboard.getUnion(), blocker)) ||
            (turn == BLACK_TURN && getBit(whiteboard.getUnion(), blocker)) 
        )
            clearBit(precomp, blocker);
    }
    //en-passant 
    if(state.is_enPassant() && state.get_enPassant_side() != turn){
        int targetfile = state.get_enPassant_file();
        int vertpos = (turn == WHITE_TURN) ? 8 : -8;
        int horipos;
        //right by 1
        if(abs(targetfile-srcfile)==1)
            horipos = (targetfile - srcfile);
        //check if target is valid (fix for rank match check)
        bool isValidPawn = (turn == WHITE_TURN) ?  
            (getBit(blackboard.getPiece(PAWN), pos+horipos)) :  
            (getBit(whiteboard.getPiece(PAWN), pos+horipos)); 
        //confirm, so allow attack
        if(isValidPawn)
            setBit(precomp, pos+vertpos+horipos);
    }
    return precomp;
}



uint64_t getPawnPrecomputedMoves(char file, int rank, int turn=WHITE_TURN) {
    uint64_t moves = 0;
    int pos = getPieceBitIndex(file, rank);
    if(turn == WHITE_TURN) {
        //square forward
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
            //two squares on start rank (rank 7))
            if(rank == 7) {
                int newPos2 = newPos - 8;
                if (newPos2 >= 0) setBit(moves, newPos2); 
            }
        }
    } 
    return moves;
}


U64 getPawnPrecomputedAttacks(char file, int rank, bool turn){
    int pos = getPieceBitIndex(file, rank);
    U64 moves = 0;
    for(int steps: pawnAttackDirections[turn]){
        int nextpos = pos + steps;
        int prevpos = pos - steps;
        if(nextpos <= 63)
            setBit(moves, nextpos);
        if(prevpos >= 0)
            setBit(moves, prevpos);
        //fix: pawn capturing on other side of edge (still in edge but not valid) 
        if(file == 'a' || file == 'h')
            moves &= ~(getFileBits('h') | getFileBits('a'));
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

//check castling -> return marks to add if set
U64 getKingCastlingCheck(char file, int rank, BitBoardSet &whiteboard, BitBoardSet &blackboard, GameState &state){
    U64 allPieces = whiteboard.getUnion() | blackboard.getUnion();
    U64 rankMask = (state.turn == WHITE_TURN) ? RANK_1_BITS : (RANK_1_BITS << 56);
    U64 kingrook_leftMask = (state.turn == WHITE_TURN) ? KING_LEFT_ROOK_RANK_1 : KING_LEFT_ROOK_RANK_8;
    U64 kingrook_rightMask = (state.turn == WHITE_TURN) ? KING_RIGHT_ROOK_RANK_1 : KING_RIGHT_ROOK_RANK_8;
    U64 moves = 0Ull;
    int bitID = getPieceBitIndex(file, rank);
    //left castling available + space free
    if(state.checkCastlingAvailable(state.turn, true) && !(allPieces & rankMask & kingrook_leftMask)){
       setBit(moves, bitID-2);
    }
    //right castling available + space free
    if(state.checkCastlingAvailable(state.turn) && !(allPieces & rankMask & kingrook_rightMask) ){
        setBit(moves, bitID+2);
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