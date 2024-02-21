
#ifndef PLAYERLISTITEMGRAPHICSITEM_H
#define PLAYERLISTITEMGRAPHICSITEM_H


#include <QGraphicsItem>

#include <UI/Player/PlayerIconGraphicsItem.h>

class Player;


class PlayerListItemGraphicsItem : public QGraphicsItem
{
public:
    /* Constructors*/
    PlayerListItemGraphicsItem(QRectF rect, Player* player, QGraphicsItem* parent = nullptr);

    /* Required overrides for QGraphicsItem */
    QRectF boundingRect() const override { return m_border; }
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;

    /* Getters */
    Player* getPlayer() const { return m_player; };

    /* Methods */
    void setCurrentPlayer(bool value = true);
    void setInJail(bool value = true);
    void updateBalance(int difference);

private:
    const qreal M_MARGING_INBETWEEN_RATIO{ 0.05 };

    QRectF m_border{};
    // The % of width required for a specific graphic element of the border width
    const qreal
        M_ICON_WIDTH_RATIO{ 0.11 },
        M_NAME_WIDTH_RATIO{ 0.6 - M_MARGING_INBETWEEN_RATIO},
        M_BALANCE_WIDTH_RATIO{ 1 - M_ICON_WIDTH_RATIO - M_NAME_WIDTH_RATIO };

    const qreal M_TEXT_Y_OFFSET{ m_border.height()/2 - 14.0 };
    const QString M_FONT_FAMILY{ "Helvetica" };

    Player* m_player{};

    PlayerIconGraphicsItem* m_iconItem{};
    QGraphicsTextItem* m_nameItem{};
    QGraphicsTextItem* m_balanceItem{};
    QGraphicsTextItem* m_balanceUpdateItem{};

    QColor M_DEFAULT_BACKGROUND_COLOR{ 222, 223, 223 };
    QColor M_HIGHLIGHTED_BACKGROUND_COLOR{ 255, 255, 255 };
    QColor M_PRISON_BACKGROUND_COLOR{ 255, 152, 56 };
    QColor M_HIGHLIGHTED_PRISON_BACKGROUND_COLOR{ 250, 188, 130 };
    QGraphicsRectItem* m_background{};

    bool m_isCurrentPlayer{ false };
    bool m_isInJail{ false };

    void setupIcon();
    void setupName();
    void setupBalance();
    void setupBackground();
};

#endif // PLAYERLISTITEMGRAPHICSITEM_H
