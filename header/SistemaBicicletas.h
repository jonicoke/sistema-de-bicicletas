#ifndef SISTEMA_BICICLETAS_H
#define SISTEMA_BICICLETAS_H

#include <cstddef>
#include <string>
#include "Grafo.h"
#include "AbbBicicleta.h"
#include "MinHeap.h"
#include "TablaHash.h"

using namespace std;

class SistemaBicicletas
{
private:
    TablaHash* tablaEstaciones;
    AbbBicicleta* arbolBicicletas;
    Grafo* grafoCiclovias;
    MinHeap* heapTaller;

public:
    SistemaBicicletas();
    // Destructor para liberar puntero con delete
    ~SistemaBicicletas();
    SistemaBicicletas(const TablaHash &tablaEstaciones, const AbbBicicleta &arbolBicicletas, const Grafo &grafoCiclovias, MinHeap heapTaller);

    void mostrarInformacionDeEstacion(const string& codigoEstacion);
    void agregarEstacion(const Estacion& estacion);
    void eliminarEstacion(const string& codigoEstacion);
};

#endif

