#pragma once
#include "item.h"
#include "character.h"
class Food : public Item {
public:
    Food(QPoint p, int growBy = 1);
    void apply(Character &c) override;
    void paint(QPainter &g, int cell) const override;
private:
    int growBy_;
};
