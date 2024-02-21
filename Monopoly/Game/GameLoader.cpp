
#include "GameLoader.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <iostream>
#include <QString>

#include "Cards/BuyableCard.h"
#include "Cards/StreetCard.h"
#include "Cards/CompanyCard.h"
#include "Cards/StationCard.h"
#include "Cards/ActionCard.h"
#include "Game/Game.h"

#include "Board/Squares/Square.h"
#include "Board/Squares/StreetSquare.h"
#include "Board/Squares/CompanySquare.h"
#include "Board/Squares/StationSquare.h"
#include "Board/Squares/StartSquare.h"
#include "Board/Squares/IncomeTaxSquare.h"
#include "Board/Squares/FreeSquare.h"
#include "Board/Squares/PoliceSquare.h"
#include "Board/Squares/JailSquare.h"
#include "Board/Squares/ChanceSquare.h"
#include "Board/Squares/CommunityChestSquare.h"

#include <Player/Player.h>
#include <Board/Board.h>
#include "Board/Jail.h"

/* Author: Liam */
GameLoader::GameLoader(Board* board, Purchase* purchase, QVector<Player*>& players)
    : m_board{ board },
    m_purchase{ purchase },
    m_players{ players }
{
    for (int i = 0; i < Game::MAX_PLAYER_COUNT; ++i) {
        // Create a new player and give it a color according to the Player::Color enum
        m_players.append(new Player{(Player::Color)i});
    }
}

/* Author: Liam */
GameLoader::~GameLoader() {
    m_chancePool.clear();
    m_communityChestPool.clear();
}

/* Author: Liam */
/* Load a gamesetup*/
void GameLoader::loadGame(const QString& fileLocation, const int playerCount) {
    QJsonObject data{ readJsonData(fileLocation) };

    loadAllCards(data);
    loadCardSets(data);
    loadActionCards(data);
    loadAllSquares(data);
    m_board->initialize(m_squares, m_jail);

    loadGameInfo(data, playerCount);

    // Spawn the players on the board
    for (Player* player : m_players)
        player->spawn();
}

/* Author: Liam */
/*
 * Convert a given filelocation to json data
 * Requires the filelocation to be a valid json file
*/
QJsonObject GameLoader::readJsonData(const QString& fileLocation) {
    QFile file{ fileLocation };
    file.open(QIODevice::ReadOnly);

    QByteArray fileData{ file.readAll() };

    QJsonDocument jsonDoc{ QJsonDocument::fromJson(fileData) };
    file.close();
    return jsonDoc.object();
}

/* Author: Jesse */
/*
 * Convert a given filelocation to json data
 * Requires the filelocation to be a valid json file
*/
void GameLoader::writeJsonData(const QString& fileLocation, QJsonObject& jsonOb) {
    QFile file{ fileLocation };
    file.open(QIODevice::ReadOnly);

    QByteArray fileData{ file.readAll() };
    file.close();

    QJsonDocument jsonDoc{ QJsonDocument::fromJson(fileData) };
    jsonDoc.setObject(jsonOb);

    file.open(QFile::WriteOnly | QFile::Text | QFile::Truncate);

    file.write(jsonDoc.toJson());
    file.close();
}

/* Author: Liam */
/*
 * Load the gameinfo inside gamesetup data into the game
 * Requires the cards to be loaded into the game first
 * + gameinfo must be formatted correctly
*/
void GameLoader::loadGameInfo(const QJsonObject& data, int playerCount) {
    if (data.contains("gameinfo") && data["gameinfo"].isObject()) {
        QJsonObject gameinfo = data["gameinfo"].toObject();

        // Loading general data
        m_currentPlayerIndex = gameinfo["currentPlayer"].toInt();
        m_backToBackEquals = gameinfo["backToBackEquals"].toInt();

        if (gameinfo.contains("playerCount"))
            playerCount = gameinfo["playerCount"].toInt();

        // Loading the players
        QJsonArray players = gameinfo["players"].toArray();
        for (int i = 0; i < players.size(); ++i) {

            // Otherwise, load the player
            const QJsonValue player{ players[i] };
            loadPlayer(player.toObject());
        }

        // Remove players not needed in the game (according to bankruptcy)
        int j{ 0 }, k{ 0 };
        while (j < m_players.size()) {
            if (players[k++].toObject()["isBankrupt"].toBool()) {
                delete m_players[j];
                m_players.removeAt(j);
            } else {
                ++j;
            }
        }

        // Remove the players not needed in the game (according to the max player slider)
        qsizetype playersToRemove{ m_players.size() - playerCount };
        for (int i = 0; i < playersToRemove; ++i) {
            delete m_players[m_players.size() - 1];
            m_players.erase(m_players.cbegin() + m_players.size() - 1);
        }

        emit finishedLoadingPlayers(m_players);
    }
}

