#include "desarrollador.h"
#include <ostream>

Desarrollador::Desarrollador(std::string nombre, int id, double salarioBase,
                             Seniority nivel, int horasExtra, double tarifaExtra)
    : Empleado(std::move(nombre), id, salarioBase),
      nivel_(nivel), horasExtra_(horasExtra), tarifaExtra_(tarifaExtra) {}

double Desarrollador::multiplicadorPorSeniority() const {
    switch (nivel_) {
        case Seniority::Jr:  return 1.00;
        case Seniority::Ssr: return 1.15;
        case Seniority::Sr:  return 1.35;
    }
    return 1.0;
}

double Desarrollador::calcularSalario() const {
    return salarioBase_ * multiplicadorPorSeniority() + horasExtra_ * tarifaExtra_;
}

const char* Desarrollador::nivelStr(Seniority s) {
    switch (s) {
        case Seniority::Jr:  return "Jr";
        case Seniority::Ssr: return "Ssr";
        case Seniority::Sr:  return "Sr";
    }
    return "N/A";
}

void Desarrollador::descrpcionDetallada(std::ostream& os) const {
    os << "  Seniority: " << nivelStr(nivel_) << "\n";
    os << "  Horas extra: " << horasExtra_ << " @ $" << tarifaExtra_ << "\n";
}
