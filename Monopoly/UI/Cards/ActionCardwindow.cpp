#include "ActionCardwindow.h"
#include "ui_ActionCardwindow.h"

ActionCardWindow::ActionCardWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ActionCardWindow)
{
    ui->setupUi(this);
    connect(ui->pushButton, &QPushButton::clicked, this, &ActionCardWindow::close);
}

ActionCardWindow::~ActionCardWindow()
{
    delete ui;
}

void ActionCardWindow::call(const QString& message) {
    ui->message->setText(message);

    show();
}
