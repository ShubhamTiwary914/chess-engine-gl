#ifndef STATE_H
#define STATE_H

#include <utils/consts.h>
#include <vector>



class GameState
{
public:
    bool turn = WHITE_TURN;
    U8 check = 0;
    U8 enpassant = 0;
    U8 castling = 0;

    // history (over turns)
    std::string eventsFEN;
    U8 halfMoves = 0;
    U8 fullMoves = 0;
    U8 draw50Streak = 0;

    /// @brief  event to be added onto the event string of FEN strings.
    /// @param event FEN string (ex:  "Qxa7").
    void add_eventFEN(std::string event);

    /// @brief rollback event to a state with full move ID
    void rollback_event(U8 stateID);

    void move_state(std::string event);
};


#endif