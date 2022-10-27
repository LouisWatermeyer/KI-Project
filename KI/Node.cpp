#include <cmath>
#include <string>
#include <algorithm>

#include "Node.h"

/**
 * Public constructor.
 *
 */
Node::Node()
{
}

/**
 * Initializes the node. This method has to be called after the node was created!
 *
 * \param field The field the node will use.
 * \param turn The turn the node will make.
 * \param moveToMake The number of the column the node will make its move in. If it is <0 the node will not make a
 * move.
 */
void Node::init(Field field, Field::Player turn, int moveToMake)
{
    m_field = field;
    m_turn = turn;
    m_moveMade = moveToMake;

    if (moveToMake > 0)
        m_field.placeStone(moveToMake, turn);
}

/**
 * Evaluates the state of the node. This value is accessible using Node::getNodeValue
 * 
 */
void Node::evaluateState()
{
    if (m_field.isDraw())
    {
        m_nodeValue = 0;
        return;
    }
    else if (m_field.isGameOver())
    {
        if (m_field.getWinner() == Field::Player::Algorithm)
            m_nodeValue = INT_MAX;
        else
            m_nodeValue = INT_MIN;

        return;
    }

    int score = 0;

    // Score center column separate, because it is the most valuable column
    std::vector<char> centerColumn = m_field.getColumn((int)std::ceil(m_field.width() / 2));
    int centerPieces = (int)std::count(centerColumn.begin(), centerColumn.end(), ALGOTITHM_SYMBOL);
    score += centerPieces * 3;
    int centerPiecesEnemy = (int)std::count(centerColumn.begin(), centerColumn.end(), HUMAN_SYMBOL);
    score -= centerPiecesEnemy * 3;

    // Score horizontally
    for (int rowNr = 0; rowNr < m_field.height(); rowNr++)
    {
        std::vector<char> rowVector = m_field.getRow(rowNr);
        for (int columnNr = 0; columnNr <= m_field.width() - WIN_NR; columnNr++)
        {
            score += evaluateSubset(rowVector.begin() + columnNr, rowVector.begin() + columnNr + WIN_NR - 1);
        }
    }

    // Score vertically
    for (int columnNr = 0; columnNr < m_field.width(); columnNr++)
    {
        std::vector<char> columnVector = m_field.getColumn(columnNr);
        for (int rowNr = 0; rowNr <= m_field.height() - WIN_NR; rowNr++)
        {
            score += evaluateSubset(columnVector.begin() + rowNr, columnVector.begin() + rowNr + WIN_NR - 1);
        }
    }

    // Score diagonal top left to bottom right
    int rows = m_field.height(), cols = m_field.width();
    std::string test;
    for (int diagonalNr = rows - 1; diagonalNr >= -(cols - 1); diagonalNr--)
    {
        std::vector<char> diagonal;
        for (int i = std::max(diagonalNr, 0); i < std::min(rows, diagonalNr + cols); i++)
        {
            diagonal.push_back(m_field.getRow(i)[static_cast<std::vector<char, std::allocator<char>>::size_type>(i) - diagonalNr]);
        }

        if (diagonal.size() < WIN_NR)
            continue;

        for (int diagonalPos = 0; diagonalPos < diagonal.size() - WIN_NR; diagonalPos++)
        {
            score += evaluateSubset(diagonal.begin() + diagonalPos, diagonal.begin() + diagonalPos + WIN_NR - 1);
        }
    }

    // Score diagonal bottom left to top right
    for (int diagonalNr = cols - 1; diagonalNr >= -(rows - 1); diagonalNr--)
    {
        std::vector<char> diagonal;
        for (int i = std::max(diagonalNr, 0); i < std::min(cols, diagonalNr + rows); i++)
        {
            diagonal.push_back(m_field.getRow(i - diagonalNr)[i]);
        }

        if (diagonal.size() < WIN_NR)
            continue;

        for (int diagonalPos = 0; diagonalPos < diagonal.size() - WIN_NR; diagonalPos++)
        {
            score += evaluateSubset(diagonal.begin() + diagonalPos, diagonal.begin() + diagonalPos + WIN_NR - 1);
        }
    }

    m_nodeValue = score;
}

