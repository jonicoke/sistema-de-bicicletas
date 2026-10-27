#ifndef SISTEMA_BICICLETAS_H
#define SISTEMA_BICICLETAS_H

#include <cstddef>
#include <string>
#include "Grafo.h"
#include "AbbBicicleta.h"
#include "MinHeap.h"
#include "TablaHash.h"
#include "ListaViajes.h"
#include "Bicicleta.h"

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
    ~SistemaBicicletas();
    SistemaBicicletas(const SistemaBicicletas&) = delete;
    SistemaBicicletas& operator=(const SistemaBicicletas&) = delete;
    
    // A
    void agregarEstacion(const Estacion& estacion);
    void eliminarEstacion(const string& codigoEstacion);
    void listarEstacionesOrdenadas(bool porCapacidad); // A.4
    void listarEstacionesConBajaOcupacion(); // A.5
    void mostrarInformacionDeEstacion(const string& codigoEstacion);
    
    // B
    void registrarViaje(const Viaje& viaje);
    ListaViajes buscarViajesDeUsuario(int dniUsuario);
    int contarViajesDesdeEstacion(const string& codigoEstacion, int fechaDesde, int fechaHasta); // B.1
    void mostrarViajesDeUsuario(int dniUsuario);            // B.2
    int mostrarMinutosPedaleadosDeUsuario(int dniUsuario);  // B.2
    void listarBicicletasConXKm(int kilometros);            // B.3
    void retirarBicicleta(const string& codigoEstacion, int dniUsuario);                        // B.4
    void devolverBicicleta(const string& codigoEstacion);  
    void enviarBicicletaATaller(int biciId); //B.5
    void atenderTaller();                   // B.5
    
    void agregarBicicleta(int id, const std::string& codigoEstacion, TipoBici tipo, EstadoBici estado, int kilometros, bool esManual = false); // adicionales
    void agregarCiclovia(const std::string& codigoOrigen, const std::string& codigoDestino); // adicionales
    void calcularRecorridoPorCiclovia(const string& codigoOrigen, const string& codigoDestino);

    // Requisito estructural de A (para el README)
    void mostrarEstadisticasTabla();
};

#endif

