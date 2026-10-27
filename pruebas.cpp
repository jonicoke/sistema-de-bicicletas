#include <iostream>
#include <stdexcept>
#include <string>
#include "header/SistemaBicicletas.h"
#include "header/Grafo.h"
#include "header/MinHeap.h"
#include "header/ColaEspera.h"
#include "header/TablaHash.h"

using namespace std;

static int fallos = 0;

static void verificar(bool condicion, const string& descripcion) {
    cout << (condicion ? "  [OK]    " : "  [FALLO] ") << descripcion << endl;
    if (!condicion) fallos++;
}

// ---------- B4: cola de espera ----------
static void probarColaDeEspera() {
    cout << "\n== B4: ColaEspera (FIFO y copia profunda) ==" << endl;
    ColaEspera a;
    a.encolar(111);
    a.encolar(222);
    {
        ColaEspera b = a;          // copia profunda
        b.encolar(333);
        verificar(b.desencolar() == 111, "la copia entrega primero al 111");
    }                              // se destruye b: no debe afectar a 'a'
    verificar(a.verPrimero() == 111, "el original no se altero");
    verificar(a.desencolar() == 111, "FIFO: sale el 111");
    verificar(a.desencolar() == 222, "FIFO: sale el 222");
    verificar(a.estaVacia(), "la cola queda vacia");
}

// ---------- B4 y A5 sobre Estacion ----------
static void probarEstacion() {
    cout << "\n== B4 y A5: Estacion ==" << endl;
    Estacion e("PLZ", "PlazaDeMayo", "Monserrat", 5, 1);
    verificar(e.retirarBicicleta(111) == true, "111 retira la unica bici");
    verificar(e.retirarBicicleta(222) == false, "222 queda en cola");
    verificar(e.retirarBicicleta(333) == false, "333 queda en cola");

    int dni = 0;
    e.devolverBicicleta(dni);
    verificar(dni == 222, "la 1ra devolucion va al 222");
    e.devolverBicicleta(dni);
    verificar(dni == 333, "la 2da devolucion va al 333");
    e.devolverBicicleta(dni);
    verificar(dni == 0 && e.getBicisDisponibles() == 1, "la 3ra: nadie espera, bicis = 1");

    Estacion llena("LLE", "Llena", "X", 2, 2);
    verificar(llena.devolverBicicleta(dni) == false, "estacion llena rechaza la devolucion");

    Estacion cab("CAB", "PrimeraJunta", "Caballito", 25, 4);
    Estacion plz("PLZ", "PlazaDeMayo", "Monserrat", 30, 12);
    Estacion justo("JUS", "Justo", "X", 10, 2);
    Estacion poco("POC", "Poco", "X", 10, 1);
    verificar(cab.tieneBajaOcupacion(), "CAB (4/25 = 16%) tiene baja ocupacion");
    verificar(!plz.tieneBajaOcupacion(), "PLZ (12/30 = 40%) no");
    verificar(!justo.tieneBajaOcupacion(), "exactamente 20% no entra (es menor estricto)");
    verificar(poco.tieneBajaOcupacion(), "10% si entra");
}

