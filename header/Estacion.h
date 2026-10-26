#ifndef ESTACION_H
#define ESTACION_H

#include <cstddef>
#include <string>
#include "ColaEspera.h"

using namespace std;

class Estacion
{
private:
    string codigo;
    string nombre;
    string barrio;
    int capacidadAnclajes;
    int bicisDisponibles;
    ColaEspera colaEspera;

public:
    Estacion(const string &codigo, const string &nombre, const string &barrio, int capacidadAnclajes, int bicisDisponibles);

    string getCodigo() const;
    const string &getNombre() const;
    const string &getBarrio() const;
    int getCapacidadAnclajes() const;
    int getBicisDisponibles() const;
    void setBicisDisponibles(int num);
    const ColaEspera &getColaEspera() const;

    // Mostrar la información de una estación dado su código. 
    void mostrarInformacion();
    

    bool tieneBajaOcupacion() const;
    // Gestionar la cola de espera de una estación.
    // Retirar una bicicleta de una estación. Si no hay bicicletas disponibles, el usuario se agrega a la cola de espera.
    bool retirarBicicleta(int dniUsuario);
    // Devolver una bicicleta a una estación. Si hay usuarios en la cola de espera, se le da la bicicleta al primero de la cola. Si no hay usuarios en la cola, se incrementa el contador de bicicletas disponibles.
    bool devolverBicicleta(int& dniAsignado);
};

#endif

