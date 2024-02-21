
#ifndef MAINMENUWINDOW_H
#define MAINMENUWINDOW_H

#include <QMainWindow>

class GameWindow;
class QFileDialog;

QT_BEGIN_NAMESPACE
namespace Ui { class MainMenuWindow; }
QT_END_NAMESPACE

class MainMenuWindow : public QMainWindow

{
    Q_OBJECT

public:
    /* Constructors */
    MainMenuWindow(QWidget *parent = nullptr);
    ~MainMenuWindow();

    /* Close the gamewindow when pressing X */
    void closeEvent(QCloseEvent* event) { m_gameStarted ? QMainWindow::closeEvent(event) : emit closedMainMenu(); }

signals:
    void startedGame(int playerCount, const QString& loadFile);
    void closedMainMenu();

public slots:
    void startGameButton();
    void loadGameButton();
    void exitMenuButton();
    void on_loadFileSelected(const QString& loadFile);

private:
    Ui::MainMenuWindow *ui;
    QFileDialog* m_fileDialog{};

    bool m_gameStarted{ false };
};

#endif // MAINMENUWINDOW_H
