#include "gamewidget.h"
#include <QPainter>
#include <QKeyEvent>
#include <QRandomGenerator>

GameWidget::GameWidget(QWidget *parent)
    : QWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);
    setMinimumSize(480, 400);
    resetGame();
    connect(&timer, &QTimer::timeout, this, &GameWidget::tick);
    timer.start(120);
    update();
}

void GameWidget::resetGame() {
    score = 0;
    paused = false;
    gameOver = false;
    player = std::make_unique<Snake>(QPoint(5, 5), 4);
    items.clear();
    enemies.clear();
    spawnFood();
    spawnEnemy();
}

QPoint GameWidget::randomEmptyCell() const {
    for (int tries = 0; tries < 500; ++tries) {
        int x = QRandomGenerator::global()->bounded(cols());
        int y = QRandomGenerator::global()->bounded(rows());
        QPoint p(x, y);
        if (player && player->occupies(p)) continue;
        bool conflict = false;
        for (auto &it : items)   if (it->position() == p) { conflict = true; break; }
        for (auto &en : enemies) if (en->position() == p) { conflict = true; break; }
        if (!conflict) return p;
    }
    return {1,1};
}

void GameWidget::spawnFood()  { items.emplace_back(std::make_unique<Food>(randomEmptyCell(), 1)); }
void GameWidget::spawnEnemy() { enemies.emplace_back(std::make_unique<Enemy>(randomEmptyCell(), Enemy::Behavior::PatrolHorizontal)); }

void GameWidget::paintEvent(QPaintEvent*) {
    QPainter g(this);
    g.fillRect(rect(), QColor(20,20,28));
    g.setPen(QColor(40,40,52));
    for (int x = 0; x < width();  x += cellSize) g.drawLine(x, 0, x, height());
    for (int y = 0; y < height(); y += cellSize) g.drawLine(0, y, width(), y);
    for (auto &it : items)   it->paint(g, cellSize);
    for (auto &en : enemies) en->paint(g, cellSize);
    if (player)              player->paint(g, cellSize);
    g.setPen(Qt::white);
    g.drawText(10, 20, QString("Score: %1").arg(score));
    if (paused)  g.drawText(10, 40, "Pausa (P)");
    if (gameOver) g.drawText(10, 60, "Game Over - Enter para reiniciar");
}

void GameWidget::keyPressEvent(QKeyEvent *e) {
    if (gameOver) {
        if (e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter) resetGame();
        return;
    }
    switch (e->key()) {
        case Qt::Key_P: paused = !paused; break;
        case Qt::Key_Up:    if (player) player->setDirection(Direction::Up);    break;
        case Qt::Key_Down:  if (player) player->setDirection(Direction::Down);  break;
        case Qt::Key_Left:  if (player) player->setDirection(Direction::Left);  break;
        case Qt::Key_Right: if (player) player->setDirection(Direction::Right); break;
        default: QWidget::keyPressEvent(e);
    }
}

void GameWidget::tick() {
    if (paused || gameOver) { update(); return; }
    for (auto &en : enemies) en->update(*player, cols(), rows());
    player->update(cols(), rows());
    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i]->position() == player->head()) {
            items[i]->apply(*player);
            score += 10;
            items.erase(items.begin() + static_cast<long>(i));
            spawnFood();
            break;
        }
    }
    if (player->hitSelf()) gameOver = true;
    for (auto &en : enemies)
        if (en->position() == player->head()) { player->takeDamage(1); gameOver = true; }
    update();
}
