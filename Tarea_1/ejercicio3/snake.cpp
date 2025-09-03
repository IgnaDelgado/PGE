#include "snake.h"
#include <algorithm>
Snake::Snake(QPoint start, int initialLength) {
    body.clear();
    for (int i=0; i<initialLength; ++i)
        body.push_front(QPoint(start.x()+i, start.y()));
}
void Snake::setDirection(Direction d) {
    if ((dir==Direction::Up    && d==Direction::Down) ||
        (dir==Direction::Down  && d==Direction::Up)   ||
        (dir==Direction::Left  && d==Direction::Right)||
        (dir==Direction::Right && d==Direction::Left))
        return;
    dir = d;
}
void Snake::update(int cols, int rows) {
    QPoint h = head();
    switch (dir) {
        case Direction::Up:    h.ry() -= 1; break;
        case Direction::Down:  h.ry() += 1; break;
        case Direction::Left:  h.rx() -= 1; break;
        case Direction::Right: h.rx() += 1; break;
    }
    if (h.x() < 0) h.setX(cols-1);
    if (h.y() < 0) h.setY(rows-1);
    if (h.x() >= cols) h.setX(0);
    if (h.y() >= rows) h.setY(0);
    body.push_front(h);
    if (growth > 0) { --growth; } else { body.pop_back(); }
}
bool Snake::hitSelf() const {
    const QPoint h = head();
    return std::count(body.begin()+1, body.end(), h) > 0;
}
bool Snake::occupies(const QPoint &c) const {
    return std::find(body.begin(), body.end(), c) != body.end();
}
void Snake::grow(int amount) { growth += amount; }
void Snake::takeDamage(int) {}
void Snake::paint(QPainter &g, int cell) const {
    g.setPen(QPen(QColor(0, 60, 40), 1));
    g.setBrush(QColor(0, 230, 130));
    g.drawRect(head().x()*cell, head().y()*cell, cell, cell);
    g.setBrush(QColor(0, 175, 105));
    for (size_t i=1; i<body.size(); ++i) {
        const QPoint &p = body[i];
        g.drawRect(p.x()*cell, p.y()*cell, cell, cell);
    }
}
