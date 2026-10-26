#ifndef SISTEMA_BICICLETAS_H
#define SISTEMA_BICICLETAS_H

#include <cstddef>
#include <string>
#include "Grafo.h"
#include "AbbBicicleta.h"
#include "MinHeap.h"
#include "TablaHash.h"
#include "ListaViajes.h"

using namespace std;

class SistemaBicicletas
{
private:
    TablaHash* tablaEstaciones;
    Grafo* grafoCiclovias;
    MinHeap* heapTaller;
    ListaViajes listaGlobalViajes;
    AbbBicicleta* abbBicicletas;

    void ordenarPorSeleccion(Estacion** arr, int n, bool porCapacidad);

public:
    SistemaBicicletas();
    // Destructor para liberar puntero con delete
    ~SistemaBicicletas();
    //SistemaBicicletas(const TablaHash &tablaEstaciones, const AbbBicicleta &arbolBicicletas, const Grafo &grafoCiclovias, MinHeap* heapTaller);

    
    void agregarEstacion(const Estacion& estacion);
    void eliminarEstacion(const string& codigoEstacion);
    void listarEstacionesOrdenadas(bool porCapacidad); // A.4
    void listarEstacionesConBajaOcupacion(); // A.5
    void mostrarInformacionDeEstacion(const string& codigoEstacion);
    
    void registrarViaje(const Viaje& viaje);
    ListaViajes buscarViajesDeUsuario(int dniUsuario);
    int contarViajesDesdeEstacion(const string& codigoEstacion, int fechaDesde, int fechaHasta); // B.1
    int mostrarMinutosPedaleadosDeUsuario(int dniUsuario);
    
    void listarBicicletasConXKm(int kilometros);
    void retirarBicicleta(const string& codigoEstacion, int dniUsuario);                        // B.4
    void devolverBicicleta(const string& codigoEstacion);  
    // NodoLista* listarBicicletasConXKm(int kilometros);
};

#endif

