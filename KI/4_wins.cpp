#include <windows.h>
#include <iostream>
#include <string>

#include "ConsoleHandler.h"
#include "GameMaster.h"
#include "CustomDefines.h"
#include "Field.h"
#include "Algorithm.h"

int main(void)
{
    // Create a GameMaster to initialize the game
    GameMaster gameMaster;
    Field::Player nextTurnBy = Field::Player::Human;

    Algorithm* algorithm = Algorithm::getInstance();

    // Game loop
    while (gameMaster.getStatus() == GameMaster::GameStatus::Running)
    {
        ConsoleHandler::getInstance()->clearScreen();
        gameMaster.printGame();
        bool turnCompleted = false;

        if (nextTurnBy == Field::Player::Human)
        {
            // Get the move of the human and execute it
            std::string input;
            std::cin >> input;

            try
            {
                turnCompleted = gameMaster.playMove(std::stoi(input), Field::Player::Human);
            }
            catch (const std::invalid_argument& e)
            {
                // We don't have to do anything. The user probably entered a letter, the turn will not be completed,
                // hence it will re loop.
                UNUSED(e);
            }
        }
        else if (nextTurnBy == Field::Player::Algorithm)
        {
            // Algorithm's turn
            turnCompleted = gameMaster.playMove(algorithm->getNextMove(gameMaster.getField()),
                Field::Player::Algorithm);
        }

        // After the move prepare for the next turn.
        if (turnCompleted && nextTurnBy == Field::Player::Human)
            nextTurnBy = Field::Player::Algorithm;
        else if (turnCompleted)
            nextTurnBy = Field::Player::Human;
    }

    ConsoleHandler::getInstance()->clearScreen();
    gameMaster.printGame();

    std::string winner;
    if (gameMaster.getStatus() == GameMaster::GameStatus::Draw)
        std::cout << "The game ended in a draw." << std::endl;
    else if (gameMaster.getStatus() == GameMaster::GameStatus::HumanWon)
        std::cout << "You won!!!";
    else
        std::cout << "The bot won!!!";

    return 0;
}