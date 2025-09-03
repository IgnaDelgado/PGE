#include <iostream>
#include <memory>
#include "empresa.h"
#include "gerente.h"
#include "desarrollador.h"
#include "tester.h"
#include "vendedor.h"

int main() {
    Empresa emp;

    auto g = std::make_unique<Gerente>("Laura Gómez", 100, 900000.0, 150000.0);
    g->asignarMiembro(200);
    g->asignarMiembro(201);

    auto d1 = std::make_unique<Desarrollador>("Iñaki Merino", 200, 600000.0, Seniority::Sr, 10, 5000.0);
    auto d2 = std::make_unique<Desarrollador>("Ana Ruiz", 201, 420000.0, Seniority::Ssr, 0, 0.0);
    auto t1 = std::make_unique<Tester>("Pedro Test", 300, 350000.0, 18, 2000.0);
    auto v1 = std::make_unique<Vendedor>("Sofía Sales", 400, 300000.0, 0.05, 2500000.0);

    emp.agregar(std::move(g));
    emp.agregar(std::move(d1));
    emp.agregar(std::move(d2));
    emp.agregar(std::move(t1));
    emp.agregar(std::move(v1));

    emp.listar(std::cout);

    // Polimorfismo en acción: modificar algo y recalcular
    if (auto* d = dynamic_cast<Desarrollador*>(emp.buscar(201))) {
        d->setHorasExtra(8);
        d->setTarifaExtra(4000.0);
    }

    std::cout << "\n--- Después de actualizar extras de Ana ---\n";
    emp.listar(std::cout);

    // Eliminar un empleado
    emp.eliminarPorId(300);

    std::cout << "\n--- Después de eliminar Tester 300 ---\n";
    emp.listar(std::cout);

    return 0;
}