/* Author: Liam */
/*
 * Load a player from the json data into the game
 * Requires the cards to be loaded into the game first
 * + player data must be formatted correctly
*/
void GameLoader::loadPlayer(const QJsonObject& playerData) {
    // Getting the correct data
    int index = playerData["id"].toInt();
    int position = playerData["position"].toInt();
    Money money = playerData["money"].toInt();
    bool isInJail = playerData["inJail"].toBool();
    int getOutOfJailCards = playerData["getOutOfJailCards"].toInt();

    QVector<int> cardList{};
    for (const QJsonValue& cardId : playerData["cardList"].toArray()) {
        cardList += cardId.toInt();
    }


    // Getting the player
    Player* player{ m_players[index] };

    // Moving the player to the correct position
    player->moveTo(position);

    // Setting the right amount of money
    player->pay(player->getMoney());
    player->gain(money);

    // Adding the get out of jail cards
    for (int i = 0; i < getOutOfJailCards; ++i)
        player->addJailCard();

    // Sending the player to jail if nessesary
    if (isInJail) m_board->sendPlayerToJail(player);

    // Adding the cards to the player cardlist
    for (const int id : cardList) {
        // Getting the card* from id
        BuyableCard* currentCard{};
        for (BuyableCard* card : m_cards) {
            if (card->getId() == id) {
                currentCard = card;
                break;
            }
        }

        // Giving the card to the player
        player->addCard(currentCard);
    }
}

/* Author: Liam */
/* Load all the cards inside gamesetup data into the game */
void GameLoader::loadAllCards(const QJsonObject& data) {
    if (data.contains("cards") && data["cards"].isArray()) {
        QJsonArray cardsData = data["cards"].toArray();

        for(int i = 0; i < cardsData.size(); ++i) {
            if (cardsData[i].isObject())
                loadCard(cardsData[i].toObject());
        }
    }
}

/* Author: Liam */
/*
 * Load all the card sets into the cards
 * Requires the cards to be loaded into the game
*/
void GameLoader::loadCardSets(const QJsonObject& data) {
    QVector<QVector<StreetCard*>> cardSets{ getCardSets(data) };

    // Set the set for every streetCard
    for (BuyableCard* card : m_cards) {
        // Only streetcards have a set
        if (card->getType() != BuyableCard::TYPE::STREET) continue;
        StreetCard* streetCard = static_cast<StreetCard*>(card);

        // If the set contains the streetCard, that set belongs to it
        for (const QVector<StreetCard*>& cardSet : cardSets) {
            if (cardSet.contains(streetCard))
                streetCard->setCardSet(cardSet);
        }
    }
}

/* Author: Liam */
/* Converts a cardset list of ids to a list of Card* */
QVector<QVector<StreetCard*>> GameLoader::getCardSets(const QJsonObject& data) {
    QVector<QVector<int>> cardSetsIds{ getCardSetsIds(data) };

    // Initializing the new cardSets list
    QVector<QVector<StreetCard*>> cardSets{};
    for (const QVector<int>& cardSetIds : cardSetsIds)
        cardSets += QVector<StreetCard*>{};

    // Finding and entering the Card* in the correct list
    for (BuyableCard* card : m_cards) {
        // Only streetcards have a set
        if (card->getType() != BuyableCard::TYPE::STREET) continue;
        StreetCard* streetCard = static_cast<StreetCard*>(card);

        for (int i = 0; i < cardSetsIds.size(); ++i) {
            if (cardSetsIds[i].contains(streetCard->getId()))
                cardSets[i] += streetCard;
        }
    }

    return cardSets;
}

/* Author: Liam */
/* Converts a cardset list of ids from QJsonArray to QVector */
QVector<QVector<int>> GameLoader::getCardSetsIds(const QJsonObject& data) {
    QVector<QVector<int>> setIds{};

    if (data.contains("cardSets") && data["cardSets"].isArray()) {
        QJsonArray cardSets = data["cardSets"].toArray();

        // Convert the QJsonArray<QJsonArray<int>> to a QVector<QVector<int>>
        for (const QJsonValue& cardSet : cardSets) {
            // If the set ins't a list, it's incorrectly formatted
            if (!cardSet.isArray()) continue;

            QVector<int> ids{};
            for (const QJsonValue& id : cardSet.toArray())
                ids += id.toInt();

            setIds += ids;
        }
    }

    return setIds;
}

