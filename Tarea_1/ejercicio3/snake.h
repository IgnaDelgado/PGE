#pragma once
#include "character.h"
#include <deque>
class Snake : public Character {
public:
    Snake(QPoint start, int initialLength);
    void update(int cols, int rows) override;
    void setDirection(Direction d) override;
    void paint(QPainter &g, int cell) const override;
    QPoint head() const { return body.front(); }
    bool hitSelf() const;
    bool occupies(const QPoint &c) const;
    void grow(int amount);
    void takeDamage(int) override;
private:
    std::deque<QPoint> body;
    Direction dir = Direction::Right;
    int growth = 0;
};
