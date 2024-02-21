#ifndef CARDACTIONSWINDOW_H
#define CARDACTIONSWINDOW_H

#include <QDialog>
#include "Cards/BuyableCard.h"
#include "Player/Player.h"

namespace Ui {
class CardActionsWindow;
}

class CardActionsWindow : public QDialog
{
    Q_OBJECT

public:
    /* Constructors */
    explicit CardActionsWindow(QWidget *parent = nullptr);
    ~CardActionsWindow();

signals:
    void cardSold(Player* user);

public slots:
    void on_success() { close(); }
    void call(Player* player);
    void call(QVector<Player*>& players) { call(players.first()); }
    void setCard(BuyableCard* card) { m_card = card; }

private slots:
    void on_sell()  { m_card->executeAction(BuyableCard::ACTION::SELL); emit cardSold(m_user); }
    void on_build()  { m_card->executeAction(BuyableCard::ACTION::BUILD); }
    void on_mortgage()  { m_card->executeAction(BuyableCard::ACTION::MORTGAGE); }
    void on_unmortgage()  { m_card->executeAction(BuyableCard::ACTION::UNMORTGAGE); }
    void on_downgrade()  { m_card->executeAction(BuyableCard::ACTION::DOWNGRADE); }
    void on_exit() { close(); }

private:
    Ui::CardActionsWindow *ui;
    
    BuyableCard* m_card{};
    Player* m_user{};

    void showButtons();
    void connectButtons();
    void connectSignals();
};

#endif // CARDACTIONSWINDOW_H
