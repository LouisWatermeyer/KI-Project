#include <string>
#include <iostream>
#include "GameMaster.h"

/**
 * Public constructor.
 *
 */
GameMaster::GameMaster()
{
}

/**
 * Prints the game to the console. Includes the text underneath the field.
 * 
 */
void GameMaster::printGame()
{
    std::string horizontalLine;

    for (int i = 0; i != m_field.width(); i++)
    {
        horizontalLine.append("----");
    }
    horizontalLine.append("-");
    std::cout << horizontalLine << std::endl;

    for (int rowNr = 0; rowNr != m_field.height(); rowNr++)
    {
        std::vector<char> row = m_field.getRow(rowNr);
        std::cout << "| ";
        for (char value : row)
        {
            std::cout << value << " | ";
        }

        std::cout << std::endl << horizontalLine << std::endl;
    }

    std::string numbersUnderField;
    for (int i = 1; i != m_field.width() + 1; i++)
    {
        numbersUnderField += "   " + std::to_string(i);
    }
    numbersUnderField.erase(numbersUnderField.begin());
    std::cout << numbersUnderField;
    std::cout << std::endl
        << std::endl
        << "Bitte die Nummer der Zeile eingeben, in der ein Stein plaziert werden soll."
        << std::endl;
    std::cout << " > ";
}

/**
 * Plays the specified move on the actual game field.
 * 
 * \param input The number of the column where the move is to be played.
 * \param player The Player making the move.
 * \return Returns true if the move was a success. False could mean a non existing column or a full column.
 */
bool GameMaster::playMove(int input, Field::Player player)
{
    return m_field.placeStone(input, player);
}

/**
 * Gives info about the status of the game.
 * 
 * \return returns the status of the game.
 */
GameMaster::GameStatus GameMaster::getStatus()
{
    if (m_field.isDraw())
        return GameMaster::GameStatus::Draw;
    else if (!m_field.isGameOver())
        return GameMaster::GameStatus::Running;
    else if (m_field.getWinner() == Field::Player::Human)
        return GameMaster::GameStatus::HumanWon;
    else
        return GameMaster::GameStatus::AlgorithmWon;
}

/**
 * Getter for the playing field.
 * 
 * \return Returns the playing field.
 */
Field GameMaster::getField()
{
    return m_field;
}
