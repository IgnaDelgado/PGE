#pragma once
#include "entity.h"
#include "character.h"
class Enemy : public Entity {
public:
    enum class Behavior { PatrolHorizontal, PatrolVertical };
    Enemy(QPoint pos, Behavior b);
    void update(const Character &player, int cols, int rows);
    void paint(QPainter &g, int cell) const override;
    QPoint position() const { return pos; }
private:
    QPoint pos;
    Behavior behavior;
    int dir = 1;
};
