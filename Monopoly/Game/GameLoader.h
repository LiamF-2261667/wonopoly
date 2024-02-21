/*
 * Author: Liam
*/


#ifndef GAMELOADER_H
#define GAMELOADER_H

#include <QJsonObject>
#include <QString>

#include "Board/Squares/Square.h"
#include "Cards/ActionCard.h"

class Board;
class BuyableCard;
class Player;
class Purchase;
class Jail;
class StreetCard;


class GameLoader : public QObject
{
    Q_OBJECT

public:
    /* Constructors */
    GameLoader(Board* board, Purchase* purchase, QVector<Player*>& players);
    ~GameLoader();

    /* Getters */
    const QVector<BuyableCard*>& getCards() const { return m_cards; }
    int getCurrentPlayerIndex() const { return m_currentPlayerIndex; }
    int getBackToBackEquals() const { return m_backToBackEquals; }

    /* Methods */
    void loadGame(const QString& fileLocation, const int playerCount = 6);
    void newGame(const int playerCount) { loadGame(M_DEFAULT_FILE_LOCATION, playerCount); }
    void saveGame(int currentPlayerId, int backToBackTurns, const QString& fileLocation);

signals:
    void finishedLoadingPlayers(QVector<Player*>& players);
    void finishedLoadingActionCards(const QVector<ActionCard*>& communityChest, const QVector<ActionCard*>& chance);

private:
    Board* m_board{};
    Purchase* m_purchase{};
    Jail* m_jail{};
    QVector<BuyableCard*> m_cards{};
    QVector<Player*>& m_players;
    QVector<Square*> m_squares{};

    QVector<ActionCard*> m_communityChestPool{};
    QVector<ActionCard*> m_chancePool{};
    
    QVector<QVector<BuyableCard*>> m_cardSets{};
    int m_currentPlayerIndex{};
    int m_backToBackEquals{};

    QString M_DEFAULT_FILE_LOCATION{"://SaveFiles/defaultGameSetup.json"};

    QJsonObject readJsonData(const QString& fileLocation);
    void writeJsonData(const QString& fileLocation, QJsonObject& jsonOb);

    void loadGameInfo(const QJsonObject& data, int playerCount);
    void loadPlayer(const QJsonObject& playerData);

    void loadAllCards(const QJsonObject& data);
    void loadCard(const QJsonObject& cardData);
    void loadCardSets(const QJsonObject& data);
    void loadActionCards(const QJsonObject& data); // Author: Jesse
    ActionCard* loadActionCard(const QJsonObject& cardData); // Author: Jesse

    void loadAllSquares(const QJsonObject& data);
    void loadSquare(const QJsonObject& squareData);
    
    QVector<QVector<StreetCard*>> getCardSets(const QJsonObject& data);
    QVector<QVector<int>> getCardSetsIds(const QJsonObject& data);

    void loadStreetCard(const QJsonObject& cardData, const int id, const bool isMortgaged);
    void loadStationCard(const QJsonObject& cardData, const int id, const bool isMortgaged);
    void loadCompanyCard(const QJsonObject& cardData, const int id, const bool isMortgaged);
    
    BuyableCard* getCard(const int cardId);

    // Author: Jesse
    void loadStreetSquare(const QJsonObject& squareData, const int pos);
    void loadCompanySquare(const QJsonObject& squareData, const int pos);
    void loadStationSquare(const QJsonObject& squareData, const int pos);
    void loadStartSquare(const QJsonObject& squareData, const int pos);
    void loadIncomeTaxSquare(const QJsonObject& squareData, const int pos);
    void loadChanceSquare(const QJsonObject& squareData, const int pos);
    void loadCommunityChestSquare(const QJsonObject& squareData, const int pos);
    void loadJailSquare(const QJsonObject& squareData, const int pos);
    void loadBasicSquare(const QJsonObject& squareData, const int pos, const Square::TYPE type);

    // Author: Jesse
    void savePlayer(QJsonObject& data, QJsonObject& playerData, QJsonValueRef& playerRef);
    void saveCard(QJsonArray& cards, BuyableCard* card);

    // Author: Jesse
    template<typename T>
    void saveData(QJsonValueRef& playerRef, QString name, T value);
};

#endif // GAMELOADER_H
