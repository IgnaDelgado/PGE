// main.cpp

#include <QCoreApplication>
#include <QTextStream>
#include <QString>
#include <QVector>
#include <QPair>
#include <QMap>
#include "vuelo.h"
#include "reserva.h"
#include "pasajero.h"

static void printAsientos(const Vuelo &v) {
    QTextStream out(stdout);
    auto mat = v.getMatrizAsientos();
    int filas = mat.size();
    int cols = filas ? mat[0].size() : 0;

    // Encabezado columnas
    out << "    ";
    for (int j = 0; j < cols; ++j) {
        out << QString::number(j+1).rightJustified(3, ' ');
        if ((j+1)%3 == 0) out << "  ";
    }
    out << "\n";

    // Filas con números y asientos
    for (int i = 0; i < filas; ++i) {
        out << QString::number(i+1).rightJustified(3, ' ') << " ";
        for (int j = 0; j < cols; ++j) {
            out << (mat[i][j] ? "[ ]" : "[*]");
            if ((j+1)%3 == 0) out << "  ";
        }
        out << "\n";
    }
    out << "\n";
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QTextStream in(stdin), out(stdout);

    // --- Datos iniciales ---
    QVector<Vuelo> vuelos;
    QVector<Reserva> reservas;
    QMap<QString, QVector<Pasajero>> pasajerosPorReserva;

    // Ejemplo de vuelo
    vuelos.append(Vuelo("AR123", "Boeing 737", "AR", 25, 10));

    while (true) {
        out << "\n=== Menú Aeropuerto ===\n"
            << "1. Ver disponibilidad de asientos\n"
            << "2. Gestionar reserva\n"
            << "3. Crear reserva\n"
            << "0. Salir\n"
            << "Seleccione una opción: ";
        out.flush();

        int opc;
        in >> opc;

        if (opc == 0) break;

        if (opc == 1) {
            out << "Ingrese nro de vuelo: "; out.flush();
            QString nro; in >> nro;
            bool found = false;
            for (auto &v : vuelos) {
                if (v.getNroVuelo() == nro) {
                    printAsientos(v);
                    found = true;
                    break;
                }
            }
            if (!found) out << "Vuelo no encontrado.\n";
        }
        else if (opc == 2) {
            out << "Gestionar reserva (ID o titular): "; out.flush();
            QString key; in >> key;

            int idxRes = -1;
            for (int i = 0; i < reservas.size(); ++i) {
                if (reservas[i].getIdReserva() == key || reservas[i].getTitular() == key) {
                    idxRes = i; break;
                }
            }
            if (idxRes < 0) {
                out << "Reserva no encontrada.\n";
                continue;
            }
            Reserva &res = reservas[idxRes];
            auto &plist = pasajerosPorReserva[res.getIdReserva()];

            out << "\n--- Gestión Reserva [" << res.getIdReserva() << "] ---\n"
                << "1. Cambiar titular\n"
                << "2. Cambiar pasajero\n"
                << "3. Dar de baja pasajero\n"
                << "4. Dar de baja reserva\n"
                << "0. Volver\n"
                << "Opción: "; out.flush();
            int sub; in >> sub;

            if (sub == 1) {
                out << "Nuevo titular: "; out.flush();
                in.skipWhiteSpace();
                QString nuevo = in.readLine();
                res = Reserva(res.getNroVuelo(), res.getModelo(), res.getAerolinea(),
                              res.getMatrizAsientos().size(), res.getMatrizAsientos()[0].size(),
                              res.getCantidadPasajeros(), nuevo);
                out << "Titular actualizado.\n";
            }
            else if (sub == 2) {
                out << "Pasajero a reemplazar (nombre completo): "; out.flush();
                in.skipWhiteSpace(); QString oldName = in.readLine();
                int pIdx = -1;
                for (int i = 0; i < plist.size(); ++i) {
                    if ((plist[i].getNombre() + " " + plist[i].getApellido()) == oldName) {
                        pIdx = i; break;
                    }
                }
                if (pIdx < 0) { out << "Pasajero no encontrado.\n"; }
                else {
                    out << "Nuevo nombre completo: "; out.flush();
                    in.skipWhiteSpace(); QString full = in.readLine();
                    auto parts = full.split(' ');
                    auto oldSeat = plist[pIdx].getAsiento();
                    // liberar antiguo asiento
                    for (auto &v : vuelos) if (v.getNroVuelo() == res.getNroVuelo()) {
                            v.liberarAsiento(oldSeat.first, oldSeat.second); break;
                        }
                    // reservar nuevo asiento
                    int r,c;
                    while (true) {
                        out << "Asiento nuevo (fila columna): "; out.flush();
                        in >> r >> c;
                        for (auto &v : vuelos) if (v.getNroVuelo() == res.getNroVuelo()) {
                                if (v.estaDisponible(r,c)) { v.reservarAsiento(r,c); break; }
                                else { out << "Asiento no disponible.\n"; }
                            }
                        break;
                    }
                    // reemplazar pasajero con nueva firma correcta
                    plist[pIdx] = Pasajero(res.getNroVuelo(), res.getModelo(), res.getAerolinea(),
                                           res.getMatrizAsientos().size(), res.getMatrizAsientos()[0].size(),
                                           res.getCantidadPasajeros(), res.getTitular(),
                                           parts.value(0), parts.value(1), plist[pIdx].esTitularReserva(), r, c);
                    out << "Pasajero reemplazado.\n";
                }
            }
            else if (sub == 3) {
                if (plist.size() <= 1) { out << "Debe cancelar la reserva.\n"; }
                else {
                    out << "Pasajero a dar de baja: "; out.flush();
                    in.skipWhiteSpace(); QString rem = in.readLine();
                    int pIdx = -1;
                    for (int i = 0; i < plist.size(); ++i) {
                        if ((plist[i].getNombre() + " " + plist[i].getApellido()) == rem) {
                            pIdx = i; break;
                        }
                    }
                    if (pIdx < 0) { out << "Pasajero no encontrado.\n"; }
                    else {
                        auto seat = plist[pIdx].getAsiento();
                        for (auto &v : vuelos)
                            if (v.getNroVuelo() == res.getNroVuelo())
                                v.liberarAsiento(seat.first, seat.second);
                        plist.remove(pIdx);
                        out << "Pasajero dado de baja.\n";
                    }
                }
            }
            else if (sub == 4) {
                for (auto &p : plist) {
                    auto s = p.getAsiento();
                    for (auto &v : vuelos)
                        if (v.getNroVuelo() == res.getNroVuelo())
                            v.liberarAsiento(s.first, s.second);
                }
                pasajerosPorReserva.remove(res.getIdReserva());
                reservas.removeAt(idxRes);
                out << "Reserva cancelada.\n";
            }
        }
        else if (opc == 3) {
            out << "Vuelo: "; out.flush(); QString nro; in >> nro;
            int vidx = -1;
            for (int i = 0; i < vuelos.size(); ++i) if (vuelos[i].getNroVuelo() == nro) { vidx = i; break; }
            if (vidx < 0) { out << "Vuelo no existe.\n"; continue; }
            Vuelo &v = vuelos[vidx];
            printAsientos(v);

            out << "Cantidad de pasajeros: "; out.flush(); int cp; in >> cp;
            int libres = 0;
            for (auto &row : v.getMatrizAsientos()) for (bool f : row) if (f) ++libres;
            if (libres < cp) { out << "No hay suficientes asientos.\n"; continue; }

            out << "Titular (nombre apellido): "; out.flush();
            in.skipWhiteSpace(); QString titular = in.readLine();

            // Crear reserva sin pedir ID
            Reserva nueva(v.getNroVuelo(), v.getModelo(), v.getAerolinea(),
                          v.getMatrizAsientos().size(), v.getMatrizAsientos()[0].size(),
                          cp, titular);
            QString idRes = nueva.getIdReserva();
            QVector<Pasajero> lst;

            for (int i = 0; i < cp; ++i) {
                QString nom, ape;
                bool esTit = (i == 0);
                if (i == 0) {
                    auto p = titular.split(' ');
                    nom = p.value(0); ape = p.value(1);
                } else {
                    out << "Pasajero " << (i+1) << " (nombre apellido): "; out.flush();
                    in.skipWhiteSpace(); QString full = in.readLine();
                    auto p = full.split(' ');
                    nom = p.value(0); ape = p.value(1);
                }
                int r,c;
                while (true) {
                    out << "Asiento (fila columna): "; out.flush();
                    in >> r >> c;
                    if (v.estaDisponible(r,c)) { v.reservarAsiento(r,c); nueva.agregarAsiento(r,c); break; }
                    out << "No disponible.\n";
                }
                lst.append(Pasajero(v.getNroVuelo(), v.getModelo(), v.getAerolinea(),
                                    v.getMatrizAsientos().size(), v.getMatrizAsientos()[0].size(),
                                    cp, titular, nom, ape, esTit, r, c));
            }
            reservas.append(nueva);
            pasajerosPorReserva[idRes] = lst;
            out << "Reserva " << idRes << " creada.\n";
        }
        else {
            out << "Opcion inválida.\n";
        }
    }

    return 0;
}
