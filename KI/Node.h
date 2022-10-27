#ifndef NODE_H
#define NODE_H

#include <vector>
#include <memory>
#include "Field.h"

class Node
{
public:
    Node();

    void init(Field field, Field::Player turn, int moveToMake = -1);
    void evaluateState();
    void setNodeValue(int value);
    int getNodeValue();
    int getMoveMade();
    void createNextMoves(int depth);
    bool isGameOver();
    std::vector<std::shared_ptr<Node>> getChildren();

private:
    int evaluateSubset(std::vector<char>::iterator begin, std::vector<char>::iterator end);

    std::vector<std::shared_ptr<Node>>  m_children;
    Field                               m_field;
    int                                 m_moveMade  = -1;
    int                                 m_nodeValue = 0;
    Field::Player                       m_turn      = Field::Player::Algorithm;
};

#endif