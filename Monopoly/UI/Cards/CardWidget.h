#ifndef CARDWIDGET_H
#define CARDWIDGET_H

#include <QWidget>

class BuyableCard;
class QGraphicsPixmapItem;
class QGraphicsScene;
class QGraphicsColorizeEffect;

namespace Ui {
class CardWidget;
}

class CardWidget : public QWidget
{
    Q_OBJECT

public:
    /* Constructors */
    explicit CardWidget(BuyableCard* card, QWidget *parent = nullptr);
    ~CardWidget();

    void setScale(double scale);
    BuyableCard* getCard() { return m_card; }

private:
    const QString M_TEXTURE_PATH{ "://Images/Cards/card_" };
    const qreal M_MORTGAGED_OPACITY{ 0.3 };

    Ui::CardWidget *ui;

    QGraphicsScene* m_cardScene{};
    QGraphicsPixmapItem* m_cardPixmapItem{};
    BuyableCard* m_card{};
};

#endif // CARDWIDGET_H