/**
 * Helper method to evaluate a subset of characters.
 * 
 * \param begin Start iterator of the subset.
 * \param end End iterator of the subset
 * \return Returns the score the subset provides to the field.
 */
int Node::evaluateSubset(std::vector<char>::iterator begin, std::vector<char>::iterator end)
{
    // Score by checking all subsets of the row that can cause a win.
    // 1.)
    //  |X|X|X|O|O|X|O|
    //   I---->I
    // 2.)
    //  |X|X|X|O|O|X|O|
    //     I---->I
    // 3.) ...

    int returnValue = 0;
    int numberOfPieces = 0;
    int numberOfSpaces = 0;
    int numberOfEnemyPieces = 0;
    std::for_each(begin, end, [&numberOfPieces, &numberOfSpaces, &numberOfEnemyPieces](char& value) {
        if (value == FREE_SPACE_SYMBOL)
            numberOfSpaces++;
        else if (value == ALGOTITHM_SYMBOL)
            numberOfPieces++;
        else
            numberOfEnemyPieces++;
        });

    // Award points for the distribution of pieces in the subset.
    returnValue += numberOfPieces * 2;
    returnValue -= numberOfEnemyPieces * 2;

    // Also award points for empty cells in the right positions.
    // E.g. this subset: 
    //  |O|O|O| |
    // Is more valuable that this one:
    //  |O|O|O|X|
    if (numberOfPieces == 3 && numberOfSpaces == 1)
        returnValue += 1000;
    else if (numberOfEnemyPieces == 3 && numberOfSpaces == 1)
        returnValue -= 1000;

    return returnValue;
}

/**
 * Sets the value of the node. This way the node can get a value without being evaluated.
 * 
 * \param value The new value.
 */
void Node::setNodeValue(int value)
{
    m_nodeValue = value;
}

/**
 * Getter for the move the node made.
 * 
 * \return Move the node made. Can be -1 meaning, the node made no move.
 */
int Node::getMoveMade()
{
    return m_moveMade;
}

/**
 * Getter for the value of the node.
 * 
 * \return Returns the value of the node.
 */
int Node::getNodeValue()
{
    return m_nodeValue;
}

/**
 * Creates the next possible moves recursively according to its field. These Moves will be represented by new nodes
 * which will be the children of this node.
 * 
 * \param depth Is the maximum depth of the recursion.
 */
void Node::createNextMoves(int depth)
{
    // Don't create a deeper level if we bottom out here.
    if (depth == 0 || m_field.isGameOver())
        return;

    // If this node already has children, just pass the instruction along.
    // Otherwise create children.
    if (m_children.empty())
    {
        for (int nextMoveColumn = 0; nextMoveColumn <= m_field.width(); nextMoveColumn++)
        {
            Field::Player nextPlayer = m_turn == Field::Player::Human ? Field::Player::Algorithm
                : Field::Player::Human;
            if (m_field.isMovePossible(nextMoveColumn))
            {
                std::shared_ptr<Node> newChild = std::make_shared<Node>();
                newChild->init(m_field, nextPlayer, nextMoveColumn);
                m_children.push_back(newChild);
            }
        }
    }

    for (std::shared_ptr<Node> children : m_children)
    {
        children->createNextMoves(depth - 1);
    }
}

/**
 * Indicates if the game state the node represents is over.
 * 
 * \return 
 */
bool Node::isGameOver()
{
    return m_field.isGameOver();
}

/**
 * Getter for the children of the node.
 * 
 * \return 
 */
std::vector<std::shared_ptr<Node>> Node::getChildren()
{
    return m_children;
}
