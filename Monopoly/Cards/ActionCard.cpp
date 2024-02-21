/*
 * Author: Jesse
*/

#include "ActionCard.h"

/* Do an effect based on the type of the card */
void ActionCard::doEffect(Player* player) {
    if (m_type == ACTION::MOVE)                 doMovement(player);
    if (m_type == ACTION::PAY)                  doPayment(player);
    if (m_type == ACTION::PAY_REPAIRS)          doRepairs(player);
    if (m_type == ACTION::RECEIVE)              doGain(player);
    if (m_type == ACTION::RECEIVE_FROM_OTHERS)  doGainFromOthers(player);
    if (m_type == ACTION::JAIL)                 goJail(player);
    if (m_type == ACTION::CARD)                 giveCard(player);
    if (m_type == ACTION::PAY_OTHERS)           doPaymentToOthers(player);

    emit doneEffect(m_title);
}

/* Move the player a given amount or to a specific position */
void ActionCard::doMovement(Player* player) {
    if (m_moveAmount != 0) {
        player->goForward(m_moveAmount);
        return;
    }

    player->moveTo(m_position);
}

/* Make the player pay a specific amount */
void ActionCard::doPayment(Player* player) {
    player->pay(m_amount);
}

/* Make the player pay all other players a specific amount */
void ActionCard::doPaymentToOthers(Player* player) {
    for (int i = 0; i < m_players.size(); ++i) {
        if (m_players[i] != player)
            m_players[i]->gain(m_amount);
    }

    player->pay(m_amount * (m_players.size() - 1));
}

/* Make the player pay a specific amount based on their build count */
void ActionCard::doRepairs(Player* player) {
    player->pay(m_amounts.first * player->buildingsOwned().first);
    player->pay(m_amounts.second * player->buildingsOwned().second);
}

/* Give the player a certain amount of money */
void ActionCard::doGain(Player* player) {
    player->gain(m_amount);
}

/* Make other players in the game pay the player */
void ActionCard::doGainFromOthers(Player* player) {
    for (int i = 0; i < m_players.size(); ++i) {
        if (m_players[i] != player)
            m_players[i]->pay(m_amount, player);
    }

    player->pay(m_amount * (m_players.size() - 1));
    player->gain(m_amount * (m_players.size() - 1));
}

/* Make the player go to jail */
void ActionCard::goJail(Player* player) {
    player->goToJail();
}

/* Give the player a get out of jail free card */
void ActionCard::giveCard(Player* player) {
    player->addJailCard();
}
