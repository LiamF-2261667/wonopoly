/*
 * Author: Liam
*/

#ifndef BUYABLECARD_H
#define BUYABLECARD_H

#include <array>

#include "Player/Money.h"
#include <QObject>
class Player;


class BuyableCard : public QObject
{
    Q_OBJECT

public:
    enum class TYPE {
        STREET,
        STATION,
        COMPANY,
    };

    enum class ACTION {
        SELL,
        BUILD,
        DOWNGRADE,
        TRADE,
        MORTGAGE,
        UNMORTGAGE
    };

    /* Constructors */
    BuyableCard(const int id, const Money price, const Money mortgagePrice, const bool isMortgaged)
        : m_price{ price }, M_ID{ id }, m_mortgageCost{ mortgagePrice }, m_isMortgaged{ isMortgaged } { }

    /* Getters */
    int getId() const { return M_ID; }
    Player* getOwner() const { return m_owner; }
    Money getMortgagePrice() const { return m_mortgageCost; }
    bool isMortgaged() const { return m_isMortgaged; }

    virtual Money getPrice() const { return m_price; }
    virtual Money getSellValue() const { return m_price/2; }
    virtual Money getTotalValue() const { return getSellValue(); }

    virtual Money getRent() const = 0;
    virtual TYPE getType() const = 0;
    virtual QVector<ACTION> getActions(Player* user) const = 0; // Author: Jesse

    /* Methods */
    void changeOwner(Player* newOwner);

    virtual void executeAction(const ACTION action) = 0; // Author: Jesse

signals:
    void cardActionPerformed();

protected:
    Player* m_owner{};

    const Money m_price{};

    void mortgage(bool set = true);
    void sell();

private:
    const int M_ID;

    const Money m_mortgageCost{};

    bool m_isMortgaged{ false };
};

#endif // BUYABLECARD_H
