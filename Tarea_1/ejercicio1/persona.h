#ifndef PERSONA_H
#define PERSONA_H

#include <QString>

class Persona
{
public:
    Persona(const QString &nombre, const QString &dni);
    QString getNombre() const;
    QString getDni() const;

protected:
    QString m_nombre;
    QString m_dni;
};

#endif
