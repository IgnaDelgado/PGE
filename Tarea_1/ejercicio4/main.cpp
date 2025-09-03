#include <QApplication>
#include "inventorywidget.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    InventoryWidget w;
    w.setWindowTitle("Gestión de Inventario - Producto (Qt + qmake)");
    w.resize(720, 420);
    w.show();
    return app.exec();
}
