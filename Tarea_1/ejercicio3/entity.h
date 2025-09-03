#pragma once
#include <QPoint>
#include <QPainter>
class Entity {
public:
    virtual ~Entity() = default;
    virtual void paint(QPainter &g, int cell) const = 0;
};
