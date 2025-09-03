#include "persona.h"

Persona::Persona(const QString &nombre, const QString &dni)
    : m_nombre(nombre), m_dni(dni)
{}

QString Persona::getNombre() const { return m_nombre; }
QString Persona::getDni()    const { return m_dni;    }
