#pragma once
#include <QWidget>
#include <QTimer>
#include <memory>
#include <vector>
#include "snake.h"
#include "food.h"
#include "enemy.h"

class GameWidget : public QWidget {
    Q_OBJECT
public:
    explicit GameWidget(QWidget *parent = nullptr);
    QSize sizeHint() const override { return {640, 480}; }

protected:
    void paintEvent(QPaintEvent*) override;
    void keyPressEvent(QKeyEvent *e) override;

private slots:
    void tick();

private:
    const int cellSize = 20;
    QTimer timer;
    int score = 0;
    bool paused = false;
    bool gameOver = false;

    std::unique_ptr<Snake> player;
    std::vector<std::unique_ptr<Item>> items;
    std::vector<std::unique_ptr<Enemy>> enemies;

    int cols() const { return std::max(1, width()  / cellSize); }
    int rows() const { return std::max(1, height() / cellSize); }

    QPoint randomEmptyCell() const;
    void resetGame();
    void spawnFood();
    void spawnEnemy();
    QRect cellRect(const QPoint &c) const {
        return { c.x()*cellSize, c.y()*cellSize, cellSize, cellSize };
    }
};
