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