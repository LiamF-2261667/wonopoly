
#include "Game.h"

#include "Cards/BuyableCard.h"
#include "Player/Player.h"

Game::~Game() {
    for (int i = 0; i < m_players.size(); ++i) {
        delete m_players[i];
    }
    for (int i = 0; i < m_cards.size(); ++i) {
        delete m_cards[i];
    }
}

/* Author: Liam */
/* Start the interactable game */
void Game::startGame(const QString& loadFile) {
    emit gameStarted(&m_board, &m_dice, &m_gameLoader, &m_purchase, &m_auction);

    // Loading the game
    if (loadFile != QString()) {
        try {
            m_gameLoader.loadGame(loadFile);
        } catch(...) {
            m_gameLoader.newGame(m_maxPlayerCount);
        }
    }
    else
        m_gameLoader.newGame(m_maxPlayerCount);

    // Loading the needed variables for a game
    m_cards = m_gameLoader.getCards();
    m_currentPlayerIndex = m_gameLoader.getCurrentPlayerIndex();
    m_backToBackEquals = m_gameLoader.getBackToBackEquals();

    // Setting the first player
    m_currentPlayer = m_players[m_currentPlayerIndex];
    emit changedCurrentPlayer(m_currentPlayer);
}

/* Author: Jesse */
/* Resumes to the next turn after various game effects */
void Game::resumeGame() {
    manageGameStatus();

    if (!(m_currentPlayer->getMoney() < 0)) {
        m_doingAction = false;
        m_currentPlayer->inEffect(false);
        nextPlayer();
    }
}

/* Author: Liam */
/* Stops the game and calculates a winner */
void Game::stop() {
    // By default, the winner is the first player
    Player* winner{ m_players.first() };

    // Go over every player to decide if it should win
    for (Player* player : m_players) {
        // Add the total worth of selling everything
        player->gain(player->getNetWorth());

        // Remove all the cards
        for (int i = 0; i < player->cardsOwned(); ++i)
            player->removeCard(player->getCard(i));

        // Remove all the getOutOfJailCards
        player->removeJailCard(player->jailCardsOwned());

        // Check if the player has more money than the winner
        if (player->getMoney() > winner->getMoney())
            winner = player;
    }

    m_hasWinner = true;
    emit playerWon(winner);
}

/* Author: Liam */
/* Save the current gamestate in the given file */
void Game::save(const QString &file) {
    m_gameLoader.saveGame(m_currentPlayerIndex, m_backToBackEquals, file);
}

/* Author: Jesse */
/* Apply the roll to the current game state */
void Game::applyRoll() {

    // Send signals to update balance changes
    for (Player* p : m_players)
        p->pay(0);

    // If in square effect or in jail
    if (m_doingAction || m_currentPlayer->isInJail()) {
        m_board.doRollEffect(m_currentPlayer, m_dice.getDiceRoll(), m_dice.isEqual());
        m_doingAction = false;
    }

    // Standard case: move the player
    else {
        moveCurrentPlayer();
    }

    // Continue to the next player unless they're in a special effect
    if (!m_currentPlayer->isInEffect()) {
        if (!(m_currentPlayer->getMoney() < 0))
            resumeGame();
        else
            manageGameStatus();
    }
}

/* Author: Liam */
/* Roll the dice to move the current player */
void Game::moveCurrentPlayer() {
    if (!m_dice.isEqual())
        m_backToBackEquals = 0;

    else {
        m_backToBackEquals++;

        if (m_backToBackEquals > 2) {
            m_backToBackEquals = 0;
            m_board.sendPlayerToJail(m_currentPlayer);
            return;
        }
    }

    executeCurrentPlayerMove();
}

