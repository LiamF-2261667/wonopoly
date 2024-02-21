#ifndef ACTIONCARDWINDOW_H
#define ACTIONCARDWINDOW_H

#include <QDialog>

namespace Ui {
class ActionCardWindow;
}

class ActionCardWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ActionCardWindow(QWidget *parent = nullptr);
    ~ActionCardWindow();

public slots:
    void call(const QString& message);

private:
    Ui::ActionCardWindow *ui;
};

#endif // ACTIONCARDWINDOW_H
