#include "../header/SistemaBicicletas.h"
#include <iostream>

using namespace std;

SistemaBicicletas::SistemaBicicletas(){
    tablaEstaciones = new TablaHash();
    arbolBicicletas = new AbbBicicleta();
    grafoCiclovias = new Grafo();
    heapTaller = new MinHeap();

    cout << "Sistema de Bicicletas inicializado correctamente." << endl;
}

SistemaBicicletas::~SistemaBicicletas() {
    delete tablaEstaciones;
    delete arbolBicicletas;
    delete grafoCiclovias;
    delete heapTaller;
    cout << "Memoria del sistema liberada." << endl;
}

void SistemaBicicletas::mostrarInformacionDeEstacion(const string& codigoEstacion){
    Estacion* estacion = tablaEstaciones->buscar(codigoEstacion);

    if(estacion != nullptr){
        estacion->mostrarInformacion();
    }
    else{
        cout << "Error encontrado : La estacion " << codigoEstacion << " no existe en el sistema." << endl;
    }
}

void SistemaBicicletas::agregarEstacion(const Estacion& estacion) {
    bool insertado = tablaEstaciones->insertar(estacion);

    if(insertado){
        cout << "Estacion " << estacion.getCodigo() << " agregada con exito." << endl;
    }
    else{
        cout << "Error encontrado : La estacion " << estacion.getCodigo() << " ya existe." << endl;
    }
}

void SistemaBicicletas::eliminarEstacion(const string& codigoEstacion) {
    bool borrado = tablaEstaciones->eliminar(codigoEstacion);

    if (borrado) {
        cout << "Estacion " << codigoEstacion << " eliminada correctamente." << endl;
    } else {
        cout << "Error: No se pudo eliminar. La estacion '" << codigoEstacion << "' no existe." << endl;
    }
}

ListaViajes SistemaBicicletas::buscarViajesDeUsuario(int dniUsuario) {
    ListaViajes viajesFiltrados;

    NodoViaje* actual = listaGlobalViajes.getCabeza();

    while (actual != nullptr) {
        Viaje viajeActual = actual->getViaje();

        if (viajeActual.getDniUsuario() == dniUsuario) {
            viajesFiltrados.agregarFinal(viajeActual);
        }

        actual = actual->getSiguiente();
    }

    return viajesFiltrados;
}

int SistemaBicicletas::mostrarMinutosPedaleadosDeUsuario(int dniUsuario){
    int totalMinutos = 0;

    ListaViajes viajes = buscarViajesDeUsuario(dniUsuario);

    NodoViaje* actual = viajes.getCabeza();
    while(actual != nullptr){
        totalMinutos += actual->getViaje().getDuracionMin();
        actual = actual->getSiguiente();
    }

    cout << "El usuario con DNI : " << dniUsuario << " pedaleo un total de : " << totalMinutos << " minutos." << endl;

    return totalMinutos;
}

void SistemaBicicletas::registrarViaje(const Viaje& viaje) {
    listaGlobalViajes.agregarFinal(viaje);
}