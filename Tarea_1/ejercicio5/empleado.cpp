#include "empleado.h"
#include <iomanip>

void Empleado::imprimir(std::ostream& os) const {
    os << rol() << " #" << id_ << " - " << nombre_ << "\n";
    os << "  Salario base: $" << std::fixed << std::setprecision(2) << salarioBase_ << "\n";
    descrpcionDetallada(os);
    os << "  Salario total: $" << std::fixed << std::setprecision(2) << calcularSalario() << "\n";
}
