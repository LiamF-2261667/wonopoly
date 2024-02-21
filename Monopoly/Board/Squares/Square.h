/*
 * Author: Liam
*/

#ifndef SQUARE_H
#define SQUARE_H


#include <QObject>

class BuyableCard;
class Player;

class Square : public QObject
{
    Q_OBJECT

public:
    enum class TYPE {
        START,
        STREET,
        STATION,
        COMPANY,
        INCOME_TAX,
        CHANCE,
        COMMUNITY_CHEST,
        FREE,
        JAIL,
        POLICE
    };

    /* Constructors */
    Square(const int pos = 0, BuyableCard* card = nullptr) : M_POS{ pos }, m_card{ card } { }

    /* Getters */
    int getPos() const { return M_POS; }
    BuyableCard* getCard() const { return m_card; }
    virtual TYPE getType() const = 0;

    /* Methods */
    virtual void doEffect(Player* player) = 0;

signals:
    void inEffect();

private:
    const int M_POS;
    BuyableCard* m_card;
};

#endif // SQUARE_H
