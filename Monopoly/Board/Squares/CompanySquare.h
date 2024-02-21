/*
 * Author: Liam
*/

#ifndef COMPANYSQUARE_H
#define COMPANYSQUARE_H


#include "BuyableSquare.h"


class CompanySquare : public BuyableSquare
{
    Q_OBJECT

public:
    /* Constructors */
    CompanySquare(BuyableCard* card, Purchase* purchase, const int pos = 0) : BuyableSquare(card, purchase, pos) { }

    /* Getters */
    TYPE getType() const override { return TYPE::COMPANY; }

    /* Methods */
    void doEffect(Player* player) override;
    void executeEffectWithDice(const int diceSum, Player* player);
};

#endif // COMPANYSQUARE_H