/* Author: Liam */
/*
 * Load a single card from the card data into the game
 * Requires the data to be formatted correctly
*/
void GameLoader::loadCard(const QJsonObject& cardData) {
    // Data for all card types
    int id = cardData["id"].toInt();
    BuyableCard::TYPE type = static_cast<BuyableCard::TYPE>(cardData["type"].toInt());
    bool isMortgaged = cardData["isMortgaged"].toBool();
    
    if (type == BuyableCard::TYPE::STREET)
        loadStreetCard(cardData, id, isMortgaged);
    
    if (type == BuyableCard::TYPE::STATION)
        loadStationCard(cardData, id, isMortgaged);
    
    if (type == BuyableCard::TYPE::COMPANY)
        loadCompanyCard(cardData, id, isMortgaged);
}

/* Author: Liam */
/* Load a street card into the game */
void GameLoader::loadStreetCard(const QJsonObject& cardData, const int id, const bool isMortgaged) {
    // Getting the type specific data
    Money price = cardData["price"].toInt();
    QJsonArray rentCostsJson = cardData["rentCosts"].toArray();
    Money mortgageCost = cardData["mortgageCost"].toInt();
    Money houseCost = cardData["houseCost"].toInt();
    Money hotelCost = cardData["hotelCost"].toInt();
    StreetCard::BUILD_TIER tier = static_cast<StreetCard::BUILD_TIER>(cardData["currentBuildTier"].toInt());

    // Convert the json array to an std::array<Money, 6>
    std::array<Money, 6> rentCosts;
    for (int i = 0; i < rentCostsJson.size(); ++i) {
        rentCosts[i] = rentCostsJson[i].toInt();
    }

    // Adding the card to the game
    BuyableCard* newStreetCard = new StreetCard(rentCosts, mortgageCost, houseCost, hotelCost, price, id, tier, isMortgaged);
    m_cards.append(newStreetCard);
}

/* Author: Liam */
/* Load a station card into the game */
void GameLoader::loadStationCard(const QJsonObject& cardData, const int id, const bool isMortgaged) {
    // Getting the type specific data
    Money price = cardData["price"].toInt();
    Money baseRent = cardData["baseRent"].toInt();
    Money mortgageCost = cardData["mortgageCost"].toInt();

    // Adding the card to the game
    BuyableCard* newStationCard = new StationCard(baseRent, price, mortgageCost, id, isMortgaged);
    m_cards.append(newStationCard);
}


/* Author: Liam */
/* Load a company card into the game */
void GameLoader::loadCompanyCard(const QJsonObject& cardData, const int id, const bool isMortgaged) {
    // Getting the type specific data
    Money price = cardData["price"].toInt();
    QJsonArray rentsJson = cardData["rentDiceMultipliers"].toArray();
    Money mortgageCost = cardData["mortgageCost"].toInt();

    // Convert the json array to an QVector
    QVector<Money> rents;
    for (int i = 0; i < rentsJson.size(); ++i) {
        rents.push_back(rentsJson[i].toInt());
    }

    // Adding the card to the game
    BuyableCard* newCompanyCard = new CompanyCard(rents, price, mortgageCost, id, isMortgaged);
    m_cards.append(newCompanyCard);
}

/* Author: Liam */
/* Return a specific card based on the id */
BuyableCard* GameLoader::getCard(const int cardId) {
    for (int i = 0; i < m_cards.size(); ++i) {
        if (m_cards[i]->getId() == cardId)
            return m_cards[i];
    }
    return nullptr;
}

/* Author: Jesse */
/* Load all action cards */
void GameLoader::loadActionCards(const QJsonObject& data) {
    // Load community chest cards
    if (data.contains("communitychest") && data["communitychest"].isArray()) {
        QJsonArray communityChestData = data["communitychest"].toArray();

        for (int i = 0; i < communityChestData.size(); ++i) {
            if (communityChestData[i].isObject()) {
                ActionCard* card = loadActionCard(communityChestData[i].toObject());
                m_communityChestPool.push_back(card);
            }
        }
    }

    // Load chance cards
    if (data.contains("chance") && data["chance"].isArray()) {
        QJsonArray chanceData = data["chance"].toArray();

        for (int i = 0; i < chanceData.size(); ++i) {
            if (chanceData[i].isObject()) {
                ActionCard* card = loadActionCard(chanceData[i].toObject());
                m_chancePool.push_back(card);
            }
        }
    }

    emit finishedLoadingActionCards(m_communityChestPool, m_chancePool);
}

