#pragma once
#include <string>
#include <ostream>

class Empleado {
public:
    Empleado(std::string nombre, int id, double salarioBase)
        : nombre_(std::move(nombre)), id_(id), salarioBase_(salarioBase) {}
    virtual ~Empleado() = default;

    // No copiar por error; se puede habilitar si se necesita
    Empleado(const Empleado&) = delete;
    Empleado& operator=(const Empleado&) = delete;

    // Acceso básico (encapsulamiento)
    const std::string& nombre() const { return nombre_; }
    int id() const { return id_; }
    double salarioBase() const { return salarioBase_; }
    void setSalarioBase(double s) { salarioBase_ = s; }

    // Interfaz polimórfica
    virtual std::string rol() const = 0;
    virtual double calcularSalario() const = 0;
    virtual void descrpcionDetallada(std::ostream& os) const = 0;

    // Común para imprimir ficha
    void imprimir(std::ostream& os) const;

protected:
    std::string nombre_;
    int id_;
    double salarioBase_;
};

inline std::ostream& operator<<(std::ostream& os, const Empleado& e) {
    e.imprimir(os);
    return os;
}
