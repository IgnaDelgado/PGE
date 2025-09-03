#pragma once
#include <QWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QLabel>
#include "inventory.h"

// UI sencilla basada en QWidget para gestionar el inventario
class InventoryWidget : public QWidget {
    Q_OBJECT
public:
    explicit InventoryWidget(QWidget* parent = nullptr);

private slots:
    void onAdd();
    void onUpdate();
    void onRemove();
    void onClear();
    void onRowSelected(int row, int col);

private:
    void refreshTable();
    void showStatus(const QString& msg, bool ok);
    bool readForm(QString& nombre, int& cantidad, double& precio);

    Inventory inv_;

    // Controles
    QTableWidget* table_ = nullptr;
    QLineEdit*     nameEdit_ = nullptr;
    QSpinBox*      qtySpin_ = nullptr;
    QDoubleSpinBox* priceSpin_ = nullptr;
    QPushButton*   addBtn_ = nullptr;
    QPushButton*   updateBtn_ = nullptr;
    QPushButton*   removeBtn_ = nullptr;
    QPushButton*   clearBtn_ = nullptr;
    QLabel*        status_ = nullptr;
};
