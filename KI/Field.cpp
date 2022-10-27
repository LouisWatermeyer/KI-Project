#include "Field.h"

/**
 * Public constructor.
 *
 */
Field::Field()
{
    // Initialize the field with free spaces
    m_fieldState = std::vector<std::vector<char>>(FIELD_HEIGHT,
        std::vector<char>(FIELD_WIDTH,
            FREE_SPACE_SYMBOL));
}

/**
 * Returns a vector representing the requested row. Row number zero is the topmost row.
 *
 * \param rowNr is the number of the requested row from top to bottom starting at zero.
 * \return Returns a vector representing the symbols in the requested row.
 */
std::vector<char> Field::getRow(int rowNr)
{
    if (rowNr <= 0 && rowNr >= FIELD_HEIGHT)
        return std::vector<char>();

    return m_fieldState.at(rowNr);
}

/**
 * Returns a vector representing the requested column. Column number zero is the leftmost column.
 *
 * \param columnNr is the number of the requested column from left to right starting at zero.
 * \return Returns a vector representing the symbols in the requested column.
 */
std::vector<char> Field::getColumn(int columnNr)
{
    if (columnNr < 0 || columnNr > FIELD_WIDTH - 1)
        return std::vector<char>();

    std::vector<char> returnValue;
    for (std::vector<char> row : m_fieldState)
    {
        returnValue.push_back(row.at(columnNr));
    }

    return returnValue;
}

/**
 * Places a stone in the given column.
 *
 * \param columnNr The column to place the stone in.
 * \param player The player that wants to place the stone. Starts at 1 to ease with human inputs.
 * \return Returns true if the operation was successful. False means, that the column is full and no stone can be
 * placed there or the column is not valid.
 */
bool Field::placeStone(int columnNr, Field::Player player)
{
    if (columnNr < 1 || columnNr > FIELD_WIDTH)
        return false;

    // Decrease the columnNumber, because the user enters it from 1 to 7, the vector needs 0 to 6.
    columnNr--;

    // Find the first free field in the column starting from the bottom
    std::vector<char> columnToEdit = getColumn(columnNr);
    for (int rowNr = (int)columnToEdit.size() - 1; rowNr >= 0; rowNr--)
    {
        if (columnToEdit.at(rowNr) == FREE_SPACE_SYMBOL)
        {
            if (player == Player::Human)
                m_fieldState[rowNr][columnNr] = HUMAN_SYMBOL;
            else
                m_fieldState[rowNr][columnNr] = ALGOTITHM_SYMBOL;

            // Check if this move was a winning move
            m_lastMoveColumn = columnNr;
            m_lastMoveRow = rowNr;
            checkWin();

            return true;
        }
    }

    return false;
}

/**
 * Gives info about the status of the game.
 * 
 * \return Returns true if there is a winner.
 */
bool Field::isGameOver()
{
    return m_win || isDraw();
}

/**
 * Gives info about the status of the game.
 * 
 * \return Returns true if the game ended in a draw.
 */
bool Field::isDraw()
{
    std::vector<char> topRow = getRow(0);
    return !m_win && !(std::find(topRow.begin(), topRow.end(), FREE_SPACE_SYMBOL) != topRow.end());
}

/**
 * Indicates if the specified move is possible.
 * 
 * \param columnNr The number of the column to check. Starts at 1 to ease human inputs.
 * \return Returns true if the move is possible.
 */
bool Field::isMovePossible(int columnNr)
{
    std::vector<char> columnToCheck = getColumn(columnNr - 1);

    // If the column is empty, the columnNr might be faulty. This would make a move impossible.
    if (columnToCheck.empty())
        return false;

    return columnToCheck[0] == FREE_SPACE_SYMBOL;
}

/**
 * Gives info about the winner of the game.
 * 
 * \return Returns the winner of the game.
 */
Field::Player Field::getWinner()
{
    return m_winner;
}

/**
 * Gives Info about the height of the field.
 * 
 * \return Returns the height of the field.
 */
int Field::height()
{
    return FIELD_HEIGHT;
}

