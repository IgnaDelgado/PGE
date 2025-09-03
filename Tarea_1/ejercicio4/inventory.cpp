#include "inventory.h"

int Inventory::indexOf(const QString& nombre) const {
    for (size_t i = 0; i < items_.size(); ++i) {
        if (items_[i].nombre() == nombre) return static_cast<int>(i);
    }
    return -1;
}

bool Inventory::addProduct(const Producto& p) {
    if (indexOf(p.nombre()) != -1) return false;
    items_.push_back(p);
    return true;
}

bool Inventory::updateProduct(const QString& nombre, int nuevaCantidad, double nuevoPrecio) {
    int idx = indexOf(nombre);
    if (idx < 0) return false;
    items_[idx].setCantidad(nuevaCantidad);
    items_[idx].setPrecio(nuevoPrecio);
    return true;
}

bool Inventory::removeProduct(const QString& nombre) {
    int idx = indexOf(nombre);
    if (idx < 0) return false;
    items_.erase(items_.begin() + idx);
    return true;
}

std::optional<Producto> Inventory::findByName(const QString& nombre) const {
    int idx = indexOf(nombre);
    if (idx < 0) return std::nullopt;
    return items_[idx];
}
