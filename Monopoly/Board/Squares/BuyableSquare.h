/*
 * Author: Liam
*/


#ifndef BUYABLESQUARE_H
#define BUYABLESQUARE_H


#include "Square.h"

#include <Game/Purchase.h>

class BuyableCard;


class BuyableSquare : public Square
{
public:
    /* Constructors */
    BuyableSquare(BuyableCard* card, Purchase* purchase, const int pos = 0) : Square(pos, card), m_purchase{ purchase } { }

    /* Methods */
    void startPurchase(Player* buyer) { m_purchase->start(getCard(), buyer); }

private:
    Purchase* m_purchase{};
};

#endif // BUYABLESQUARE_H
