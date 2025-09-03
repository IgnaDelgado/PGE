// vuelo.cpp
#include "vuelo.h"
#include <QTextStream>

Vuelo::Vuelo(const QString &nroVuelo,
             const QString &modelo,
             const QString &aerolinea,
             int filas,
             int columnas)
    : m_nroVuelo(nroVuelo)
    , m_modelo(modelo)
    , m_aerolinea(aerolinea)
    , m_filas(filas)
    , m_columnas(columnas)
{
    // inicializa todos los asientos como libres
    m_asientos = QVector<QVector<bool>>(filas, QVector<bool>(columnas, true));
}

QString Vuelo::getNroVuelo()    const { return m_nroVuelo; }
QString Vuelo::getModelo()      const { return m_modelo; }
QString Vuelo::getAerolinea()   const { return m_aerolinea; }

bool Vuelo::estaDisponible(int fila, int columna) const {
    if (fila < 0 || fila >= m_filas || columna < 0 || columna >= m_columnas)
        return false;
    return m_asientos[fila][columna];
}

void Vuelo::reservarAsiento(int fila, int columna) {
    if (estaDisponible(fila, columna))
        m_asientos[fila][columna] = false;
}

void Vuelo::liberarAsiento(int fila, int columna) {
    if (fila >= 0 && fila < m_filas && columna >= 0 && columna < m_columnas)
        m_asientos[fila][columna] = true;
}

QVector<QVector<bool>> Vuelo::getMatrizAsientos() const {
    return m_asientos;
}

void Vuelo::imprimirAsientos() const {
    QTextStream out(stdout);
    out << "Mapa de asientos (O = libre, X = ocupado):\n";
    for (int i = 0; i < m_filas; ++i) {
        for (int j = 0; j < m_columnas; ++j) {
            out << (m_asientos[i][j] ? "O " : "X ");
        }
        out << "\n";
    }
    out << "\n";
}
