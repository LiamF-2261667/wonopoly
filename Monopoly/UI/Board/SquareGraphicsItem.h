
#ifndef SQUAREGRAPHICSITEM_H
#define SQUAREGRAPHICSITEM_H

#include <QGraphicsRectItem>

class Square;
class Player;

class SquareGraphicsItem : public QObject, public QGraphicsRectItem
{
    Q_OBJECT

public:
    /* Constructors */
    SquareGraphicsItem(Square* square, qreal width, qreal height, QGraphicsItem* parent = nullptr);

    /* Methods */
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

signals:
    void selected(Square* square);

private:
    const QString M_TEXTURE_PATH{ "://Images/Squares/square_" };

    Square* m_square{};
};

#endif // SQUAREGRAPHICSITEM_H
