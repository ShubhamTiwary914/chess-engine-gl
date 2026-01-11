#include <cstdint>
#include <gtest/gtest.h>
#include "../lib/bitutills.h"


///@brief converting BERF index to LERF index
///ex: 0->7, 4->3, 7->0, 56->63, ... (i.e: reverse the columns)
TEST(BitUtils, GetLerfIndex){
    for(int k=0; k<=56; k+=8){
        int berfidx=k; 
        int lerfidx=k+7;
        while(lerfidx<=k){
            EXPECT_EQ(lerfidx, utils::getLERFIndex(berfidx));
            lerfidx--;
            berfidx++;
        }
    }
}

///@brief test berf rank, file conversion (assuming both getLerfIndex overloads are equivalent)
TEST(BitUtils, GetLERF_rankfile){
    for(int bigidx=0; bigidx<64; bigidx++){
        int rank, file;
        std::tie(rank, file) = utils::getLERF_rankfile(bigidx);
        EXPECT_EQ(utils::getLERFIndex(bigidx), utils::getLERFIndex(rank, file));
    }
}

///@brief test the u64 setter from all powers of 2 from 1-2^63
TEST(BitUtils, SetBit64){
    uint64_t num = 1ULL;
    for(int idx=0; idx<63; idx++){
        uint64_t tmp = 0ULL;
        utils::setBit64(&tmp, idx);
        EXPECT_EQ(num, tmp);
        num *= 2;
    }
}

///@brief substracting bits for: A - B
TEST(BitUtils, SubtractBit64) {
    // remove one bit
    u64 from1    = 0b100100;
    u64 targets1 = 0b000100;
    EXPECT_EQ(utils::substractBit64(from1, targets1), 0b100000);

    // remove multiple bits
    u64 from2    = 0b111011;
    u64 targets2 = 0b011001;
    EXPECT_EQ(utils::substractBit64(from2, targets2), 0b100010);

    // removing none
    u64 from3    = 0b101010;
    u64 targets3 = 0;
    EXPECT_EQ(utils::substractBit64(from3, targets3), 0b101010);

    // removing all
    u64 from4    = 0xFFFFULL;
    u64 targets4 = 0xFFFFULL;
    EXPECT_EQ(utils::substractBit64(from4, targets4), 0ULL);

    // removing bits not present in from
    u64 from5    = 0b0011;
    u64 targets5 = 0b1100;
    EXPECT_EQ(utils::substractBit64(from5, targets5), 0b0011);
}


///@brief ensure no indexes of pieces have been changed
TEST(BitUtils, PiecesIndexes){
    EXPECT_EQ(utils::getPieceIdx('p'), 0);
    EXPECT_EQ(utils::getPieceIdx('n'), 1);
    EXPECT_EQ(utils::getPieceIdx('k'), 2);
    EXPECT_EQ(utils::getPieceIdx('r'), 3);
    EXPECT_EQ(utils::getPieceIdx('b'), 4);
    EXPECT_EQ(utils::getPieceIdx('q'), 5);
}

TEST(BitUtils, FilesMasking){
    uint64_t expected = 0xB5B5B5B5B5B5B5B5ULL;
    u64 masked = utils::maskFiles({0,2,4,5,7,7});
    EXPECT_EQ(expected, masked);
}

TEST(BitUtils, RanksMasking){
    uint64_t expected = 0ULL;
    u64 masked = utils::maskRanks({0,1,2,4,6,7});
}


TEST(BitUtils, RayBrute_file){
    u64 targetBoard = 0ULL;
    utils::setBit64(&targetBoard, b1);
    utils::setBit64(&targetBoard, b3);
    utils::setBit64(&targetBoard, b6);
    utils::setBit64(&targetBoard, b7);
    utils::setBit64(&targetBoard, b8);
    
    u64 resBoard = utils::rayBrute_file(targetBoard, b5);
    u64 expected = 0ULL;
    utils::setBit64(&expected,  b3);
    utils::setBit64(&expected,  b4);
    utils::setBit64(&expected,  b6);
    
    EXPECT_EQ(resBoard, expected);
}

TEST(BitUtils, RayBrute_rank){
    u64 targetBoard = 0ULL;
    utils::setBit64(&targetBoard, a5);
    utils::setBit64(&targetBoard, g5);
    utils::setBit64(&targetBoard, h5);

    u64 resBoard = utils::rayBrute_rank(targetBoard, c5);
    u64 expected = 0ULL;
    utils::setBit64(&expected,  b5);
    utils::setBit64(&expected,  d5);
    utils::setBit64(&expected,  e5);
    utils::setBit64(&expected,  f5);
    utils::setBit64(&expected,  a5);
    utils::setBit64(&expected,  g5);

    EXPECT_EQ(resBoard, expected);
}