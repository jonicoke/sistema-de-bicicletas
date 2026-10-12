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
    Estacion();
    Estacion(const string &codigo, const string &nombre, const string &barrio, int capacidadAnclajes);

    string getCodigo() const;
    const string &getNombre() const;
    const string &getBarrio() const;
    int getCapacidadAnclajes() const;
    int getBicisDisponibles() const;
    const ColaEspera &getColaEspera() const;

    // Mostrar la información de una estación dado su código. 
    void mostrarInformacion();
    
    // Listar las bicicletas que tengan más de X km recorridos.
    // Este método puede estar a nivel global 
    // void listarBicicletasConXKm(int km);

    // Gestionar la cola de espera de una estación.
    string gestionarColaDeEspera();

};

#endif