/**
 * Gives Info about the width of the field.
 * 
 * \return Returns the width of the field.
 */
int Field::width()
{
    return FIELD_WIDTH;
}

/**
 * Checks the field for a win.
 * 
 */
void Field::checkWin()
{
    // Assuming this function runs every time a move is made, the winning combination must contain the last placed
    // stone.
    char referenceSymbol = m_fieldState[m_lastMoveRow][m_lastMoveColumn];
    int maxStepsLeft = std::min(m_lastMoveColumn, WIN_NR - 1);
    int maxStepsRight = std::min(width() - m_lastMoveColumn, WIN_NR) - 1;
    int maxStepsUp = std::min(m_lastMoveRow, WIN_NR - 1);
    int maxStepsDown = std::min(height() - m_lastMoveRow, WIN_NR) - 1;

    // Check horizontal win
    int groupSize = 1;
    for (int offsetLeft = 1; offsetLeft <= maxStepsLeft; offsetLeft++)
    {
        if (m_fieldState[m_lastMoveRow][m_lastMoveColumn -offsetLeft] == referenceSymbol)
            groupSize++;
        else
            break;
    }

    for (int offsetRight = 1; offsetRight <= maxStepsRight; offsetRight++)
    {
        if (m_fieldState[m_lastMoveRow][m_lastMoveColumn + offsetRight] == referenceSymbol)
            groupSize++;
        else
            break;
    }

    if (groupSize >= WIN_NR)
    {
        m_winner = (referenceSymbol == HUMAN_SYMBOL ? Player::Human : Player::Algorithm);
        m_win = true;
        return;
    }

    // Check vertical win
    groupSize = 1;
    for (int offsetTop = 1; offsetTop <= maxStepsUp; offsetTop++)
    {

        if (m_fieldState[m_lastMoveRow - offsetTop][m_lastMoveColumn] == referenceSymbol)
            groupSize++;
        else
            break;
    }

    for (int offsetDown = 1; offsetDown <= maxStepsDown; offsetDown++)
    {
        if (m_fieldState[m_lastMoveRow + offsetDown][m_lastMoveColumn] == referenceSymbol)
            groupSize++;
        else
            break;
    }

    if (groupSize >= WIN_NR)
    {
        m_winner = (referenceSymbol == HUMAN_SYMBOL ? Player::Human : Player::Algorithm);
        m_win = true;
        return;
    }

    // Check diagonal top left to bottom right
    groupSize = 1;
    for (int offset = 1; offset <= __min(maxStepsUp, maxStepsLeft); offset++)
    {
        if (m_fieldState[m_lastMoveRow - offset][m_lastMoveColumn - offset] == referenceSymbol)
            groupSize++;
        else
            break;
    }

    for (int offset = 1; offset <= __min(maxStepsDown, maxStepsRight); offset++)
    {
        if (m_fieldState[m_lastMoveRow + offset][m_lastMoveColumn + offset] == referenceSymbol)
            groupSize++;
        else
            break;
    }

    if (groupSize >= WIN_NR)
    {
        m_winner = (referenceSymbol == HUMAN_SYMBOL ? Player::Human : Player::Algorithm);
        m_win = true;
        return;
    }

    // Check diagonal top right to bottom left
    groupSize = 1;

    for (int offset = 1; offset <= __min(maxStepsUp, maxStepsRight); offset++)
    {
        if (m_fieldState[m_lastMoveRow - offset][m_lastMoveColumn + offset] == referenceSymbol)
            groupSize++;
        else
            break;
    }

    for (int offset = 1; offset <= __min(maxStepsDown, maxStepsLeft); offset++)
    {
        if (m_fieldState[m_lastMoveRow + offset][m_lastMoveColumn - offset] == referenceSymbol)
            groupSize++;
        else
            break;
    }

    if (groupSize >= WIN_NR)
    {
        m_winner = (referenceSymbol == HUMAN_SYMBOL ? Player::Human : Player::Algorithm);
        m_win = true;
        return;
    }
}