// ---------- B5: min-heap ----------
static void probarHeap() {
    cout << "\n== B5: MinHeap ==" << endl;
    MinHeap h;
    Bicicleta b1(101, "PLZ", TipoBici::MECANICA, EstadoBici::AVERIADA, 1000);   // prio 3
    Bicicleta b2(102, "PAL", TipoBici::MECANICA, EstadoBici::AVERIADA, 2000);   // prio 2
    Bicicleta b3(103, "CAB", TipoBici::ELECTRICA, EstadoBici::AVERIADA, 500);   // prio 1 (electrica)
    Bicicleta b4(104, "PLZ", TipoBici::MECANICA, EstadoBici::AVERIADA, 3500);   // prio 1 (km)
    Bicicleta b5(105, "PLZ", TipoBici::MECANICA, EstadoBici::OK, 9000);         // no entra

    verificar(!h.insertar(b5), "una bici OK no entra al taller");
    h.insertar(b1);
    h.insertar(b2);
    h.insertar(b3);
    h.insertar(b4);

    int esperadas[4] = {1, 1, 2, 3};
    bool ordenCorrecto = true;
    for (int i = 0; i < 4; i++) {
        if (h.estaVacia() || h.extraerMasUrgente().prioridad != esperadas[i]) ordenCorrecto = false;
    }
    verificar(ordenCorrecto && h.estaVacia(), "salen en orden de prioridad 1, 1, 2, 3");

    bool lanzo = false;
    try { h.extraerMasUrgente(); } catch (const runtime_error&) { lanzo = true; }
    verificar(lanzo, "extraer de un taller vacio lanza excepcion");

    MinHeap grande;   // 40 > capacidad inicial (16): obliga a agrandar
    for (int i = 1; i <= 40; i++) {
        grande.insertar(Bicicleta(i, "PLZ", TipoBici::MECANICA, EstadoBici::AVERIADA, i * 100));
    }
    int previa = 0, sacadas = 0;
    bool ordenado = true;
    while (!grande.estaVacia()) {
        int p = grande.extraerMasUrgente().prioridad;
        if (p < previa) ordenado = false;
        previa = p;
        sacadas++;
    }
    verificar(ordenado && sacadas == 40, "40 bicis (el heap se agranda): salen ordenadas");
}

// ---------- A6: grafo y BFS ----------
static void probarGrafo() {
    cout << "\n== A6: Grafo y BFS ==" << endl;
    Grafo g;
    g.agregarCiclovia("PLZ", "CAB");
    g.agregarCiclovia("CAB", "PAL");
    g.agregarCiclovia("PAL", "RET");
    g.agregarVertice("BOE");   // estacion aislada

    string camino;
    verificar(g.caminoMasCorto("PLZ", "RET", camino) == 3 && camino == "PLZ -> CAB -> PAL -> RET",
              "PLZ -> RET: 3 tramos");
    verificar(g.caminoMasCorto("PLZ", "BOE", camino) == -1, "estacion aislada: no hay recorrido");
    verificar(g.caminoMasCorto("PLZ", "XXX", camino) == -2, "estacion inexistente: -2");
    verificar(g.caminoMasCorto("PLZ", "PLZ", camino) == 0, "origen = destino: 0 tramos");
    verificar(!g.agregarCiclovia("CAB", "PLZ"), "ciclovia repetida al reves se ignora");

    g.agregarCiclovia("PLZ", "RET");   // atajo
    verificar(g.caminoMasCorto("PLZ", "RET", camino) == 1, "con atajo: 1 tramo (BFS da el mas corto)");

    Grafo h;
    h.agregarCiclovia("PLZ", "CAB");
    h.agregarCiclovia("CAB", "PAL");
    verificar(h.caminoMasCorto("PLZ", "PAL", camino) == 2, "antes de eliminar: 2 tramos");
    verificar(h.eliminarVertice("CAB"), "se elimina CAB");
    verificar(h.caminoMasCorto("PLZ", "PAL", camino) == -1, "sin CAB no hay recorrido");
    verificar(h.caminoMasCorto("CAB", "PAL", camino) == -2, "CAB ya no existe en el grafo");
    verificar(!h.eliminarVertice("CAB"), "eliminar dos veces devuelve false");
}

