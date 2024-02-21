#include "JailWindow.h"
#include "ui_JailWindow.h"
#include "Player/Player.h"
#include "Board/Jail.h"

JailWindow::JailWindow(Jail* jail, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::JailWindow),
    m_jail{ jail }
{
    ui->setupUi(this);
    setWindowFlag(Qt::WindowCloseButtonHint, false);

    /* Connect buttons */
    connect(ui->pay50Button, &QPushButton::released, this, &JailWindow::pay50);
    connect(ui->rollDiceButton, &QPushButton::released, this, &JailWindow::rollDice);
    connect(ui->useCardButton, &QPushButton::released, this, &JailWindow::useCard);
}


JailWindow::~JailWindow()
{
    delete ui;
}


void JailWindow::showPrompt(Player* currentPlayer) {
    m_currentPlayer = currentPlayer;

    if (!m_currentPlayer->canAfford(50))
        ui->pay50Button->setDisabled(true);
    else
        ui->pay50Button->setDisabled(false);

    if (m_currentPlayer->jailCardsOwned() == 0)
        ui->useCardButton->setDisabled(true);
    else
        ui->useCardButton->setDisabled(false);

    show();
}


void JailWindow::pay50() {
    m_currentPlayer->pay(50);
    m_jail->removeFromJail(m_currentPlayer);

    hide();
}


void JailWindow::rollDice() {
    m_jail->attemptExit(m_currentPlayer);

    hide();
}


void JailWindow::useCard() {
    m_currentPlayer->removeJailCard();
    m_jail->removeFromJail(m_currentPlayer);

    hide();
}
