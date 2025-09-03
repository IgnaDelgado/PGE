#include "product.h"
#include <stdexcept>

Producto::Producto(QString nombre, int cantidad, double precio) {
    setNombre(nombre);
    setCantidad(cantidad);
    setPrecio(precio);
}

void Producto::setNombre(const QString& n) {
    if (n.trimmed().isEmpty()) throw std::invalid_argument("El nombre no puede estar vacío.");
    nombre_ = n.trimmed();
}

void Producto::setCantidad(int c) {
    if (c < 0) throw std::invalid_argument("La cantidad no puede ser negativa.");
    cantidad_ = c;
}

void Producto::setPrecio(double p) {
    if (p < 0.0) throw std::invalid_argument("El precio no puede ser negativo.");
    precio_ = p;
}
