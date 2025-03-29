#include "./state.h"
#include <engine/moves.h>


void GameState::add_eventFEN(std::string event){

}


void GameState::rollback_event(U8 stateID){

}

/// @brief move game state (after turn is over)
void GameState::move_state(std::string event){ 
    //turn & +1up turn
    if(this->turn == BLACK_TURN)
    this->fullMoves++;
    this->halfMoves++;
    //remove en-passant after full move since set.
    bool enPassantSide = this->get_enPassant_side();
    if ( (enPassantSide == WHITE_TURN && turn == BLACK_TURN) ||
         (enPassantSide == BLACK_TURN && turn == WHITE_TURN) )
        this->enpassant = 0;      
    //switch turn and add event as FEN
    this->turn = !this->turn; 
    add_eventFEN(event);
}


bool GameState::get_enPassant_side(){
    return (bool)(this->enpassant & 8);
}

int GameState::get_enPassant_file(){
    return this->enpassant & 7;
}

bool GameState::is_enPassant(){
    return (bool)(this->enpassant & 128);
}


void GameState::set_enPassant(bool side, int fileID){
    setBit(this->enpassant, 7);
    if(side)
        setBit(this->enpassant, 3);
    this->enpassant += fileID; 
}


U8 GameState::checkCastlingAvailable(bool turn, bool leftCheck){
    U8 castlingSide;
    // castling & 00000011  (white)
    if(turn == WHITE_TURN)
        castlingSide = (this->castling & 3);
    // castling & 00001100 >> 2 (black)
    else
        castlingSide =  ((this->castling & 12) >> 2);
    // 00000011 >> 1 (left)
    if(leftCheck)
        return !(castlingSide >> 1);
    // 00000011 - 0000010 (right)
    clearBit(castlingSide, 1);
    return !castlingSide; 
}


void GameState::markCastling(bool turn, bool leftSide, bool whole){
   int pos = (turn == WHITE_TURN) ? 1 : 3;
   //mark both sides (king moved)
   if(whole){
        setBit(this->castling, pos); setBit(this->castling, pos-1);
        return;
   } 
   if(leftSide)
        setBit(this->castling, pos);
   else
        setBit(this->castling, pos-1);
}