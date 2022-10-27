#include "Algorithm.h"
#include <omp.h>

/**
 * Private constructor to prevent instancing. This class is designed using the singleton pattern.
 *
 */
Algorithm::Algorithm()
{
    m_topLevelNode = std::make_unique<Node>();
}

/**
 * Static getter for the instance of this singleton class.
 *
 * \return Instance of the singleton class.
 */
Algorithm* Algorithm::getInstance()
{
    static Algorithm instance;
    return &instance;
}

/**
 * Calculates the next move the algorithm wants to make.
 *
 * \param field The field that is used as the top node of the tree. The algorithm will calculate its next move on the
 * basis of that field.
 * \return The number of the column in which the algorithm wants make its next move. This starts at 1 because it mimics
 *  a human player.
 */
int Algorithm::getNextMove(Field field)
{
    // Create the tree
    m_topLevelNode.reset(new Node());
    m_topLevelNode->init(field, Field::Player::Human);
    m_topLevelNode->createNextMoves(TREE_DEPTH);

    // Evaluate tree
    minimax(m_topLevelNode, TREE_DEPTH, INT_MIN, INT_MAX, Field::Player::Algorithm);

    // Get the next move by checking which direct child has the best outcome
    int bestOutcome = INT_MIN;
    int moveToMake = -1;
    for (std::shared_ptr<Node> directChild : m_topLevelNode->getChildren())
    {
        if (directChild->getNodeValue() > bestOutcome)
        {
            bestOutcome = directChild->getNodeValue();
            moveToMake = directChild->getMoveMade();
        }
    }

    return moveToMake;
}

/**
 * Minimax function that works recursively.
 *
 * \param node The node that gets evaluated.
 * \param depth The maximum search depth.
 * \param alpha Alpha value for Alpha-Beta pruning.
 * \param beta Beta value for Alpha-Beta pruning.
 * \param nextPlayer The player  that makes the next move in reference to the given node.
 * \return
 */
int Algorithm::minimax(std::shared_ptr<Node> node, int depth, int alpha, int beta, Field::Player nextPlayer)
{
    // return the evaluation of a node if we have reached the maximum search depth.
    if (depth <= 0 || node->isGameOver())
    {
        node->evaluateState();
        return node->getNodeValue();
    }

    if (nextPlayer == Field::Player::Algorithm)
    {
        // Pick the best outcome
        int max = INT_MIN;

        #pragma omp parallel for
        for (int index = 0; index < node->getChildren().size(); index++)
        {
            max = std::max(max, minimax(node->getChildren()[index], depth - 1, alpha, beta, Field::Player::Human));
            alpha = std::max(alpha, max);

            // We don't need to check the rest of the children, if the human already has a better choice by taking
            // another branch.
            if (beta <= alpha)
                break;
        }
        node->setNodeValue(max);
        return max;
    }
    else
    {
        // Pick the worst outcome
        int min = INT_MAX;

        #pragma omp parallel for
        for (int index = 0; index < node->getChildren().size(); index++)
        {
            min = std::min(min, minimax(node->getChildren()[index], depth - 1, alpha, beta, Field::Player::Algorithm));
            beta = std::min(beta, min);

            // We don't need to check the rest of the children, if the algorithm already has a better choice by taking
            // another branch.
            if (beta <= alpha)
                break;
        }
        node->setNodeValue(min);
        return min;
    }
}
