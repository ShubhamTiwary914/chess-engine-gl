#include "./state.h"
#include <engine/moves.h>


void GameState::add_eventFEN(std::string event){

}


void GameState::rollback_event(U8 stateID){

}


void GameState::move_state(std::string event){
    //turn & +1up turn
    if(this->turn == BLACK_TURN)
        this->fullMoves++;
    this->halfMoves++;
    this->turn = !this->turn;

    //reset enpassant, add event
    this->enpassant = 0;
    add_eventFEN(event);
}