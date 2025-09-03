#include "gerente.h"
#include <ostream>

Gerente::Gerente(std::string nombre, int id, double salarioBase, double bono)
    : Empleado(std::move(nombre), id, salarioBase), bono_(bono) {}

double Gerente::calcularSalario() const {
    // Sencillo: base + bono fijo (podrías agregar KPIs)
    return salarioBase_ + bono_;
}

void Gerente::descrpcionDetallada(std::ostream& os) const {
    os << "  Bono fijo: $" << bono_ << "\n";
    os << "  Equipo a cargo: " << equipoIds_.size() << " personas\n";
}