/* Author: Jesse */
/* Load one specific action card and return it */
ActionCard* GameLoader::loadActionCard(const QJsonObject& cardData) {
    // Initializing variables
    ActionCard::ACTION type{};
    QString title{};
    Money amount{ 0 };
    QPair<Money, Money> amounts{ 0, 0 };
    int moveAmount{ 0 };
    int position{ 0 };

    // Adding variables
    if (cardData.contains("type"))
        type = ActionCard::ACTION(cardData["type"].toInt());

    if (cardData.contains("title"))
        title = cardData["title"].toString();

    if (cardData.contains("money"))
        amount = cardData["money"].toInt();

    if (cardData.contains("moneypair")) {
        QJsonArray moneyarray = cardData["moneypair"].toArray();
        amounts.first = moneyarray[0].toInt();
        amounts.second = moneyarray[1].toInt();
    }

    if (cardData.contains("moveAmount"))
        moveAmount = cardData["moveAmount"].toInt();

    if (cardData.contains("position"))
        position = cardData["position"].toInt();

    ActionCard* resultingCard =  new ActionCard( m_players, type, title, amount, amounts, moveAmount, position );

    return resultingCard;
}

/* Author: Jesse */
/* Load all the squares inside gamesetup data into game */
void GameLoader::loadAllSquares(const QJsonObject& data) {
    if (data.contains("squares") && data["squares"].isArray()) {
        QJsonArray squaresData = data["squares"].toArray();

        for (int i = 0; i < squaresData.size(); ++i) {
            if (squaresData[i].isObject())
                    loadSquare(squaresData[i].toObject());
        }
    }
}

/* Author: Jesse */
/*
 * Load a single square from the square data into the game
 * Requires the data to be formatted correctly
*/
void GameLoader::loadSquare(const QJsonObject& squareData) {
    int pos = squareData["pos"].toInt();
    Square::TYPE type = Square::TYPE(squareData["type"].toInt());

    if (type == Square::TYPE::STREET)
        loadStreetSquare(squareData, pos);

    else if (type == Square::TYPE::COMPANY)
        loadCompanySquare(squareData, pos);

    else if (type == Square::TYPE::STATION)
        loadStationSquare(squareData, pos);

    else if (type == Square::TYPE::START)
        loadStartSquare(squareData, pos);

    else if (type == Square::TYPE::INCOME_TAX)
        loadIncomeTaxSquare(squareData, pos);

    else if (type == Square::TYPE::CHANCE)
        loadChanceSquare(squareData, pos);

    else if (type == Square::TYPE::COMMUNITY_CHEST)
        loadCommunityChestSquare(squareData, pos);

    else if (type == Square::TYPE::JAIL)
        loadJailSquare(squareData, pos);

    else loadBasicSquare(squareData, pos, type);
}

/* Author: Jesse */
/* Load a street square into the game */
void GameLoader::loadStreetSquare(const QJsonObject& squareData, const int pos) {
    BuyableCard* card = getCard(squareData["cardId"].toInt());

    StreetSquare* square = new StreetSquare(card, m_purchase, pos);
    m_squares.push_back(square);
}

/* Author: Jesse */
/* Load a company square into the game */
void GameLoader::loadCompanySquare(const QJsonObject& squareData, const int pos) {
    BuyableCard* card = getCard(squareData["cardId"].toInt());

    CompanySquare* square = new CompanySquare(card, m_purchase, pos);
    m_squares.push_back(square);
}

/* Author: Jesse */
/* Load a station square into the game */
void GameLoader::loadStationSquare(const QJsonObject& squareData, const int pos) {
    BuyableCard* card = getCard(squareData["cardId"].toInt());

    StationSquare* square = new StationSquare(card, m_purchase, pos);
    m_squares.push_back(square);
}

/* Author: Jesse */
/* Load a start square into the game */
void GameLoader::loadStartSquare(const QJsonObject& squareData, const int pos) {
    Money money = squareData["gainForPassing"].toInt();

    StartSquare* square = new StartSquare(money, pos);
    m_squares.push_back(square);
}

/* Author: Jesse */
/* Load a income square into the game */
void GameLoader::loadIncomeTaxSquare(const QJsonObject& squareData, const int pos) {
    Money tax = squareData["taxAmount"].toInt();

    IncomeTaxSquare* square = new IncomeTaxSquare(tax, pos);
    m_squares.push_back(square);
}

/* Author: Jesse */
/* Load a chance square into the game */
void GameLoader::loadChanceSquare(const QJsonObject& squareData, const int pos) {
    ChanceSquare* square = new ChanceSquare(m_chancePool, pos);
    m_squares.push_back(square);
}

/* Author: Jesse */
/* Load a community square into the game */
void GameLoader::loadCommunityChestSquare(const QJsonObject& squareData, const int pos) {
    CommunityChestSquare* square = new CommunityChestSquare(m_communityChestPool, pos);
    m_squares.push_back(square);
}

