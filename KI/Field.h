#ifndef FIELD_H
#define FIELD_H

#include <vector>

constexpr auto FIELD_WIDTH = 7;
constexpr auto FIELD_HEIGHT = 6;
constexpr auto WIN_NR = 4;
constexpr auto HUMAN_SYMBOL = 'X';
constexpr auto ALGOTITHM_SYMBOL = 'O';
constexpr auto FREE_SPACE_SYMBOL = ' ';

class Field
{
public:
    Field();

    enum class Player
    {
        Human,
        Algorithm
    };

    enum class GameState
    {
        Running,
        Draw,
        HumanWon,
        AlgorythmWon
    };

    std::vector<char> getRow(int rowNr);
    std::vector<char> getColumn(int columnNr);

    bool placeStone(int columnNr, Player player);
    bool isGameOver();
    bool isDraw();
    bool isMovePossible(int columnNr);
    Player getWinner();

    int height();
    int width();

private:
    void checkWin();

    std::vector<std::vector<char>>  m_fieldState;
    GameState                       m_gameState         = GameState::Running;
    bool                            m_win               = false;
    Player                          m_winner;
    int                             m_lastMoveColumn    = 0;
    int                             m_lastMoveRow       = 0;
};

#endif