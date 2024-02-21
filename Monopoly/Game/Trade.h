/*
 * Author: Jesse
*/

#ifndef TRADE_H
#define TRADE_H

#include <QObject>
#include "Player/Player.h"

class Trade : public QObject
{
    Q_OBJECT
public:
    explicit Trade(QObject *parent = nullptr) { }

    void start(QPair<Player*, Player*>& traders,
               QPair<QVector<BuyableCard*>, QVector<BuyableCard*>>& cards,
               QPair<Money, Money>& money,
               QPair<int, int>& jailCards);

signals:
    void tradeFailed(const QString& reason) const;
    void tradeSuccessful() const;

private:
    QPair<Player*, Player*> m_players{};
    
    bool checkCardViability(const QVector<BuyableCard*>& cards) const;

    void exchangeMoney(QPair<Money, Money>& money);
    void exchangeCards(QPair<QVector<BuyableCard*>, QVector<BuyableCard*>>& cards, QPair<int, int>& jailCards);

};

#endif // TRADE_H
