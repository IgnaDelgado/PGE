// estudiante.h
#ifndef ESTUDIANTE_H
#define ESTUDIANTE_H

#include "persona.h"
#include <QVector>
#include <QMap>

class Estudiante : public Persona
{
public:
    Estudiante(const QString &nombre, const QString &dni);

    // Registrar
    void registrarMateria(const QString &materia);
    void registrarCalificacion(const QString &materia, const QString &calificacion);

    // Consultar
    QVector<QString> materiasCursadas() const;
    QString calificacionDe(const QString &materia) const;

private:
    QVector<QString> m_materias;
    QMap<QString, QString> m_calificaciones;
};

#endif
