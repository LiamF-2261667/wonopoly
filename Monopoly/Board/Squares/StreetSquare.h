/*
 * Author: Jesse
*/



#ifndef STREETSQUARE_H
#define STREETSQUARE_H

#include "BuyableSquare.h"



class StreetSquare : public BuyableSquare
{
public:
    /* Constructors */
    StreetSquare(BuyableCard* card, Purchase* purchase, const int pos = 0) : BuyableSquare(card, purchase, pos) { }

    /* Getters */
    TYPE getType() const override { return TYPE::STREET; }

    /* Methods */
    void doEffect(Player* player) override;
};

#endif // STREETSQUARE_H
