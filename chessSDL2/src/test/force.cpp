/*
 ?  Tester Methods to check against Subject methods

 * example:  
      - tester method:  a brute-force algorithm, slower but 100% corrrect to base off.alignas
      - subject method: optimal algorithm (usually the method needed to be tested)
*/


#include "./force.h"


uint64_t getRank_bruteForce(int rank){
    uint64_t bits = 0;
    for(int i=8*(rank-1); i<= (8*rank-1); i++){
        setBit(bits, i);
    }
    return bits;
}


uint64_t getFile_bruteForce(char file){
    uint64_t bits = 0;
    int fileID = file - 'a';
    for(int i=(7-fileID); i<=63; i+=8){
        setBit(bits, i);
    }
    return bits;
}


uint64_t getDiagonalLeft_bruteForce(char file, int rank){
    uint64_t bits = 0;
    char ftemp = file;
    int rtemp = rank;
    //down right
    while(file <= 'h' && rank >= 1){
        int index = getPieceBitIndex(file, rank);
        setBit(bits, index);
        file++;
        rank--;
    } 

    file = ftemp;
    rank = rtemp;

    //up left
    while(file >= 'a' && rank <= 8){
        int index = getPieceBitIndex(file, rank);
        setBit(bits, index);
        file--;
        rank++;
    } 
    //up left
    return bits;
}



uint64_t getDiagonalRight_bruteForce(char file, int rank){
    uint64_t bits = 0;
    char ftemp = file;
    int rtemp = rank;
    //up right
    while(file <= 'h' && rank <= 8){
        int index = getPieceBitIndex(file, rank);
        setBit(bits, index);
        file++;
        rank++;
    } 

    file = ftemp;
    rank = rtemp;

    //down left
    while(file >= 'a' && rank >= 1){
        int index = getPieceBitIndex(file, rank);
        setBit(bits, index);
        file--;
        rank--;
    } 
    //up left
    return bits;
}


uint64_t getEdgesMask_bruteForce(){
    //a1-a8
    uint64_t bits = 0;
    for(int i=7; i<=63; i+=8){
        setBit(bits, i);
    }
    //a8-h8
    for(int i=63; i>=56; i--){
        setBit(bits, i);
    }
    //h8-h1
    for(int i=56; i>=0; i-=8){
        setBit(bits, i);
    }
    //h1-a1
    for(int i=0; i<=7; i++){
        setBit(bits, i);
    }
    return bits;
}


