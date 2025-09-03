#include "estudiante.h"

Estudiante::Estudiante(const QString &nombre, const QString &dni)
    : Persona(nombre, dni)
{}

void Estudiante::registrarMateria(const QString &materia) {
    if (!m_materias.contains(materia))
        m_materias.append(materia);
}

void Estudiante::registrarCalificacion(const QString &materia, const QString &calificacion) {
    if (!m_materias.contains(materia))
        m_materias.append(materia);
    m_calificaciones[materia] = calificacion;
}

QVector<QString> Estudiante::materiasCursadas() const {
    return m_materias;
}

QString Estudiante::calificacionDe(const QString &materia) const {
    return m_calificaciones.value(materia, QString("N/A"));
}