/* Author: Jesse */
/* Load a jail square + a jail into the game */
void GameLoader::loadJailSquare(const QJsonObject& squareData, const int pos) {
    JailSquare* square = new JailSquare(pos);
    m_squares.push_back(square);

    m_jail = new Jail(square);
}

/* Author: Jesse */
/* Load a free/police square into the game */
void GameLoader::loadBasicSquare(const QJsonObject& squareData, const int pos, const Square::TYPE type) {
    Square* square{};

    if (type == Square::TYPE::FREE)
        square = new FreeSquare(pos);

    if (type == Square::TYPE::POLICE)
        square = new PoliceSquare(pos);

    m_squares.push_back(square);
}

/* Author: Jesse */
/* Saves the current state of the game */
void GameLoader::saveGame(int currentPlayerId, int backToBackTurns, const QString& fileLocation) {
    QJsonObject data{ readJsonData(M_DEFAULT_FILE_LOCATION) };
    QJsonValueRef dataref = data.find("gameinfo").value();

    saveData<int>(dataref, "playerCount", m_players.size());

    if (data.contains("gameinfo") && data["gameinfo"].isObject()) {
        QJsonObject gameinfo = data["gameinfo"].toObject();

        // Loading player data
        saveData<int>(dataref, "currentPlayer", currentPlayerId);
        saveData<int>(dataref, "backToBackEquals", backToBackTurns);

        // Save player data
        QJsonArray playerinfo = gameinfo["players"].toArray();
        for (int i = 0; i < playerinfo.size(); ++i) {
            QJsonValue player{ playerinfo[i] };
            QJsonObject playerData = player.toObject();
            QJsonValueRef playerRef = playerinfo[i];
            savePlayer(data, playerData, playerRef);
            saveData<QJsonArray>(dataref, "players", playerinfo);
        }
    }

    writeJsonData(fileLocation, data);
}

/* Author: Jesse */
/* Saves the data of a player */
void GameLoader::savePlayer(QJsonObject& data, QJsonObject& playerData, QJsonValueRef& playerRef) {
    // Variables
    QVector<int> playerIds{};
    QJsonArray cardIdList{};
    int i{};

    // Find the player in the player list
    for (int j = 0; j < m_players.size(); ++j) {
        playerIds.push_back((int)m_players[j]->getColor());

        if (playerData["id"].toInt() == (int)m_players[j]->getColor())
            i = j;
    }

    // If player isn't in the game, declare them as bankrupt and strip them of their cards
    if (!playerIds.contains(playerData["id"].toInt())) {
        saveData<bool>(playerRef, "isBankrupt", true);
        saveData<QJsonArray>(playerRef, "cardList", cardIdList);
        return;
    } else {
        saveData<bool>(playerRef, "isBankrupt", false);
    }

    // Save other variables
    saveData<int>(playerRef, "position", m_players[i]->getPosition());
    saveData<int>(playerRef, "money", m_players[i]->getMoney());
    saveData<bool>(playerRef, "inJail", m_players[i]->isInJail());
    saveData<int>(playerRef, "getOutOfJailCards", m_players[i]->jailCardsOwned());

    // Save cards
    QJsonArray cards = data["cards"].toArray();
    for (int j = 0; j < m_players[i]->cardsOwned(); ++j) {
        cardIdList.push_back(m_players[i]->getCard(j)->getId());
        saveCard(cards, m_players[i]->getCard(j));
    }

    saveData<QJsonArray>(playerRef, "cardList", cardIdList);
    data["cards"] = cards;
}

/* Author: Jesse */
/* Saves the data for a card (specifically street card) */
void GameLoader::saveCard(QJsonArray& cards, BuyableCard* card) {
    QJsonObject cardObject{};
    
    if (card->getType() != BuyableCard::TYPE::STREET) return;

    // Find card
    int i{ 0 };
    do {
        cardObject = cards[i++].toObject();
    } while (cardObject["id"].toInt() != card->getId());

    QJsonValueRef cardRef = cards[i - 1];

    saveData<bool>(cardRef, "isMortgaged", card->isMortgaged());

    // Save build tier
    StreetCard* streetCard = static_cast<StreetCard*>(card);

    saveData<int>(cardRef, "currentBuildTier", (int)streetCard->getTier());
}

/* Author: Jesse */
/* Save an object using its value reference */
template<typename T>
void GameLoader::saveData(QJsonValueRef& ref, QString name, T value) {
    QJsonObject ob = ref.toObject();
    ob.insert(name, value);
    ref = ob;
}
