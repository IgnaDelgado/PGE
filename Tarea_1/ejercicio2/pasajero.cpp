// pasajero.cpp
#include "pasajero.h"

Pasajero::Pasajero(const QString &nroVuelo,
                   const QString &modelo,
                   const QString &aerolinea,
                   int filas,
                   int columnas,
                   int cantidadPasajeros,
                   const QString &titular,
                   const QString &nombre,
                   const QString &apellido,
                   bool esTitular,
                   int fila,
                   int columna)
    : Reserva(nroVuelo,
              modelo,
              aerolinea,
              filas,
              columnas,
              cantidadPasajeros,
              titular)
    , m_nombre(nombre)
    , m_apellido(apellido)
    , m_esTitular(esTitular)
    , m_asiento(qMakePair(fila, columna))
{
    // Reserva ya generó su ID internamente y marcó el asiento ocupado
    agregarAsiento(fila, columna);
}

QString Pasajero::getNombre() const {
    return m_nombre;
}

QString Pasajero::getApellido() const {
    return m_apellido;
}

bool Pasajero::esTitularReserva() const {
    return m_esTitular;
}

QPair<int,int> Pasajero::getAsiento() const {
    return m_asiento;
}
