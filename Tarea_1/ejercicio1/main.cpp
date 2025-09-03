#include <QCoreApplication>
#include <QTextStream>
#include "estudiante.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QTextStream out(stdout);

    Estudiante alumno("Juan Perez", "30.123.456");
    alumno.registrarMateria("Matematica");
    alumno.registrarCalificacion("Matematica", "8.5");
    alumno.registrarMateria("Fisica");
    alumno.registrarCalificacion("Fisica", "7.0");

    out << "Estudiante: " << alumno.getNombre()
        << " (DNI: " << alumno.getDni() << ")\n\n";

    out << "Materias y calificaciones:\n";
    for (const QString &mat : alumno.materiasCursadas()) {
        out << " - " << mat
            << ": " << alumno.calificacionDe(mat) << "\n";
    }

    return 0;
}
