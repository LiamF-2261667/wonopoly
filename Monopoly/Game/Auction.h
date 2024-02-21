/*
 * Author: Liam
*/

#ifndef AUCTION_H
#define AUCTION_H

#include <QObject>
#include <QVector>

#include "Purchase.h"

class Player;


class Auction : public QObject
{
    Q_OBJECT

public:
    /* Constructors */
    Auction(QVector<Player*>& players) : m_players{ players } { }

    /* Getters */
    int getHighestBid() const { return m_highestBid.second; }
    Player* getCurrentBidder() const { return m_players[m_currentBidderIndex]; }
    BuyableCard* getCard() const { return m_card; }

    /* Methods */
    void startAuction(BuyableCard* card);
    void bid(const int amount);
    void nextBidder();
    
    void startMassAuction(QVector<BuyableCard*>& cards); // Author: Jesse

signals:
    void startingAuction(Auction* auction);
    void endingAuction();
    void nextBid();

private:
    const Money M_STARTING_PRICE{ 10 };

    QVector<Player*>& m_players;
    BuyableCard* m_card{};

    int m_currentBidderIndex{};
    QPair<Player*, Money> m_highestBid{ QPair<Player*, Money>(nullptr, M_STARTING_PRICE - 1) };

    // Author: Jesse
    bool m_hasMultipleItems{ false };
    int m_currentItemIndex{ 0 };
    QVector<BuyableCard*> m_cards{};

    void buyForBestBid();
    void endAuction();
};

#endif // AUCTION_H
