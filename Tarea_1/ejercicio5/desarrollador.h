#pragma once
#include "empleado.h"
#include <string>

enum class Seniority { Jr, Ssr, Sr };

class Desarrollador : public Empleado {
public:
    Desarrollador(std::string nombre, int id, double salarioBase,
                  Seniority nivel, int horasExtra = 0, double tarifaExtra = 0.0);

    std::string rol() const override { return "Desarrollador"; }
    double calcularSalario() const override;
    void descrpcionDetallada(std::ostream& os) const override;

    Seniority nivel() const { return nivel_; }
    void setNivel(Seniority s) { nivel_ = s; }
    int horasExtra() const { return horasExtra_; }
    void setHorasExtra(int h) { horasExtra_ = h; }
    double tarifaExtra() const { return tarifaExtra_; }
    void setTarifaExtra(double t) { tarifaExtra_ = t; }

private:
    Seniority nivel_;
    int horasExtra_;
    double tarifaExtra_;
    double multiplicadorPorSeniority() const;
    static const char* nivelStr(Seniority s);
};
