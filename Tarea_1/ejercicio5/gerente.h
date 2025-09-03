#pragma once
#include "empleado.h"
#include <vector>

class Gerente : public Empleado {
public:
    Gerente(std::string nombre, int id, double salarioBase, double bono);
    std::string rol() const override { return "Gerente"; }
    double calcularSalario() const override;
    void descrpcionDetallada(std::ostream& os) const override;

    void asignarMiembro(int empleadoId) { equipoIds_.push_back(empleadoId); }
    const std::vector<int>& equipo() const { return equipoIds_; }
    double bono() const { return bono_; }
    void setBono(double b) { bono_ = b; }

private:
    double bono_;
    std::vector<int> equipoIds_;
};
