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
    Estacion* estacionOrigen;
    Estacion* estacionDestino;
    int fecha;
    int duracionMin;

public:
    Viaje();
    Viaje(int viajeId, int biciId, int dniUsuario,
          Estacion* estacionOrigen, Estacion* estacionDestino,
          int fecha, int duracionMin);

    ~Viaje();

    void mostrarInformacion();

    int getViajeId() const;
    int getBiciId() const;
    int getDniUsuario() const;
    Estacion* getEstacionOrigen() const;
    Estacion* getEstacionDestino() const;
    int getFecha() const;
    int getDuracionMin() const;
};

#endif

