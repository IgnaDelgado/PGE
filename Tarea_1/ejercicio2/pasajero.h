// pasajero.h
#ifndef PASAJERO_H
#define PASAJERO_H

#include "reserva.h"
#include <QString>
#include <QPair>

class Pasajero : public Reserva
{
public:
    Pasajero(const QString &nroVuelo,
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
             int columna);

    QString getNombre()           const;
    QString getApellido()         const;
    bool    esTitularReserva()    const;
    QPair<int,int> getAsiento()   const;

private:
    QString      m_nombre;
    QString      m_apellido;
    bool         m_esTitular;
    QPair<int,int> m_asiento;
};

#endif // PASAJERO_H
