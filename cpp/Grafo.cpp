#include "../header/Grafo.h"
#include "../header/ColaEnteros.h"

using namespace std;

Grafo::Grafo(int capacidadInicial) {
    this->capacidad = capacidadInicial;
    this->cantidadVertices = 0;
    this->codigos = new string[capacidad];
    this->adyacencia = new NodoAdyacente*[capacidad];
    for (int i = 0; i < capacidad; i++) {
        adyacencia[i] = nullptr;
    }
}

Grafo::~Grafo() {
    for (int i = 0; i < cantidadVertices; i++) {
        NodoAdyacente* actual = adyacencia[i];
        while (actual != nullptr) {
            NodoAdyacente* siguiente = actual->siguiente;
            delete actual;
            actual = siguiente;
        }
    }
    delete[] adyacencia;
    delete[] codigos;
}

int Grafo::indiceDe(const string& codigo) const {
    for (int i = 0; i < cantidadVertices; i++) {
        if (codigos[i] == codigo) return i;
    }
    return -1;
}

void Grafo::agrandar() {
    int nuevaCapacidad = capacidad * 2;
    string* nuevosCodigos = new string[nuevaCapacidad];
    NodoAdyacente** nuevaAdy = new NodoAdyacente*[nuevaCapacidad];

    for (int i = 0; i < nuevaCapacidad; i++) {
        nuevaAdy[i] = nullptr;
    }
    for (int i = 0; i < cantidadVertices; i++) {
        nuevosCodigos[i] = codigos[i];
        nuevaAdy[i] = adyacencia[i];     // se mueven los punteros, los nodos no se copian
    }
    delete[] codigos;
    delete[] adyacencia;
    codigos = nuevosCodigos;
    adyacencia = nuevaAdy;
    capacidad = nuevaCapacidad;
}

bool Grafo::agregarVertice(const string& codigo) {
    if (indiceDe(codigo) != -1) return false;
    if (cantidadVertices == capacidad) agrandar();
    codigos[cantidadVertices] = codigo;
    adyacencia[cantidadVertices] = nullptr;
    cantidadVertices++;
    return true;
}

bool Grafo::existeVertice(const string& codigo) const {
    return indiceDe(codigo) != -1;
}

int Grafo::getCantidadVertices() const {
    return cantidadVertices;
}

bool Grafo::existeArista(int a, int b) const {
    for (NodoAdyacente* v = adyacencia[a]; v != nullptr; v = v->siguiente) {
        if (v->destino == b) return true;
    }
    return false;
}

void Grafo::agregarArista(int origen, int destino) {
    NodoAdyacente* nuevo = new NodoAdyacente;
    nuevo->destino = destino;
    nuevo->siguiente = adyacencia[origen];   // insercion al inicio: O(1)
    adyacencia[origen] = nuevo;
}

// Las ciclovias son bidireccionales: se agrega la arista en ambos sentidos.
bool Grafo::agregarCiclovia(const string& a, const string& b) {
    if (a == b) return false;
    agregarVertice(a);     // si ya existian, no hace nada
    agregarVertice(b);
    int ia = indiceDe(a);
    int ib = indiceDe(b);
    if (existeArista(ia, ib)) return false;   // duplicada (o repetida al reves)
    agregarArista(ia, ib);
    agregarArista(ib, ia);
    return true;
}

int Grafo::caminoMasCorto(const string& origen, const string& destino, string& camino) const {
    int o = indiceDe(origen);
    int d = indiceDe(destino);
    if (o == -1 || d == -1) return -2;

    int n = cantidadVertices;
    bool* visitado = new bool[n];
    int* padre = new int[n];
    for (int i = 0; i < n; i++) {
        visitado[i] = false;
        padre[i] = -1;
    }

    ColaEnteros cola(n);    // cada vertice se encola una sola vez, nunca se llena
    visitado[o] = true;
    cola.encolar(o);
    bool encontrado = (o == d);

    while (!cola.estaVacia() && !encontrado) {
        int actual = cola.desencolar();
        for (NodoAdyacente* v = adyacencia[actual]; v != nullptr; v = v->siguiente) {
            if (!visitado[v->destino]) {
                visitado[v->destino] = true;
                padre[v->destino] = actual;
                cola.encolar(v->destino);
                if (v->destino == d) {
                    encontrado = true;
                    break;
                }
            }
        }
    }

    int tramos = -1;
    if (encontrado) {
        // Se reconstruye siguiendo los padres desde el destino hacia el origen
        int* ruta = new int[n];
        int largo = 0;
        for (int x = d; x != -1; x = padre[x]) {
            ruta[largo++] = x;
        }
        camino = "";
        for (int i = largo - 1; i >= 0; i--) {
            camino += codigos[ruta[i]];
            if (i > 0) camino += " -> ";
        }
        tramos = largo - 1;
        delete[] ruta;
    }

    delete[] visitado;
    delete[] padre;
    return tramos;
}