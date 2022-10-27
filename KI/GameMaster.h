#ifndef GAMEMASTER_H
#define GAMEMASTER_H

#include <memory>
#include "Field.h"

class GameMaster
{
public:
    GameMaster();

    enum class GameStatus
    {
        Running,
        Draw,
        HumanWon,
        AlgorithmWon
    };

    void printGame();
    bool playMove(int columnNumber, Field::Player player);
    GameStatus getStatus();
    Field getField();

private:
    Field m_field;
};

#endif