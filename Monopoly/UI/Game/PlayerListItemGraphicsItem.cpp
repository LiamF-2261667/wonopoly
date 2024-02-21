
#include "PlayerListItemGraphicsItem.h"

#include "Player/Player.h"

#include "qbrush.h"
#include "qfont.h"

#include <UI/Player/PlayerGraphicsItem.h>

PlayerListItemGraphicsItem::PlayerListItemGraphicsItem(QRectF rect, Player* player, QGraphicsItem* parent)
    : QGraphicsItem(parent),
    m_border{ rect },
    m_player{ player }
{
    setupIcon();
    setupName();
    setupBalance();
    setupBackground();
}

/* Paint a player list item */
void PlayerListItemGraphicsItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    m_iconItem->paint(painter, option, widget);
    m_nameItem->paint(painter, option, widget);
    m_balanceItem->paint(painter, option, widget);
}

/* Setup the icon graphic */
void PlayerListItemGraphicsItem::setupIcon() {
    // Setting the icon size
    qreal radius = m_border.width() * M_ICON_WIDTH_RATIO;
    m_iconItem = new PlayerIconGraphicsItem{m_player, radius, this};

    // Setting the position
    m_iconItem->setPos(M_MARGING_INBETWEEN_RATIO * m_border.width(),
                       (m_border.height() - m_iconItem->rect().height())/2);
}

/* Setup the name grahpic */
void PlayerListItemGraphicsItem::setupName() {
    // Preparing the name
    QString playerId = QString::number(static_cast<int>(m_player->getColor()) + 1);
    QString name{ "Player " };
    name += playerId;

    // Setting the text + size
    m_nameItem = new QGraphicsTextItem{this};
    m_nameItem->setPos(m_border.width() * (M_ICON_WIDTH_RATIO + 2*M_MARGING_INBETWEEN_RATIO), M_TEXT_Y_OFFSET);
    m_nameItem->setTextWidth(m_border.width() * M_NAME_WIDTH_RATIO);
    m_nameItem->setFont(QFont(M_FONT_FAMILY));

    m_nameItem->setPlainText(name);
}

/* Setup the balance grpahic */
void PlayerListItemGraphicsItem::setupBalance() {
    // Setting the text + size
    m_balanceItem = new QGraphicsTextItem{this};
    m_balanceItem->setPos(m_border.width() * (M_ICON_WIDTH_RATIO + M_NAME_WIDTH_RATIO + 3*M_MARGING_INBETWEEN_RATIO), M_TEXT_Y_OFFSET);
    m_balanceItem->setTextWidth(m_border.width() * M_BALANCE_WIDTH_RATIO);
    m_balanceItem->setFont(QFont(M_FONT_FAMILY));

    m_balanceUpdateItem = new QGraphicsTextItem{this};
    m_balanceUpdateItem->setPos(m_border.width() * (M_ICON_WIDTH_RATIO + M_NAME_WIDTH_RATIO), M_TEXT_Y_OFFSET);
    m_balanceUpdateItem->setTextWidth(m_border.width() * M_BALANCE_WIDTH_RATIO);
    m_balanceUpdateItem->setFont(QFont(M_FONT_FAMILY));

    updateBalance(0);
}

/* Setup the background graphic */
void PlayerListItemGraphicsItem::setupBackground() {
    m_background = new QGraphicsRectItem{m_border, this};
    setInJail(m_player->isInJail());
    m_background->setZValue(-1);
}

/* Highlight or unhiglight the background if it's the current player */
void PlayerListItemGraphicsItem::setCurrentPlayer(bool value) {
    m_isCurrentPlayer = value;

    if (m_isInJail)
        value ? m_background->setBrush(M_HIGHLIGHTED_PRISON_BACKGROUND_COLOR) : m_background->setBrush(M_PRISON_BACKGROUND_COLOR);
    else
        value ? m_background->setBrush(M_HIGHLIGHTED_BACKGROUND_COLOR) : m_background->setBrush(M_DEFAULT_BACKGROUND_COLOR);
}

/* Highlight or unhiglight the background in the prison color */
void PlayerListItemGraphicsItem::setInJail(bool value) {
    m_isInJail = value;

    if (m_isCurrentPlayer)
        value ? m_background->setBrush(M_PRISON_BACKGROUND_COLOR) : m_background->setBrush(M_HIGHLIGHTED_BACKGROUND_COLOR);
    else
        value ? m_background->setBrush(M_PRISON_BACKGROUND_COLOR) : m_background->setBrush(M_DEFAULT_BACKGROUND_COLOR);
}

/* Update the balance string */
void PlayerListItemGraphicsItem::updateBalance(int difference) {
    // Preparing the balance string
    QString balanceAmount = QString::number(m_player->getMoney());
    QString balanceString{ "$" };
    balanceString += balanceAmount;

    // Setting the string
    m_balanceItem->setPlainText(balanceString);

    // Preparing the update string
    if (difference == 0) {
        m_balanceUpdateItem->setPlainText("");
        return;
    }

    QString updateAmount = QString::number(abs(difference));
    QString updateString{ "$" };
    if (difference < 0) {
        updateString = "-" + updateString;
        m_balanceUpdateItem->setDefaultTextColor("red");
    }
    else {
        updateString = "+" + updateString;
        m_balanceUpdateItem->setDefaultTextColor("green");
    }

    updateString += updateAmount;

    m_balanceUpdateItem->setPlainText(updateString);

}
