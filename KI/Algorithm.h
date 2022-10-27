#ifndef ALGORYTHM_H
#define ALGORYTHM_H

#include "Field.h"
#include "Node.h"

// TREE_DEPTH starts at 0, meaning that the root node will have a depth of 0
//      O       depth = 0
//     / \
//    O   O     depth = 1
// This would be a tree with the depth of 1.
constexpr auto TREE_DEPTH = 7;

class Algorithm
{
private:
    /* Private constructor to prevent instancing. */
    Algorithm();

    int minimax(std::shared_ptr<Node> node, int depth, int alpha, int beta, Field::Player nextPlayer);

    std::shared_ptr<Node> m_topLevelNode;

public:
    /* Static access method. */
    static Algorithm* getInstance();

    int getNextMove(Field field);
};

#endif
