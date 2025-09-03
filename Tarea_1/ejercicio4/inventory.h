#pragma once
#include <vector>
#include <optional>
#include "product.h"

// Diseño modular: la lógica de inventario vive separada de la UI
class Inventory {
public:
    // Agregar: falla si ya existe un producto con el mismo nombre
    bool addProduct(const Producto& p);
    // Actualizar por nombre (case-sensitive para simplicidad)
    bool updateProduct(const QString& nombre, int nuevaCantidad, double nuevoPrecio);
    // Eliminar por nombre
    bool removeProduct(const QString& nombre);

    // Consulta
    std::optional<Producto> findByName(const QString& nombre) const;
    const std::vector<Producto>& products() const { return items_; }

private:
    int indexOf(const QString& nombre) const;
    std::vector<Producto> items_;
};
