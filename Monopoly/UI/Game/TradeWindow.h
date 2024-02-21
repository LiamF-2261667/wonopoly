#ifndef TRADEWINDOW_H
#define TRADEWINDOW_H

#include <QDialog>
#include <QGroupBox>
#include <UI/Cards/CardWidget.h>
#include <UI/Cards/CardInspectButton.h>
#include "Cards/BuyableCard.h"

class Player;
class Trade;

namespace Ui {
class TradeWindow;
}

class TradeWindow : public QDialog
{
    Q_OBJECT

public:
    explicit TradeWindow(QWidget *parent = nullptr);
    ~TradeWindow();

    void call(QVector<Player*> players);

public slots:
    void on_cardPress(BuyableCard* card);
    void onTradeFailure(QString reason);
    void onTradeSuccess() { close(); }

private slots:
    void on_confirmButtonRelease();
    void on_declineButtonRelease() { close(); }

private:
    typedef struct CardGroup {
        QGroupBox* groupBox;
        CardWidget* cardWidget;
        CardInspectButton* pushButton;
        static const int BASE_WIDTH{ 125 };
        bool selected{ false };
    } CardGroup ;

    Ui::TradeWindow *ui;

    Trade* m_trade{};
    QPair<Player*, Player*> m_players{nullptr, nullptr};
    QVector<CardGroup> m_cardGroups{};
    QVector<CardGroup*> m_selectedCardGroups1{};
    QVector<CardGroup*> m_selectedCardGroups2{};

    QVector<QLayout*> m_currentLayouts{};
    QVector<QWidget*> m_currentWidgets{};

    void setupPlayerDetails();
    void setupCardLists();
    CardGroup createCardGroup(BuyableCard* card);

    void clear();

    void onAdd(CardGroup* selectedCard);
    void onRemove(CardGroup* selectedCard);
};

#endif // TRADEWINDOW_H