// ---------- Tabla hash ----------
static void probarTablaHash() {
    cout << "\n== Tabla hash ==" << endl;
    TablaHash t;
    t.insertar(Estacion("PLZ", "PlazaDeMayo", "Monserrat", 30, 12));
    t.insertar(Estacion("PAL", "ParqueTresDeFebrero", "Palermo", 40, 35));
    t.insertar(Estacion("CAB", "PrimeraJunta", "Caballito", 25, 4));

    verificar(t.getCantidad() == 3, "3 estaciones cargadas");
    verificar(t.buscar("CAB") != nullptr && t.buscar("XXX") == nullptr, "buscar encuentra / no encuentra");
    verificar(!t.insertar(Estacion("PLZ", "Otra", "Otra", 1, 1)), "codigo repetido rechazado");

    int n = 0;
    Estacion** arr = t.obtenerEstaciones(n);
    verificar(n == 3, "obtenerEstaciones devuelve las 3");
    delete[] arr;

    verificar(t.eliminar("PAL") && t.buscar("PAL") == nullptr && t.getCantidad() == 2, "eliminar PAL");
    verificar(!t.eliminar("PAL"), "eliminar algo inexistente devuelve false");
    cout << "  Colisiones: " << t.getColisiones() << " | Factor de carga: " << t.getFactorCarga() << endl;

    TablaHash chica(2);   // tabla chica: fuerza colisiones
    bool todasEncontradas = true;
    for (int i = 1; i <= 5; i++) {
        chica.insertar(Estacion("A" + to_string(i), "n", "b", 10, 5));
    }
    for (int i = 1; i <= 5; i++) {
        if (chica.buscar("A" + to_string(i)) == nullptr) todasEncontradas = false;
    }
    verificar(chica.getColisiones() > 0 && todasEncontradas && chica.getCantidad() == 5,
              "tabla de 2 posiciones con 5 estaciones: hay colisiones y se encuentran todas");
}

