#pragma once
#include "entity.h"
class Character;
class Item : public Entity {
public:
    explicit Item(QPoint p) : pos_(p) {}
    virtual ~Item() = default;
    QPoint position() const { return pos_; }
    void setPosition(const QPoint &p) { pos_ = p; }
    virtual void apply(Character &c) = 0;
protected:
    QPoint pos_;
};
