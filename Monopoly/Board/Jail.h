/*
 * Author: Liam
*/

#ifndef JAIL_H
#define JAIL_H


#include "Player/Money.h"
#include <QVector>
#include <QObject>

class JailSquare;
class Player;


class Jail : public QObject
{
    Q_OBJECT

public:
    /* Constructors */
    Jail(const JailSquare* jailSquare) : QObject(), m_jailSquare{ jailSquare } { }

    /* Methods */
    void addToJail(Player* player);
    void removeFromJail(Player* player);
    void attemptExit(Player* player);
    void checkRoll(Player* player, const int rollAmount, const bool isDouble);

signals:
    void addedPlayerToJail(Player* player);
    void removedPlayerFromJail(Player* player);
    void attemptedExit();

private:
    const int M_MAX_EXIT_ATTEMPTS{ 3 };
    const Money M_EXIT_JAIL_COST{ 50 };

    const JailSquare* m_jailSquare;
};

#endif // JAIL_H
