/*
 * Author: Jesse
*/


#ifndef ACTIONCARD_H
#define ACTIONCARD_H

#include "Player/Money.h"
#include "Player/Player.h"

class ActionCard : public QObject
{
    Q_OBJECT
public:
    static const Money JAIL_CARD_SELL_VALUE{ 50 };

    enum class ACTION {
        MOVE,
        PAY,
        PAY_REPAIRS,
        RECEIVE,
        RECEIVE_FROM_OTHERS,
        JAIL,
        CARD,
        PAY_OTHERS
    };

    /* Constructors */
    ActionCard(const QVector<Player*>& players, const ACTION type, const QString& title,
               const Money amount = 0, const QPair<Money, Money>& amounts = {0, 0}, const int moveAmount = 0, const int position = 0) :
        m_players{ players }, m_type{ type }, m_title{ title },
        m_amount{ amount }, m_amounts{ amounts }, m_moveAmount{ moveAmount }, m_position{ position } { }

    void doEffect(Player* player);

signals:
    void doneEffect(const QString& title);

private:
    const QVector<Player*>& m_players;
    const ACTION m_type{};
    const QString m_title{};

    const Money m_amount{};
    const QPair<Money, Money> m_amounts{};
    const int m_moveAmount{};
    const int m_position{};

    void doMovement(Player* player);
    void doPayment(Player* player);
    void doPaymentToOthers(Player* player);
    void doRepairs(Player* player);
    void doGain(Player* player);
    void doGainFromOthers(Player* player);
    void goJail(Player* player);
    void giveCard(Player* player);
};

#endif // ACTIONCARD_H
