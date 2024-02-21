/*
 * Author: Liam
*/

#ifndef PURCHASE_H
#define PURCHASE_H

#include "Player/Money.h"
#include <QObject>



class BuyableCard;
class Player;
class Auction;


class Purchase : public QObject
{
    Q_OBJECT

public:
    /* Constructor */
    Purchase(Auction* auction) : m_auction{ auction } { }

    /* Getters */
    BuyableCard* getCard() const { return m_card; }

    /* Methods */
    bool canBuy(Player* player = nullptr) const;
    void start(BuyableCard* card, Player* buyer) { m_card = card; m_currentBuyer = buyer; emit startingPurchase(this, m_card, m_currentBuyer); }
    void buy(Player* player = nullptr);
    void buy(Money price, Player* player = nullptr);
    void auction();
    void end() { emit endingPurchase(); }

signals:
    void startingPurchase(Purchase* purchase, BuyableCard* card, Player* player);
    void auctioningPurchase();
    void endingPurchase();

private:
    BuyableCard* m_card{};
    Auction* m_auction{};
    Player* m_currentBuyer{};
};

#endif // PURCHASE_H
