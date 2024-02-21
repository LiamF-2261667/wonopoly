/*
 * Author: Jesse
*/

#include "Balance.h"

/* Checks whether the balance status is stable or not
   Argument payAmount allows the user to check whether they will be bankrupt after a payment */
Balance::STATUS Balance::checkBalance(Money payAmount) const {
    if (m_balance - payAmount < 0)
        return STATUS::BANKRUPT;
    else
        return STATUS::STABLE;
}

/* Remove a specified amount of money from the balance */
void Balance::removeMoney(Money amount) {
    m_balance -= amount;
}
