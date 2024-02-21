/*
 * Author: Jesse
*/

#ifndef BALANCE_H
#define BALANCE_H

#include "Money.h"


class Balance
{
public:

    enum class STATUS {
        BANKRUPT,
        STABLE
    };

    /* Constructors */
    Balance(Money startAmount = 0) : m_balance{startAmount} { };

    /* Getters */
    Money getMoney() const { return m_balance; }

    /* Methods */
    STATUS checkBalance(Money payAmount = 0) const;
    void removeMoney(Money amount);
    void addMoney(Money amount) { m_balance += amount; };

private:
    Money m_balance{};
};

#endif // BALANCE_H
