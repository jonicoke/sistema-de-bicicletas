#include "../header/Grafo.h"
#include "../header/ColaEnteros.h"

using namespace std;

Grafo::Grafo(int capacidadInicial) {
    this->capacidad = capacidadInicial;
    this->cantidadVertices = 0;
    this->codigos = new string[capacidad];
    this->activo = new bool[capacidad];
    this->adyacencia = new NodoAdyacente*[capacidad];
    for (int i = 0; i < capacidad; i++) {
        adyacencia[i] = nullptr;
        activo[i] = false;
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
    delete[] activo;
    delete[] codigos;
}
// Solo encuentra vertices activos: uno eliminado "no existe"
int Grafo::indiceDe(const string& codigo) const {
    for (int i = 0; i < cantidadVertices; i++) {
        if (activo[i] && codigos[i] == codigo) return i;
    }
    return -1;
}

void Grafo::agrandar() {
    int nuevaCapacidad = capacidad * 2;
    string* nuevosCodigos = new string[nuevaCapacidad];
    bool* nuevoActivo = new bool[nuevaCapacidad];
    NodoAdyacente** nuevaAdy = new NodoAdyacente*[nuevaCapacidad];

    for (int i = 0; i < nuevaCapacidad; i++) {
        nuevaAdy[i] = nullptr;
        nuevoActivo[i] = false;
    }
    for (int i = 0; i < cantidadVertices; i++) {
        nuevosCodigos[i] = codigos[i];
        nuevoActivo[i] = activo[i];
        nuevaAdy[i] = adyacencia[i];     // se mueven los punteros, los nodos no se copian
    }
    delete[] codigos;
    delete[] activo;
    delete[] adyacencia;
    codigos = nuevosCodigos;
    activo = nuevoActivo;
    adyacencia = nuevaAdy;
    capacidad = nuevaCapacidad;
}

bool Grafo::agregarVertice(const string& codigo) {
    if (indiceDe(codigo) != -1) return false;

// Si la estacion fue eliminada antes, se reutiliza su lugar
    for (int i = 0; i < cantidadVertices; i++) {
        if (!activo[i] && codigos[i] == codigo) {
            activo[i] = true;
            return true;
        }
    }
    if (cantidadVertices == capacidad) agrandar();
        codigos[cantidadVertices] = codigo;
        adyacencia[cantidadVertices] = nullptr;
        activo[cantidadVertices] = true;
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
// Saca la arista origen -> destino de la lista de adyacencia de origen
void Grafo::quitarArista(int origen, int destino) {
    NodoAdyacente* anterior = nullptr;
    for (NodoAdyacente* a = adyacencia[origen]; a != nullptr; a = a->siguiente) {
        if (a->destino == destino) {
            if (anterior == nullptr) adyacencia[origen] = a->siguiente;
            else anterior->siguiente = a->siguiente;
            delete a;
            return;
        }
        anterior = a;
    }
}
// Elimina la estacion del grafo junto con todas sus ciclovias
bool Grafo::eliminarVertice(const string& codigo) {
    int i = indiceDe(codigo);
    if (i == -1) return false;

    NodoAdyacente* a = adyacencia[i];
    while (a != nullptr) {
        quitarArista(a->destino, i);          // 1. uso a->destino
        NodoAdyacente* siguiente = a->siguiente;   // 2. guardo quién sigue
        delete a;                             // 3. recién ahora tiro la tarjeta
        a = siguiente;                        // 4. avanzo a la que guardé
    }
    adyacencia[i] = nullptr;
    activo[i] = false;
    return true;
}