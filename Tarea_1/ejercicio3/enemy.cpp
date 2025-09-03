#include "enemy.h"
Enemy::Enemy(QPoint p, Behavior b) : pos(p), behavior(b) {}
void Enemy::update(const Character&, int cols, int rows) {
    if (behavior == Behavior::PatrolHorizontal) {
        pos.rx() += dir;
        if (pos.x() < 0) { pos.setX(0); dir = 1; }
        if (pos.x() >= cols) { pos.setX(cols-1); dir = -1; }
    } else {
        pos.ry() += dir;
        if (pos.y() < 0) { pos.setY(0); dir = 1; }
        if (pos.y() >= rows) { pos.setY(rows-1); dir = -1; }
    }
}
void Enemy::paint(QPainter &g, int cell) const {
    g.setPen(Qt::NoPen);
    g.setBrush(QColor(220, 60, 60));
    g.drawEllipse(pos.x()*cell, pos.y()*cell, cell, cell);
}
