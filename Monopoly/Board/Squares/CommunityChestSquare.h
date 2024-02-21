/*
 * Author: Jesse
*/



#ifndef COMMUNITYCHESTSQUARE_H
#define COMMUNITYCHESTSQUARE_H

#include "Square.h"
#include "Cards/ActionCard.h"

class CommunityChestSquare : public Square
{
public:
    /* Constructors */
    CommunityChestSquare(QVector<ActionCard*>& pool, const int pos = 0);

    /* Getters */
    TYPE getType() const override { return TYPE::COMMUNITY_CHEST; }

    /* Methods */
    void doEffect(Player* player) override;

private:
    QVector<ActionCard*>& m_pool;
};

#endif // COMMUNITYCHESTSQUARE_H
