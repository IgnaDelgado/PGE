#include "inventorywidget.h"
#include <QGridLayout>
#include <QHeaderView>
#include <QMessageBox>

InventoryWidget::InventoryWidget(QWidget* parent) : QWidget(parent) {
    auto* layout = new QGridLayout(this);

    // Tabla
    table_ = new QTableWidget(this);
    table_->setColumnCount(4);
    table_->setHorizontalHeaderLabels({ "Nombre", "Cantidad", "Precio", "Valor Total" });
    table_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table_->setSelectionBehavior(QAbstractItemView::SelectRows);
    table_->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(table_, &QTableWidget::cellClicked, this, &InventoryWidget::onRowSelected);

    // Formulario
    nameEdit_ = new QLineEdit(this);
    qtySpin_ = new QSpinBox(this);
    qtySpin_->setRange(0, 1000000);
    priceSpin_ = new QDoubleSpinBox(this);
    priceSpin_->setRange(0.0, 1e9);
    priceSpin_->setDecimals(2);

    addBtn_ = new QPushButton("Agregar", this);
    updateBtn_ = new QPushButton("Actualizar", this);
    removeBtn_ = new QPushButton("Eliminar", this);
    clearBtn_ = new QPushButton("Limpiar", this);
    status_ = new QLabel(this);

    connect(addBtn_, &QPushButton::clicked, this, &InventoryWidget::onAdd);
    connect(updateBtn_, &QPushButton::clicked, this, &InventoryWidget::onUpdate);
    connect(removeBtn_, &QPushButton::clicked, this, &InventoryWidget::onRemove);
    connect(clearBtn_, &QPushButton::clicked, this, &InventoryWidget::onClear);

    int r = 0;
    layout->addWidget(table_, r, 0, 1, 6); r++;
    layout->addWidget(new QLabel("Nombre:"), r, 0);
    layout->addWidget(nameEdit_, r, 1);
    layout->addWidget(new QLabel("Cantidad:"), r, 2);
    layout->addWidget(qtySpin_, r, 3);
    layout->addWidget(new QLabel("Precio:"), r, 4);
    layout->addWidget(priceSpin_, r, 5); r++;
    layout->addWidget(addBtn_, r, 0);
    layout->addWidget(updateBtn_, r, 1);
    layout->addWidget(removeBtn_, r, 2);
    layout->addWidget(clearBtn_, r, 3, 1, 2);
    layout->addWidget(status_, r, 5);
    setLayout(layout);

    // Seed inicial (opcional)
    inv_.addProduct(Producto("Mouse", 10, 2500.0));
    inv_.addProduct(Producto("Teclado", 5, 8000.0));
    inv_.addProduct(Producto("Monitor", 3, 120000.0));
    refreshTable();
}

bool InventoryWidget::readForm(QString& nombre, int& cantidad, double& precio) {
    nombre = nameEdit_->text().trimmed();
    cantidad = qtySpin_->value();
    precio = priceSpin_->value();
    if (nombre.isEmpty()) {
        showStatus("El nombre es obligatorio.", false);
        nameEdit_->setFocus();
        return false;
    }
    return true;
}

void InventoryWidget::onAdd() {
    try {
        QString n; int c; double p;
        if (!readForm(n, c, p)) return;
        Producto prod(n, c, p);
        if (!inv_.addProduct(prod)) {
            showStatus("Ya existe un producto con ese nombre.", false);
            return;
        }
        refreshTable();
        showStatus("Producto agregado.", true);
    } catch (const std::exception& e) {
        showStatus(QString("Error: %1").arg(e.what()), false);
    }
}

void InventoryWidget::onUpdate() {
    try {
        QString n; int c; double p;
        if (!readForm(n, c, p)) return;
        if (!inv_.updateProduct(n, c, p)) {
            showStatus("No existe un producto con ese nombre.", false);
            return;
        }
        refreshTable();
        showStatus("Producto actualizado.", true);
    } catch (const std::exception& e) {
        showStatus(QString("Error: %1").arg(e.what()), false);
    }
}

void InventoryWidget::onRemove() {
    QString n = nameEdit_->text().trimmed();
    if (n.isEmpty()) { showStatus("Indique un nombre para eliminar.", false); return; }
    if (!inv_.removeProduct(n)) {
        showStatus("No existe un producto con ese nombre.", false);
        return;
    }
    refreshTable();
    showStatus("Producto eliminado.", true);
}

void InventoryWidget::onClear() {
    nameEdit_->clear();
    qtySpin_->setValue(0);
    priceSpin_->setValue(0.0);
    table_->clearSelection();
    showStatus("Formulario limpio.", true);
}

void InventoryWidget::onRowSelected(int row, int /*col*/) {
    if (row < 0 || row >= table_->rowCount()) return;
    nameEdit_->setText(table_->item(row, 0)->text());
    qtySpin_->setValue(table_->item(row, 1)->text().toInt());
    priceSpin_->setValue(table_->item(row, 2)->text().toDouble());
}

void InventoryWidget::refreshTable() {
    const auto& items = inv_.products();
    table_->setRowCount(static_cast<int>(items.size()));
    for (int i = 0; i < static_cast<int>(items.size()); ++i) {
        const auto& p = items[static_cast<size_t>(i)];
        table_->setItem(i, 0, new QTableWidgetItem(p.nombre()));
        table_->setItem(i, 1, new QTableWidgetItem(QString::number(p.cantidad())));
        table_->setItem(i, 2, new QTableWidgetItem(QString::number(p.precio(), 'f', 2)));
        table_->setItem(i, 3, new QTableWidgetItem(QString::number(p.valorTotal(), 'f', 2)));
    }
    table_->resizeRowsToContents();
}

void InventoryWidget::showStatus(const QString& msg, bool ok) {
    status_->setText(msg);
    QPalette pal = status_->palette();
    pal.setColor(QPalette::WindowText, ok ? Qt::darkGreen : Qt::red);
    status_->setPalette(pal);
}
