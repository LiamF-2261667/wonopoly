#ifndef JAILWINDOW_H
#define JAILWINDOW_H

#include <QDialog>

class Player;
class Jail;

namespace Ui {
class JailWindow;
}

class JailWindow : public QDialog
{
    Q_OBJECT

public:
    /* Constructors */
    explicit JailWindow(Jail* jail, QWidget *parent = nullptr);
    ~JailWindow();

public slots:
    void showPrompt(Player* currentPlayer);

private slots:
    void pay50();
    void rollDice();
    void useCard();

private:
    Ui::JailWindow *ui;

    Jail* m_jail{};
    Player* m_currentPlayer{};
};

#endif // JAILWINDOW_H
