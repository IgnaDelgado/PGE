// reserva.cpp
#include "reserva.h"

// Definición del contador
QMap<QString,int> Reserva::s_contadorPorVuelo;

Reserva::Reserva(const QString &nroVuelo,
                 const QString &modelo,
                 const QString &aerolinea,
                 int filas,
                 int columnas,
                 int cantidadPasajeros,
                 const QString &titular)
    : Vuelo(nroVuelo, modelo, aerolinea, filas, columnas)
    , m_cantidadPasajeros(cantidadPasajeros)
    , m_titular(titular)
{
    // Generar el ID: vuelo en minúsculas + contador de 3 dígitos
    QString clave = nroVuelo.toLower();
    int cuenta = ++s_contadorPorVuelo[clave];
    m_idReserva = clave + QString("%1").arg(cuenta, 3, 10, QChar('0'));
}

QString Reserva::getIdReserva() const {
    return m_idReserva;
}

int Reserva::getCantidadPasajeros() const {
    return m_cantidadPasajeros;
}

QString Reserva::getTitular() const {
    return m_titular;
}

QVector<QPair<int,int>> Reserva::getAsientosReservados() const {
    return m_asientosReservados;
}

void Reserva::agregarAsiento(int fila, int columna) {
    if (estaDisponible(fila, columna) &&
        m_asientosReservados.size() < m_cantidadPasajeros)
    {
        reservarAsiento(fila, columna);  // método de Vuelo
        m_asientosReservados.append(qMakePair(fila, columna));
    }
}
