#include "../header/SistemaBicicletas.h"
#include <iostream>

using namespace std;

SistemaBicicletas::SistemaBicicletas(){
    tablaEstaciones = new TablaHash(37);
    abbBicicletas = new AbbBicicleta();
    grafoCiclovias = new Grafo();
    heapTaller = new MinHeap();

    cout << "Sistema de Bicicletas inicializado correctamente." << endl;
}

SistemaBicicletas::~SistemaBicicletas() {
    delete tablaEstaciones;
    delete abbBicicletas;
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
        //Si fue insertado lo agrego como vértice a GrafoCiclovias
        grafoCiclovias->agregarVertice(estacion.getCodigo());
        cout << "Estacion " << estacion.getCodigo() << " agregada con exito." << endl;
    }
    else{
        cout << "Error encontrado : La estacion " << estacion.getCodigo() << " ya existe." << endl;
    }
}

void SistemaBicicletas::eliminarEstacion(const string& codigoEstacion) {
    bool borrado = tablaEstaciones->eliminar(codigoEstacion);

    if (borrado) {
        grafoCiclovias->eliminarVertice(codigoEstacion);
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
    // Recorremos la lista global para validar que no exista el idViaje
    NodoViaje* actual = listaGlobalViajes.getCabeza();
    while (actual != nullptr) {
        if (actual->getViaje().getViajeId() == viaje.getViajeId()) {
            cout << "Error: El viaje con ID " << viaje.getViajeId() << " ya esta registrado." << endl;
            return;
        }
        actual = actual->getSiguiente();
    }

    // Si no existía previamente, lo agregamos al final
    listaGlobalViajes.agregarFinal(viaje);
    cout << "Viaje ID " << viaje.getViajeId() << " registrado con exito." << endl;
}

// 13/10
// Función auxiliar para obtener el valor de comparación según el criterio
static int valorCriterio(const Estacion* e, bool porCapacidad) {
    return porCapacidad ? e->getCapacidadAnclajes() : e->getBicisDisponibles();
}
// A.4 ordenamiento por seleccion, de menor a mayor
void SistemaBicicletas::ordenarPorSeleccion(Estacion** arr, int n, bool porCapacidad) {
    for (int i = 0; i < n - 1; i++) {
        int menor = i;
        for (int j = i + 1; j < n; j++) {
            if (valorCriterio(arr[j], porCapacidad) < valorCriterio(arr[menor], porCapacidad)) {
                menor = j;
            }
        }
        Estacion* temp = arr[i];
        arr[i] = arr[menor];
        arr[menor] = temp;
    }
}
void SistemaBicicletas::listarEstacionesOrdenadas(bool porCapacidad) {
    int n = 0;
    Estacion** arr = tablaEstaciones->obtenerEstaciones(n);
    if (n == 0) {
        cout << "No hay estaciones cargadas." << endl;
        return;
    }
    ordenarPorSeleccion(arr, n, porCapacidad);

    cout << "Estaciones ordenadas por "
         << (porCapacidad ? "capacidad de anclajes" : "bicicletas disponibles") << ":" << endl;
    for (int i = 0; i < n; i++) {
        arr[i]->mostrarInformacion();
        cout << "---" << endl;
    }
    delete[] arr;   // se libera el arreglo, no las estaciones
}
// A5: estaciones con menos del 20% de ocupacion
void SistemaBicicletas::listarEstacionesConBajaOcupacion() {
    int n = 0;
    Estacion** arr = tablaEstaciones->obtenerEstaciones(n);
    int encontradas = 0;

    for (int i = 0; i < n; i++) {
        if (arr[i]->tieneBajaOcupacion()) {
            arr[i]->mostrarInformacion();
            cout << "---" << endl;
            encontradas++;
        }
    }
    if (encontradas == 0) {
        cout << "No hay estaciones con menos del 20% de ocupacion." << endl;
    }
    delete[] arr;   // delete[] de nullptr es seguro si n == 0
}

// B4: retirar una bicicleta; si no hay, el usuario queda en la cola de espera
void SistemaBicicletas::retirarBicicleta(const string& codigoEstacion, int dniUsuario) {
    Estacion* estacion = tablaEstaciones->buscar(codigoEstacion);
    if (estacion == nullptr) {
        cout << "Error: la estacion " << codigoEstacion << " no existe." << endl;
        return;
    }
    if (estacion->retirarBicicleta(dniUsuario)) {
        cout << "El usuario " << dniUsuario << " retiro una bicicleta." << endl;
    } else {
        cout << "No hay bicicletas disponibles. El usuario " << dniUsuario
             << " quedo en la cola de espera." << endl;
    }
}

// B4: devolver una bicicleta; si hay cola, se la asigna al primero
void SistemaBicicletas::devolverBicicleta(const string& codigoEstacion) {
    Estacion* estacion = tablaEstaciones->buscar(codigoEstacion);
    if (estacion == nullptr) {
        cout << "Error: la estacion " << codigoEstacion << " no existe." << endl;
        return;
    }
    int dniAsignado = 0;
    if (!estacion->devolverBicicleta(dniAsignado)) {
        cout << "La estacion esta llena: no hay anclajes libres." << endl;
    } else if (dniAsignado != 0) {
        cout << "Bicicleta asignada al usuario " << dniAsignado << " (primero de la cola)." << endl;
    } else {
        cout << "Bicicleta devuelta. Nadie esperaba." << endl;
    }
}
// B1
int SistemaBicicletas::contarViajesDesdeEstacion(const string& codigo, int desde, int hasta) {
    if (desde > hasta) { cout << "Rango de fechas invalido." << endl; return 0; }
    int cantidad = 0;
    for (NodoViaje* a = listaGlobalViajes.getCabeza(); a != nullptr; a = a->getSiguiente()) {
        Viaje v = a->getViaje();
        if (v.getEstacionOrigen() == codigo && v.estaEnRango(desde, hasta)) cantidad++;
    }
    cout << "Viajes desde " << codigo << " entre " << desde << " y " << hasta << ": " << cantidad << endl;
    return cantidad;
}

// NodoLista* SistemaBicicletas::listarBicicletasConXKm(int kilometros) {
//     return abbBicicletas->listarConMasDeKm(kilometros);
// }
// B3: mostrar y liberar
void SistemaBicicletas::listarBicicletasConXKm(int km) {
    NodoLista* lista = abbBicicletas->listarConMasDeKm(km);
    if (lista == nullptr) { cout << "No hay bicicletas con mas de " << km << " km." << endl; return; }
    for (NodoLista* a = lista; a != nullptr; a = a->sig) a->dato->mostrar();
    liberarLista(lista);
}
// B5: insertar una bicicleta en el taller
void SistemaBicicletas::enviarBicicletaATaller(int biciId) {
    Bicicleta* bici = abbBicicletas->buscar(biciId);   // el arbol informa si no existe
    if (bici == nullptr) return;

    if (heapTaller->insertar(*bici)) {
        cout << "Bicicleta " << biciId << " enviada al taller con prioridad "
             << MinHeap::calcularPrioridad(*bici) << "." << endl;
    } else {
        cout << "La bicicleta " << biciId << " no esta averiada: no entra al taller." << endl;
    }
}

// B5: extraer la bicicleta mas urgente
void SistemaBicicletas::atenderTaller() {
    if (heapTaller->estaVacia()) {
        cout << "El taller no tiene bicicletas pendientes." << endl;
        return;
    }
    OrdenTaller orden = heapTaller->extraerMasUrgente();
    cout << "Se atiende la bicicleta " << orden.biciId
         << " (prioridad " << orden.prioridad << ")." << endl;
}
//B.2
void SistemaBicicletas::mostrarViajesDeUsuario(int dniUsuario) {
    ListaViajes viajes = buscarViajesDeUsuario(dniUsuario);

    if (viajes.estaVacia()) {
        cout << "El usuario " << dniUsuario << " no tiene viajes registrados." << endl;
        return;
    }
}
// void SistemaBicicletas::agregarBicicleta(int id, const string& codigoEstacion, TipoBici tipo, EstadoBici estado, int kilometros, bool esManual) {
//     Bicicleta nuevaBici(id, codigoEstacion, tipo, estado, kilometros);
//     bool insertado = abbBicicletas->insertar(nuevaBici); 
//     // Inserta en el árbol antes de verificar la estación (codigoEstacion)

//     if (insertado) {
//         cout << "Bicicleta " << id << " agregada con exito al ABB." << endl;

//         // Solo incrementamos las bicis disponibles si el registro fue manual
//         if (esManual) {
//             Estacion* estacion = tablaEstaciones->buscar(codigoEstacion);
//             if (estacion != nullptr) {
//                 estacion->setBicisDisponibles(estacion->getBicisDisponibles() + 1);
//                 // Suma como disponible aunque la bici esté averiada; una bici averiada no se puede retirar
//                 // Además, no se controla la capacidadAnclajes. 
//                 // BicisDisponibles puede superar capacidadAnclajes 
//                 cout << "Bicicletas disponibles en " << codigoEstacion << " actualizadas a: "
//                      << estacion->getBicisDisponibles() << endl;
//             } else {
//                 cout << "[ADVERTENCIA] La estacion " << codigoEstacion << " no existe en el sistema." << endl;
//             }
//         }

//     } else {
//         cout << "Error: La bicicleta con ID " << id << " ya existe en el sistema." << endl;
//     }
// }
void SistemaBicicletas::agregarBicicleta(int id, const string& codigoEstacion, TipoBici tipo, EstadoBici estado, int kilometros, bool esManual){
    // Antes de agregar la bicicleta a una estación, verificamos que la estación exista.
    Estacion* estacion = tablaEstaciones->buscar(codigoEstacion);
    // Si la estacion es nula
    if (estacion == nullptr) {
        cout << "Error: la estacion " << codigoEstacion << " no existe." << endl; 
        return;
    }
    // Si la estacion existe, verificamos el estado de la bicicleta y la capacidad de anclaje de la estacion
    if (esManual && estado == EstadoBici::OK && estacion->getBicisDisponibles() >= estacion->getCapacidadAnclajes()) {
        cout << "Error: la estacion " << codigoEstacion << " no tiene anclajes libres." << endl;
        return;
    }
    // Si la estacion existe, la bicicleta esta OK y hay capacidad de anclaje, agregamos la bicicleta
    // Creamos la bicicleta
    Bicicleta nuevaBici(id, codigoEstacion, tipo, estado, kilometros);
    // Validamos si no se insertó correctamente la bicicleta (Se ejecuta la función insertar y devuelve bool)
    if(!abbBicicletas->insertar(nuevaBici)){
        cout << "Error: la bicicleta con ID " << id  << " ya existe en el sistema." << endl;
        return;
    }
    // Si se insertó correctamente lo imprimimos:
    cout << "Bicicleta " << id << " agregada con exito al ABB." << endl;

    // Como la bicicleta ya se insertó correctamente, sumamos las bicicletas disponibles de la estacion.
    if (esManual && estado == EstadoBici::OK) {
        estacion->setBicisDisponibles(estacion->getBicisDisponibles() + 1);
        cout << "Bicicletas disponibles en " << codigoEstacion << ": " << estacion->getBicisDisponibles() << endl;
    }
}
void SistemaBicicletas::agregarCiclovia(const string& codigoOrigen, const string& codigoDestino) {
    if (tablaEstaciones->buscar(codigoOrigen) == nullptr || tablaEstaciones->buscar(codigoDestino) == nullptr) {
        cout << "Error: ambas estaciones deben existir para crear la ciclovia." << endl;
        return;
    }
    bool agregada = grafoCiclovias->agregarCiclovia(codigoOrigen, codigoDestino);

    if (agregada) {
        cout << "Ciclovia entre " << codigoOrigen << " y " << codigoDestino << " agregada con exito al Grafo." << endl;
    } else {
        cout << "Error: No se pudo agregar la ciclovia entre " << codigoOrigen << " y " << codigoDestino << "." << endl;
    }
}

// A6: recorrido con menor cantidad de tramos (BFS)
void SistemaBicicletas::calcularRecorridoPorCiclovia(const string& codigoOrigen, const string& codigoDestino) {
    if (tablaEstaciones->buscar(codigoOrigen) == nullptr) {
        cout << "Error: la estacion " << codigoOrigen << " no existe." << endl;
        return;
    }
    if (tablaEstaciones->buscar(codigoDestino) == nullptr) {
        cout << "Error: la estacion " << codigoDestino << " no existe." << endl;
        return;
    }

    string camino;
    int tramos = grafoCiclovias->caminoMasCorto(codigoOrigen, codigoDestino, camino);

    if (tramos < 0) {
        // -1: no hay recorrido. -2: alguna estacion no esta en el grafo (sin ciclovias)
        cout << "No existe recorrido por ciclovias entre " << codigoOrigen << " y " << codigoDestino << "." << endl;
    } else if (tramos == 0) {
        cout << "El origen y el destino son la misma estacion (0 tramos)." << endl;
    } else {
        cout << "Recorrido mas corto (" << tramos << " tramos): " << camino << endl;
    }
}

// Colisiones y factor de carga de la tabla hash
void SistemaBicicletas::mostrarEstadisticasTabla() {
    cout << "Estaciones en la tabla : " << tablaEstaciones->getCantidad() << endl;
    cout << "Tamanio de la tabla    : " << tablaEstaciones->getTamanioTabla() << endl;
    cout << "Colisiones             : " << tablaEstaciones->getColisiones() << endl;
    cout << "Factor de carga        : " << tablaEstaciones->getFactorCarga() << endl;
}