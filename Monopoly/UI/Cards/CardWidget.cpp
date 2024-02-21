#include "CardWidget.h"
#include "ui_CardWidget.h"

#include "Cards/BuyableCard.h"

#include <QGraphicsColorizeEffect>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>

CardWidget::CardWidget(BuyableCard* card, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CardWidget),
    m_card{ card }
{
    ui->setupUi(this);

    // Setup the scene for the card graphic
    m_cardScene = new QGraphicsScene{this};
    m_cardScene->setSceneRect(ui->graphicsView->rect());
    ui->graphicsView->setScene(m_cardScene);

    // Get the card texture
    QString currentTexturePath = M_TEXTURE_PATH + QString::number(card->getId()) +".png";

    // Setup the card texture
    m_cardPixmapItem = new QGraphicsPixmapItem{ QPixmap(currentTexturePath) };
    m_cardPixmapItem->setTransformationMode(Qt::SmoothTransformation);
    m_cardScene->addItem(m_cardPixmapItem);

    // Add the needed indications
    if (m_card->isMortgaged()) m_cardPixmapItem->setOpacity(M_MORTGAGED_OPACITY);

    show();
}

CardWidget::~CardWidget()
{
    delete ui;
}

/* Set the scale of the graphics item */
void CardWidget::setScale(double scale) {
    m_cardPixmapItem->setScale(scale);
}
