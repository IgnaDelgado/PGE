#include "food.h"
#include "snake.h"
#include <QBrush>
Food::Food(QPoint p, int growBy) : Item(p), growBy_(growBy) {}
void Food::apply(Character &c) {
    if (auto *s = dynamic_cast<Snake*>(&c)) {
        s->grow(growBy_);
    }
}
void Food::paint(QPainter &g, int cell) const {
    g.setPen(Qt::NoPen);
    g.setBrush(QColor(255, 200, 0));
    g.drawRect(pos_.x()*cell, pos_.y()*cell, cell, cell);
}
