// vuelo.h
#ifndef VUELO_H
#define VUELO_H

#include <QString>
#include <QVector>

class Vuelo
{
public:
    Vuelo(const QString &nroVuelo,
          const QString &modelo,
          const QString &aerolinea,
          int filas,
          int columnas);

    QString getNroVuelo() const;
    QString getModelo()    const;
    QString getAerolinea() const;

    bool estaDisponible(int fila, int columna) const;
    void reservarAsiento(int fila, int columna);
    void liberarAsiento(int fila, int columna);

    QVector<QVector<bool>> getMatrizAsientos() const;
    void imprimirAsientos() const;

protected:
    QString m_nroVuelo;
    QString m_modelo;
    QString m_aerolinea;
    int     m_filas;
    int     m_columnas;
    QVector<QVector<bool>> m_asientos;  // true = libre, false = ocupado
};

#endif // VUELO_H
