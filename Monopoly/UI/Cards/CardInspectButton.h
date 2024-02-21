
#ifndef CARDINSPECTBUTTON_H
#define CARDINSPECTBUTTON_H

#include <QPushButton>

class BuyableCard;



class CardInspectButton : public QPushButton
{
    Q_OBJECT

public:
    CardInspectButton(BuyableCard* card, QWidget* parent = nullptr) : QPushButton("Inspect", parent), m_card{ card } { }

    bool hitButton(const QPoint &pos) const override { emit clicked(m_card); return true; }

signals:
    void clicked(BuyableCard* card) const;

private:
    BuyableCard* m_card{};
};

#endif // CARDINSPECTBUTTON_H