// ---------- Escenario completo a traves del sistema ----------
static void probarSistema() {
    cout << "\n== Sistema completo ==" << endl;
    SistemaBicicletas s;
    s.agregarEstacion(Estacion("PLZ", "PlazaDeMayo", "Monserrat", 30, 12));
    s.agregarEstacion(Estacion("PAL", "ParqueTresDeFebrero", "Palermo", 40, 35));
    s.agregarEstacion(Estacion("CAB", "PrimeraJunta", "Caballito", 25, 4));
    s.agregarEstacion(Estacion("RET", "Retiro", "Retiro", 20, 10));

    cout << "\n-- A1: mostrar PLZ, y una inexistente (debe dar error)" << endl;
    s.mostrarInformacionDeEstacion("PLZ");
    s.mostrarInformacionDeEstacion("XXX");

    cout << "\n-- A2: agregar una repetida (debe dar error)" << endl;
    s.agregarEstacion(Estacion("PLZ", "PlazaDeMayo", "Monserrat", 30, 12));

    cout << "\n-- A4 por capacidad (esperado: RET 20, CAB 25, PLZ 30, PAL 40)" << endl;
    s.listarEstacionesOrdenadas(true);
    cout << "\n-- A4 por bicis (esperado: CAB 4, RET 10, PLZ 12, PAL 35)" << endl;
    s.listarEstacionesOrdenadas(false);

    cout << "\n-- A5 (esperado: solo CAB)" << endl;
    s.listarEstacionesConBajaOcupacion();

    cout << "\n-- A6: ciclovias y recorridos" << endl;
    s.agregarCiclovia("PLZ", "CAB");
    s.agregarCiclovia("CAB", "PAL");
    s.agregarCiclovia("CAB", "PLZ");   // repetida al reves
    s.agregarCiclovia("PLZ", "XXX");   // estacion inexistente
    s.calcularRecorridoPorCiclovia("PLZ", "PAL");   // 2 tramos
    s.calcularRecorridoPorCiclovia("PLZ", "RET");   // no existe recorrido
    s.calcularRecorridoPorCiclovia("PLZ", "XXX");   // error: no existe

    cout << "\n-- Tabla hash: estadisticas (4 estaciones)" << endl;
    s.mostrarEstadisticasTabla();

    cout << "\n-- B1 y B2" << endl;
    s.registrarViaje(Viaje(9001, 501, 30123456, "PLZ", "PAL", 20260310, 25));
    s.registrarViaje(Viaje(9002, 502, 27654321, "PAL", "CAB", 20260311, 40));
    s.registrarViaje(Viaje(9003, 501, 30123456, "PLZ", "CAB", 20260420, 15));
    verificar(s.contarViajesDesdeEstacion("PLZ", 20260301, 20260331) == 1, "PLZ en marzo: 1 viaje");
    verificar(s.contarViajesDesdeEstacion("PLZ", 20260301, 20260430) == 2, "PLZ marzo-abril: 2 viajes");
    verificar(s.contarViajesDesdeEstacion("PAL", 20260301, 20260331) == 1, "PAL en marzo: 1 viaje");
    verificar(s.contarViajesDesdeEstacion("CAB", 20260301, 20260430) == 0, "CAB: 0 viajes");
    verificar(s.contarViajesDesdeEstacion("PLZ", 20260331, 20260301) == 0, "rango invertido: 0");
    verificar(s.mostrarMinutosPedaleadosDeUsuario(30123456) == 40, "DNI 30123456: 25 + 15 = 40 min");
    verificar(s.mostrarMinutosPedaleadosDeUsuario(99) == 0, "DNI sin viajes: 0 min");
    s.mostrarViajesDeUsuario(30123456);

    cout << "\n-- B3 y B5: bicicletas" << endl;
    s.agregarBicicleta(501, "PLZ", TipoBici::MECANICA, EstadoBici::OK, 1240);
    s.agregarBicicleta(502, "PAL", TipoBici::ELECTRICA, EstadoBici::AVERIADA, 3810);
    s.agregarBicicleta(503, "CAB", TipoBici::MECANICA, EstadoBici::AVERIADA, 1600);
    s.agregarBicicleta(504, "PLZ", TipoBici::MECANICA, EstadoBici::AVERIADA, 800);

    cout << "\n-- B3 con X = 1500 (esperado: 502 y 503)" << endl;
    s.listarBicicletasConXKm(1500);

    cout << "\n-- B5: enviar al taller (502 prio 1, 503 prio 2, 504 prio 3; 501 no entra; 999 no existe)" << endl;
    s.enviarBicicletaATaller(502);
    s.enviarBicicletaATaller(503);
    s.enviarBicicletaATaller(504);
    s.enviarBicicletaATaller(501);
    s.enviarBicicletaATaller(999);
    cout << "\n-- B5: atender (esperado: 502, 503, 504 y despues taller vacio)" << endl;
    s.atenderTaller();
    s.atenderTaller();
    s.atenderTaller();
    s.atenderTaller();

    cout << "\n-- B4 en CAB (4 bicis): 4 retiros, el 5to queda en cola" << endl;
    for (int dni = 1; dni <= 4; dni++) s.retirarBicicleta("CAB", dni);
    s.retirarBicicleta("CAB", 5);       // queda en cola
    s.devolverBicicleta("CAB");         // se la lleva el 5
    s.devolverBicicleta("CAB");         // nadie esperaba
    s.retirarBicicleta("XXX", 1);       // estacion inexistente

    cout << "\n-- A3: eliminar CAB y volver a pedir PLZ -> PAL (esperado: no existe recorrido)" << endl;
    s.eliminarEstacion("CAB");
    s.calcularRecorridoPorCiclovia("PLZ", "PAL");
    s.eliminarEstacion("CAB");          // ya no existe: error
}

int main() {
    probarColaDeEspera();
    probarEstacion();
    probarHeap();
    probarGrafo();
    probarTablaHash();
    probarSistema();

    cout << "\n=============================" << endl;
    if (fallos == 0) cout << "TODAS LAS VERIFICACIONES PASARON" << endl;
    else cout << "VERIFICACIONES FALLIDAS: " << fallos << endl;
    return fallos == 0 ? 0 : 1;
}