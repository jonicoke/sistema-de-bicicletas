#ifndef VIAJE_H
#define VIAJE_H

#include <cstddef>
#include <string>
#include "Estacion.h"


using namespace std;

class Viaje
{
private:
    int viajeId;
    int biciId;
    int dniUsuario;
    std::string estacionOrigen;
    std::string estacionDestino;
    int fecha;
    int duracionMin;

public:
    Viaje();
    Viaje(int viajeId, int biciId, int dniUsuario,
          const std::string& estacionOrigen, const std::string&  estacionDestino,
          int fecha, int duracionMin);

    ~Viaje();

    void mostrarInformacion();

    int getViajeId() const;
    int getBiciId() const;
    int getDniUsuario() const;
    const std::string&  getEstacionOrigen() const;
    const std::string&  getEstacionDestino() const;
    int getFecha() const;
    int getDuracionMin() const;
    bool estaEnRango(int fechaDesde, int fechaHasta) const;
};


#endif

