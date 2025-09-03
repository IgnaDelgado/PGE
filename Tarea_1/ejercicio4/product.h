#pragma once
#include <QString>

// Clase Producto con encapsulamiento claro
class Producto {
public:
    Producto() = default;
    Producto(QString nombre, int cantidad, double precio);

    // Getters (solo lectura)
    const QString& nombre() const { return nombre_; }
    int cantidad() const { return cantidad_; }
    double precio() const { return precio_; }
    double valorTotal() const { return cantidad_ * precio_; }

    // Setters con validación básica
    void setNombre(const QString& n);
    void setCantidad(int c);
    void setPrecio(double p);

private:
    QString nombre_;
    int cantidad_ = 0;
    double precio_ = 0.0;
};