/* Author: Jesse */
/* Move a player according to the thrown dice */
void Game::executeCurrentPlayerMove() {
    // Execute any passing effect while moving
    m_board.doPassingEffect(m_currentPlayer, m_currentPlayer->getPosition(), m_dice.getDiceRoll());

    m_currentPlayer->goForward(m_dice.getDiceRoll());
    int pos = m_currentPlayer->getPosition();

    m_doingAction = true;
    m_board.doEffect(m_currentPlayer);

    // Execute any passing effect again (player could move during the board effect)
    m_board.doPassingEffect(m_currentPlayer, pos, m_currentPlayer->getPosition() - pos);
}

/* Author: Jesse */
/* Go to the next turn (aka the next player) */
void Game::nextPlayer() {
    if (m_backToBackEquals == 0) {
        m_currentPlayerIndex = (m_currentPlayerIndex + 1) % m_players.size();
        m_currentPlayer = m_players[m_currentPlayerIndex];

        emit changedCurrentPlayer(m_currentPlayer);

        /* Open jail prompt if next is in jail */
        if (m_currentPlayer->isInJail()) {
            emit inJailTurn(m_currentPlayer);
            return;
        }
    }

    emit waitingForRoll();
}

/* Author: Jesse */
/* Remove a player and allow resuming of the game */
void Game::removePlayer(Player* player) {
    m_players.removeAt(m_players.indexOf(player));

    // If the player to be removed is the current player, reset the backToBackEquals
    if (m_currentPlayer == player) {
        m_backToBackEquals = 0;
        m_doingAction = false;

        // Manage the current user ID
        m_currentPlayerIndex--;
    }
    else
        m_currentPlayerIndex = m_players.indexOf(m_currentPlayer);

    emit removedPlayerFromGame(player);

    // Make sure the current player doesn't get corrupted after deletion
    if (m_currentPlayerIndex != -1)
        m_currentPlayer = m_players[m_currentPlayerIndex];
    else
        nextPlayer();

    // Distribute the items the player will leave behind upon losing
    if (player->cardsOwned() != 0 && m_players.size() != 1)
        distributePlayerCards(player);
    else { // If there are no cards to be distributed, go next
        nextPlayer();
        manageGameStatus();
    }

    // Delete the player
    delete player;
}

/* Author: Jesse */
/* All checks that need to happen after each action */
void Game::manageGameStatus() {
    if (m_hasWinner) return;

    /* Check if someone has to be sent to jail */
    if (m_currentPlayer->isInJail()) {
        m_board.sendPlayerToJail(m_currentPlayer);
        m_backToBackEquals = 0;
    }

    /* Check if player is truly bankrupt */
    if (m_currentPlayer->getMoney() < 0) {
        if (m_currentPlayer->getNetWorth() + m_currentPlayer->getMoney() < 0)
            managePlayers();
        else
            emit playerIsBankrupt(m_currentPlayer);
    }

    /* Check if someone won the game */
    if (m_players.size() == 1) {
        m_hasWinner = true;
        emit playerWon(m_players.first());
    }
}

/* Author: Jesse */
/* Manage the player list */
void Game::managePlayers() {
    int i{0};
    while (i < m_players.size()) {
        if (m_players[i]->getMoney() < 0)
            removePlayer(m_players[i]);
        else
            ++i;
    }
}

/* Author: Jesse */
/* Distributes the items of a player to make sure they don't go lost */
void Game::distributePlayerCards(Player* player) {
    QVector<BuyableCard*> leftoverCards{};
    for (int i = 0; i < player->cardsOwned(); ++i) {
        leftoverCards.push_back(player->getCard(i));
    }
    for (int i = 0; i < leftoverCards.size(); ++i) {
        leftoverCards[i]->changeOwner(nullptr);
    }

    /* If player went bankrupt to the bank, start an auction on every card they own */
    if (player->getIndebtedPlayer() == nullptr) {
        m_auction.startMassAuction(leftoverCards);
    }

    /* Else give all cards to the player they went bankrupt to */
    else {
        for (int i = 0; i < leftoverCards.size(); ++i)
            player->getIndebtedPlayer()->addCard(leftoverCards[i]);

        nextPlayer();
    }
}
