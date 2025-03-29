#ifndef STATE_H
#define STATE_H

#include <utils/consts.h>
#include <vector>



class GameState
{
public:
    bool turn = WHITE_TURN;
    U8 check = 0;
    /// @brief shows file of piece + side that did en-passant (last one full move)
    /// @details n000kxxx (8-bit) = (n = on/off) (k = side) & (xxx = file i.e: 0-7)
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

    /// @brief move game state (after turn is over)
    void move_state(std::string event);

    //> en-passant

    /// @brief n000kxxx & 00001000 (8)
    bool get_enPassant_side();

    /// @brief n000kxxx & 00000111 (7)
    int get_enPassant_file();

    /// @brief n000kxxx & 10000000 (128)
    bool is_enPassant();

    void set_enPassant(bool side, int fileID);
};


#endif