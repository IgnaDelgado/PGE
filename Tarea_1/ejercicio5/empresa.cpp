#include "empresa.h"
#include <algorithm>
#include <numeric>
#include <iomanip>

bool Empresa::agregar(std::unique_ptr<Empleado> e) {
    if (!e) return false;
    int id = e->id();
    auto it = std::find_if(empleados_.begin(), empleados_.end(),
        [id](const std::unique_ptr<Empleado>& x){ return x->id() == id; });
    if (it != empleados_.end()) return false; // ID duplicado
    empleados_.push_back(std::move(e));
    return true;
}

bool Empresa::eliminarPorId(int id) {
    auto it = std::remove_if(empleados_.begin(), empleados_.end(),
        [id](const std::unique_ptr<Empleado>& x){ return x->id() == id; });
    if (it == empleados_.end()) return false;
    empleados_.erase(it, empleados_.end());
    return true;
}

Empleado* Empresa::buscar(int id) {
    for (auto& e : empleados_) if (e->id() == id) return e.get();
    return nullptr;
}
const Empleado* Empresa::buscar(int id) const {
    for (auto& e : empleados_) if (e->id() == id) return e.get();
    return nullptr;
}

double Empresa::nominaTotal() const {
    double total = 0.0;
    for (auto& e : empleados_) total += e->calcularSalario();
    return total;
}

void Empresa::listar(std::ostream& os) const {
    os << "=== Empleados (" << empleados_.size() << ") ===\n";
    for (auto& e : empleados_) {
        os << *e << "\n";
    }
    os << "Nómina total: $" << std::fixed << std::setprecision(2) << nominaTotal() << "\n";
}
