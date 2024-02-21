
#include "UI/Game/GameWindow.h"

#include <QApplication>
#include <QStyleFactory>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle(QStyleFactory::create("Fusion"));

    GameWindow w{};

    return a.exec();
}
