/*
 * Author: Liam
*/

#ifndef COMPANYCARD_H
#define COMPANYCARD_H


#include "BuyableCard.h"

#include <QVector>


class CompanyCard : public BuyableCard
{
public:
    /* Constructors */
    CompanyCard(QVector<Money>& rents, const Money price, const Money mortgageCost, const int id, const bool isMortgaged)
        : BuyableCard(id, price, mortgageCost, isMortgaged), m_rents{ rents } { }

    /* Getters */
    Money getRent() const override;
    BuyableCard::TYPE getType() const override { return BuyableCard::TYPE::COMPANY; }
    QVector<ACTION> getActions(Player* user) const override; // Author: Jesse

    /* Methods */
    void executeAction(ACTION action) override; // Author: Jesse


private:
    QVector<Money> m_rents{};
};

#endif // COMPANYCARD_H
