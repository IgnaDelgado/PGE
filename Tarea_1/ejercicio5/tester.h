#pragma once
#include "empleado.h"

class Tester : public Empleado {
public:
    Tester(std::string nombre, int id, double salarioBase, int bugsCerrados = 0, double bonusPorBug = 0.0)
        : Empleado(std::move(nombre), id, salarioBase),
          bugsCerrados_(bugsCerrados), bonusPorBug_(bonusPorBug) {}

    std::string rol() const override { return "Tester"; }
    double calcularSalario() const override { return salarioBase_ + bugsCerrados_ * bonusPorBug_; }
    void descrpcionDetallada(std::ostream& os) const override {
        os << "  Bugs cerrados: " << bugsCerrados_ << " (+" << bonusPorBug_ << " c/u)\n";
    }

    int bugsCerrados() const { return bugsCerrados_; }
    void setBugsCerrados(int n) { bugsCerrados_ = n; }
    double bonusPorBug() const { return bonusPorBug_; }
    void setBonusPorBug(double b) { bonusPorBug_ = b; }

private:
    int bugsCerrados_;
    double bonusPorBug_;
};
