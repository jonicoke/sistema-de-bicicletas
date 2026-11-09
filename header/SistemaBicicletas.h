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
    void liberarLista(NodoLista* lista);

public:
    SistemaBicicletas();
    ~SistemaBicicletas();
    SistemaBicicletas(const SistemaBicicletas&) = delete;
    SistemaBicicletas& operator=(const SistemaBicicletas&) = delete;
    
    // A - Estaciones
    void agregarEstacion(const Estacion& estacion);
    void eliminarEstacion(const string& codigoEstacion); // A.3
    void listarEstacionesOrdenadas(bool porCapacidad);   // A.4
    void listarEstacionesConBajaOcupacion();             // A.5
    void mostrarInformacionDeEstacion(const string& codigoEstacion);
    
    // B - Viajes y Taller
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

    // C - Operaciones sobre el ABB de Bicicletas
    void buscarBicicleta(int biciId);           // C.2
    void listarBicicletasOrdenadas();          // C.3
    void mostrarCantidadAveriadas();            // C.4
    void mostrarAlturaABB();                    // C.5

    // Adicionales
    void agregarBicicleta(int id, const std::string& codigoEstacion, TipoBici tipo, EstadoBici estado, int kilometros, bool esManual = false); // adicionales
    void agregarCiclovia(const std::string& codigoOrigen, const std::string& codigoDestino); // adicionales
    void calcularRecorridoPorCiclovia(const string& codigoOrigen, const string& codigoDestino);

    // Requisito estructural de A (para el README)
    void mostrarEstadisticasTabla();
};

#endif

