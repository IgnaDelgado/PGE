#pragma once
#include <vector>
#include <memory>
#include <optional>
#include "empleado.h"

class Empresa {
public:
    bool agregar(std::unique_ptr<Empleado> e);
    bool eliminarPorId(int id);
    Empleado* buscar(int id);
    const Empleado* buscar(int id) const;

    double nominaTotal() const;
    void listar(std::ostream& os) const;

    const std::vector<std::unique_ptr<Empleado>>& empleados() const { return empleados_; }

private:
    std::vector<std::unique_ptr<Empleado>> empleados_;
};
