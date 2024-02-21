
#ifndef PLAYER_H
#define PLAYER_H


#include <QObject>
#include "Balance.h"
#include "Cards/CardList.h"

class Player : public QObject
{
    Q_OBJECT
public:
    enum class Color {
        BLUE,
        GREEN,
        RED,
        YELLOW,
        PURPLE,
        ORANGE
    };

    /* Constructors */
    Player(Color color, int pos = 0) : m_color{ color }, m_pos{ pos } { } // Author: Liam

    /* Getters */
    int getPosition() const { return m_pos; } // Author: Liam
    Color getColor() const { return m_color; } // Author: Liam
    Money getMoney() const { return m_balance.getMoney(); } // Author: Liam
    BuyableCard* getCard(const int index) const { return m_cards[index]; } // Author: Liam
    int cardsOwned() const { return m_cards.size(); } // Author: Jesse
    int jailCardsOwned() const { return m_getOutOfJailCards; } // Author: Jesse
    QPair<int, int> buildingsOwned() const; // Author: Jesse
    bool isInJail() const { return m_isInJail; } // Author: Liam
    int exitAttempts() const { return m_jailExitAttempts; }  // Author: Jesse
    Player* getIndebtedPlayer() const { return m_indebtedPlayer; }  // Author: Jesse
    bool isInEffect() const { return m_isInEffect; } // Author: Jesse
    Money getNetWorth() const; // Author: Jesse

    /* Methods */
    void spawn(); // Author: Liam
    void goForward(const int amount); // Author: Liam
    void moveTo(const int position); // Author: Liam

    void inEffect(bool enable) { m_isInEffect = enable; } // Author: Jesse
    
    void addCard(BuyableCard* card);  // Author: Jesse
    void removeCard(const BuyableCard* card);  // Author: Jesse
    void addJailCard() { m_getOutOfJailCards++; } // Author: Jesse
    void addJailCard(const int amount) { m_getOutOfJailCards += amount; } // Author: Liam
    void removeJailCard() { m_getOutOfJailCards--; } // Author: Jesse
    void removeJailCard(const int amount) { m_getOutOfJailCards -= amount; } // Author: Liam

    bool canAfford(const Money amount); // Author: Jesse
    void pay(const Money amount, Player* receiver = nullptr); // Author: Jesse
    void gain(const Money amount); // Author: Jesse

    void changeHouseAmount(const int amount) { m_houses += amount; } // Author: Jesse
    void changeHotelAmount(const int amount) { m_hotels += amount; } // Author: Jesse

    void goToJail() { m_isInJail = true; } // Author: Liam
    void exitJail(); // Author: Jesse
    void tryExitJail() { ++m_jailExitAttempts; } // Author: Jesse

signals:
    void playerSpawned(Player* player);
    void playerMoved(Player* player, int toPos);
    void recoveredFromBankruptcy(Player* player);
    void balanceChanged(Player* player, int difference);

private:
    Color m_color{};
    int m_pos{};
    bool m_isInEffect{};

    CardList m_cards{};
    int m_getOutOfJailCards{ 0 };
    Balance m_balance{ -1 }; // If the balance doesn't get overriden by the gameloader, it will automaticly be removed from the game

    int m_houses{ 0 };
    int m_hotels{ 0 };

    Player* m_indebtedPlayer{ nullptr };

    bool m_isInJail{ false };
    int m_jailExitAttempts{ 0 };
};

#endif // PLAYER_H
