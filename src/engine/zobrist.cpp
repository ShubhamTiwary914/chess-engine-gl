
#include <utils/engine.h>
#include "./zobrist.h"



//#region zobrist
//> Zobrist Keys ---------------------------------------------------------

ZobristKeys::ZobristKeys(uint64_t keySeed = std::random_device{}()){
    this->zobristSeed = keySeed;
    this->generateRandom_zobristKeys();
}

u_int64_t ZobristKeys::getZobristHash(){
    return this->zobristHash;
}

void ZobristKeys::generateRandom_zobristKeys(){
    //Piece Keys
    for(int i=0; i<2; i++){
        for(int j=0; j<6; j++){
                for(int k=0; k<64; k++){
                this->pieceKeys[i][j][k] = generateRandomU64(this->zobristSeed);
                }
        }
    }
    //Castle Keys
    for(int i=0; i<4; i++)
        this->castleKeys[i] = generateRandomU64(this->zobristSeed);
    //en-passant keys.
    for(int i=0; i<17; i++)
        this->enPeassantkeys[i] = generateRandomU64(this->zobristSeed);
    //turn keys (0/1)
    for(int i=0; i<2; i++)
        this->turnKeys[i] = generateRandomU64(this->zobristSeed);
}