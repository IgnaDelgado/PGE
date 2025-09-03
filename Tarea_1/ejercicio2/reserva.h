// reserva.h
#ifndef RESERVA_H
#define RESERVA_H

#include "vuelo.h"
#include <QString>
#include <QVector>
#include <QPair>
#include <QMap>

class Reserva : public Vuelo
{
public:
    // Ya no recibimos idReserva: lo generamos internamente
    Reserva(const QString &nroVuelo,
            const QString &modelo,
            const QString &aerolinea,
            int filas,
            int columnas,
            int cantidadPasajeros,
            const QString &titular);

    QString getIdReserva()             const;
    int     getCantidadPasajeros()     const;
    QString getTitular()               const;
    QVector<QPair<int,int>> getAsientosReservados() const;

    void agregarAsiento(int fila, int columna);

private:
    // Contador estático por cada vuelo (clave = nroVuelo.toLower())
    static QMap<QString,int> s_contadorPorVuelo;

    QString m_idReserva;
    int     m_cantidadPasajeros;
    QString m_titular;
    QVector<QPair<int,int>> m_asientosReservados;
};

#endif // RESERVA_H
