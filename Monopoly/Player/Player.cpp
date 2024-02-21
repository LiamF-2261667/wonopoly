
#include "Player.h"
#include "Board/Board.h"
#include "Cards/BuyableCard.h"
#include "Cards/ActionCard.h"

#include <iostream>

/* Author: Liam */
/* Set is player on the board */
void Player::spawn() {
    emit playerSpawned(this);
}

/* Author: Liam */
/* Move the player a specified amount forward on the board */
void Player::goForward(int amount) {
    // Loop over all the tiles (if it exceeds the amount of tiles)
    m_pos = (m_pos + amount) % Board::TILE_AMOUNT;

    // Make sure the position is a postive number
    while (m_pos < 0)
        m_pos += Board::TILE_AMOUNT;

    emit playerMoved(this, m_pos);
}

/* Author: Liam */
/* Move to player to a specific position on the board */
void Player::moveTo(int position) {
    // Check if the new position is legal
    if (position >= Board::TILE_AMOUNT || position < 0) {
        std::cerr << "Trying to move towards an illegal position!\n";
        return;
    }

    m_pos = position;

    emit playerMoved(this, m_pos);
}

/* Author: Jesse */
/* CardList functionality */
void Player::addCard(BuyableCard* card) {
    m_cards += card;
    card->changeOwner(this);
}

/* Author: Jesse */
void Player::removeCard(const BuyableCard *card) {
    m_cards.removeAt(m_cards.indexOf(card));
}

/* Author: Jesse */
/* Returns a pair which displays how many buildings are owned
   The first in the pair being the houses, while the second is the hotels */
QPair<int, int> Player::buildingsOwned() const {
    return {m_houses, m_hotels};
}

/* Author: Jesse */
/* Checks whether a player can afford a certain sum */
bool Player::canAfford(Money amount) {
    if (m_balance.checkBalance(amount) == Balance::STATUS::BANKRUPT)
        return false;
    return true;
}

/* Author: Jesse */
/* Balance functionality */
void Player::pay(Money amount, Player* receiver) {
    if (!canAfford(amount)) {
        if (receiver != nullptr) m_indebtedPlayer = receiver;
    }

    // Pay the money
    gain(-amount);
    emit balanceChanged(this, -amount);

    // If the receiver is not the bank, then pay the receiver the amount
    if (receiver != nullptr) receiver->gain(amount);
}

/* Author: Jesse */
/* Gain a specified amount of money */
void Player::gain(Money amount) {
    bool bankrupt{ false };
    if (getMoney() < 0)
        bankrupt = true;

    m_balance.addMoney(amount);
    emit balanceChanged(this, amount);

    if (getMoney() >= 0 && bankrupt)
        emit recoveredFromBankruptcy(this);
}

/* Author: Jesse */
/* Exit jail */
void Player::exitJail() {
    m_isInJail = false;
    m_jailExitAttempts = 0;
}

/* Author: Jesse */
/* Returns the amount of money one could gain from selling all their assets */
Money Player::getNetWorth() const {
    Money total{ 0 };
    for (BuyableCard* card : m_cards) {
        if (!card->isMortgaged())
            total += card->getTotalValue();
    }

    total += m_getOutOfJailCards * ActionCard::JAIL_CARD_SELL_VALUE;

    return total;
}
