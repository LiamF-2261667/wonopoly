/*
 * Author: Jesse
*/


#ifndef STREETCARD_H
#define STREETCARD_H

#include "BuyableCard.h"
#include <array>
#include <QVector>

class StreetCard : public BuyableCard
{
public:
    enum class BUILD_TIER {
        NO_HOUSE,
        HOUSE_1,
        HOUSE_2,
        HOUSE_3,
        HOUSE_4,
        HOTEL
    };

    /* Constructors */
    StreetCard(std::array<Money, 6>& rentCosts, const Money mortgageCost, Money houseCost,
               Money hotelCost, const Money price, const int id, BUILD_TIER tier, const bool isMortgaged)
        : BuyableCard(id, price, mortgageCost, isMortgaged), m_tier{ tier }, m_rentCosts{ rentCosts },
        m_houseCost{ houseCost }, m_hotelCost{ hotelCost } { }

    /* Setters */
    void setCardSet(QVector<StreetCard*> set) { m_set = set; }

    /* Getters */
    Money getPrice() const override;
    Money getSellValue() const override;
    Money getTotalValue() const override;
    Money getRent() const override;
    BuyableCard::TYPE getType() const override { return BuyableCard::TYPE::STREET; }
    QVector<ACTION> getActions(Player* user) const override;
    BUILD_TIER getTier() const { return m_tier; }

    /* Methods */
    void executeAction(ACTION action) override;
    bool canTrade(QVector<StreetCard*>& cards) const;

private:
    BUILD_TIER m_tier{ BUILD_TIER::NO_HOUSE };

    std::array<Money, 6> m_rentCosts{};
    Money m_houseCost{};
    Money m_hotelCost{};

    QVector<StreetCard*> m_set{};

    bool canBuild() const;
    bool canSell() const;
    bool canDowngrade() const;

    void build();
    void downgrade();
};

#endif // STREETCARD_H
