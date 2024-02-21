#ifndef CARDLISTWINDOW_H
#define CARDLISTWINDOW_H

#include <QDialog>
#include "Cards/CardList.h"

class Player;
class QGroupBox;
class CardInspectButton;
class CardWidget;
class CardActionsWindow;
class BuyableCard;

namespace Ui {
class CardListWindow;
}

class CardListWindow : public QDialog
{
    Q_OBJECT

public:
    /* Constructors */
    explicit CardListWindow(CardActionsWindow* cardActionsWindow, QWidget *parent = nullptr);
    ~CardListWindow();

    /* Methods */
    void call(Player* owner);

public slots:
    void on_cardInspection();
    void on_bankruptcy(Player* player);
    void refresh(Player* user);

private slots:
    void on_ExitButton_released() { close(); m_open = false; }
    void on_sellJailCard_released();

    void on_CardListWindow_rejected();

private:
    typedef struct CardGroup {
        QGroupBox* groupBox;
        CardWidget* cardWidget;
        CardInspectButton* pushButton;
        static const int BASE_WIDTH{ 311 };
    } CardGroup ;

    Ui::CardListWindow *ui;
    CardActionsWindow* m_cardActionsWindow{};

    CardList m_cardList{};
    Player* m_owner{};

    QVector<CardGroup> m_cardGroups{};
    QLayout* m_currentLayout{};
    QWidget* m_currentWidget{};

    bool m_open{ false };
    
    CardGroup createCardGroup(BuyableCard* card);
    void sortCardList(); // using bubble sort algorithm
};

#endif // CARDLISTWINDOW_H
