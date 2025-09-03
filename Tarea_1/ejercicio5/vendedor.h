#pragma once
#include "empleado.h"

class Vendedor : public Empleado {
public:
    Vendedor(std::string nombre, int id, double salarioBase, double comision, double ventasMes)
        : Empleado(std::move(nombre), id, salarioBase),
          comision_(comision), ventasMes_(ventasMes) {}

    std::string rol() const override { return "Vendedor"; }
    double calcularSalario() const override { return salarioBase_ + ventasMes_ * comision_; }
    void descrpcionDetallada(std::ostream& os) const override {
        os << "  Ventas del mes: $" << ventasMes_ << " @ comisión " << (comision_ * 100.0) << "%\n";
    }

    double comision() const { return comision_; }
    void setComision(double c) { comision_ = c; }
    double ventasMes() const { return ventasMes_; }
    void setVentasMes(double v) { ventasMes_ = v; }

private:
    double comision_;   // 0.05 = 5%
    double ventasMes_;
};
