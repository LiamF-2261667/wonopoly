
#ifndef GAME_H
#define GAME_H

#include "Board/Board.h"
#include "Auction.h"
#include "Dice.h"
#include "GameLoader.h"
#include "Game/Purchase.h"

#include <QObject>

class Player;

class Game : public QObject
{
    Q_OBJECT

public:
    static const int MAX_PLAYER_COUNT{ 6 };

    /* Constructors */
    explicit Game(int playerCount = 0) : QObject(), m_maxPlayerCount{ playerCount } { }
    ~Game();

    /* Methods */
    void startGame(const QString& loadFile = QString()); // Author: Liam
    void resumeGame(); // Author: Jesse
    void save(const QString &file); // Author: Liam
    void stop(); // Author: Liam
    void applyRoll(); // Author: Jesse
    void managePlayers(); // Author: Jesse

signals:
    void gameStarted(Board* board, Dice* dice, GameLoader* gameloader, Purchase* purchase, Auction* auction); // Author: Liam
    void waitingForRoll(); // Author: Liam
    void changedCurrentPlayer(Player* player); // Author: Liam
    void playerIsBankrupt(Player* player); // Author: Jesse
    void removedPlayerFromGame(Player* player); // Author: Jesse
    void inJailTurn(Player* player); // Author: Jesse
    void playerWon(Player* winner); // Author: Liam

private:
    const int m_maxPlayerCount{};
    QVector<Player*> m_players{};
    int m_currentPlayerIndex{};
    Player* m_currentPlayer{};

    Auction m_auction{ m_players };
    Purchase m_purchase{ &m_auction };
    Board m_board{};
    Dice m_dice{ 2 };
    GameLoader m_gameLoader{ &m_board, &m_purchase, m_players };
    
    QVector<BuyableCard*> m_cards{};

    int m_backToBackEquals{};
    bool m_doingAction{ false };

    bool m_hasWinner{ false };

    void nextPlayer(); // Author: Jesse
    void manageGameStatus(); // Author: Jesse
    void moveCurrentPlayer(); // Author: Liam
    void executeCurrentPlayerMove(); // Author: Liam + Jesse

    void removePlayer(Player* player); // Author: Jesse
    void distributePlayerCards(Player* player); // Author: Jesse
};

#endif // GAME_H
