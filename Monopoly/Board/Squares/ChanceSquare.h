/*
 * Author: Jesse
*/

#ifndef CHANCESQUARE_H
#define CHANCESQUARE_H

#include "Square.h"
#include "Cards/ActionCard.h"

class ChanceSquare : public Square
{
public:
    /* Constructors */
    ChanceSquare(QVector<ActionCard*>& pool, const int pos = 0);

    /* Getters */
    TYPE getType() const override { return TYPE::CHANCE; }

    /* Methods */
    void doEffect(Player* player) override;

private:
    QVector<ActionCard*>& m_pool;
};

#endif // CHANCESQUARE_H
