
#include "MainMenuWindow.h"
#include "Game/Game.h"
#include "ui_MainMenuWindow.h"
#include "QFileDialog"

#include <QPushButton>


MainMenuWindow::MainMenuWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainMenuWindow)
{
    ui->setupUi(this);

    /* Connect buttons with functions */
    connect(ui->startButton, &QPushButton::released, this, &MainMenuWindow::startGameButton);
    connect(ui->loadButton, &QPushButton::released, this, &MainMenuWindow::loadGameButton);
    connect(ui->exitButton, &QPushButton::released, this, &MainMenuWindow::exitMenuButton);

    // TODO: enable the load button if there is a save file available, else disable it
}


MainMenuWindow::~MainMenuWindow()
{
    delete ui;
}


/* Starts the game when the start button is pressed */
void MainMenuWindow::startGameButton() {
    m_gameStarted = true;
    close();

    emit startedGame(ui->playerAmountSpinBox->value(), QString());
}


/* Loads and starts a game using a save file when the load button is pressed */
void MainMenuWindow::loadGameButton() {
    m_fileDialog = new QFileDialog{ this, "Select the file to load from" };
    m_fileDialog->setFileMode(QFileDialog::ExistingFile);

    connect(m_fileDialog, &QFileDialog::fileSelected, this, &MainMenuWindow::on_loadFileSelected);

    m_fileDialog->open();

}

/* Start the game with the selected loadfile */
void MainMenuWindow::on_loadFileSelected(const QString& loadFile) {
    m_gameStarted = true;
    close();

    emit startedGame(Game::MAX_PLAYER_COUNT, loadFile);
}


/* Exits the menu when the exit button is pressed */
void MainMenuWindow::exitMenuButton() {
    this->close();
    emit closedMainMenu();
}

