#pragma once
#include "entity.h"
enum class Direction { Up, Down, Left, Right };
class Character : public Entity {
public:
    virtual ~Character() = default;
    virtual void update(int cols, int rows) = 0;
    virtual void setDirection(Direction d) = 0;
    virtual void attack(Entity&) {}
    virtual void takeDamage(int) {}
};
