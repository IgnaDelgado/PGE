#include <QApplication>
#include "gamewidget.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    GameWidget w;
    w.setWindowTitle("Viborita - Qt (qmake + QWidget)");
    w.show();
    return app.exec();
}
