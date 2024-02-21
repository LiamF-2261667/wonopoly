
#include "SquareGraphicsItem.h"
#include "Board/Squares/Square.h"
#include "Cards/BuyableCard.h"


SquareGraphicsItem::SquareGraphicsItem(Square* square, qreal width, qreal height, QGraphicsItem* parent)
    : QGraphicsRectItem(0, 0, width, height, parent),
    m_square{ square }
{
    // Get the square texture
    QString identifierChar = QString::number((int)square->getType());
    if (Square::TYPE::STREET  == m_square->getType() ||
        Square::TYPE::COMPANY == m_square->getType() ||
        Square::TYPE::STATION == m_square->getType())
        identifierChar += '_' + QString::number(square->getCard()->getId());

    if (Square::TYPE::INCOME_TAX  == m_square->getType())
        identifierChar += '_' + QString::number(square->getPos());

    QString currentTexturePath = M_TEXTURE_PATH + identifierChar +".png";

    // Setup the card texture
    QPixmap file{currentTexturePath};
    QGraphicsPixmapItem* texture = new QGraphicsPixmapItem(file);
    texture->setTransformationMode(Qt::SmoothTransformation);
    texture->setParentItem(this);
    texture->setScale(rect().width() / file.width());

    // Making the square interactable
    setFlag(ItemIsSelectable);
}

void SquareGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    if (m_square->getCard() != nullptr) {
        emit selected(m_square);
    }

    QGraphicsRectItem::mouseReleaseEvent(event);
}
