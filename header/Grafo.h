#ifndef GRAFO_H
#define GRAFO_H

#include <string>

// Nodo de la lista de adyacencia: guarda el INDICE de la estacion vecina
struct NodoAdyacente {
    int destino;
    NodoAdyacente* siguiente;
};

class Grafo {
private:
    std::string* codigos;          // codigos[i] = codigo de la estacion i
    NodoAdyacente** adyacencia;    // adyacencia[i] = lista de vecinas de i
    int cantidadVertices;
    int capacidad;

    int indiceDe(const std::string& codigo) const;   // -1 si no existe
    void agrandar();
    bool existeArista(int a, int b) const;
    void agregarArista(int origen, int destino);

public:
    Grafo(int capacidadInicial = 16);
    ~Grafo();

    Grafo(const Grafo&) = delete;
    Grafo& operator=(const Grafo&) = delete;

    bool agregarVertice(const std::string& codigo);                       // false si ya existe
    bool agregarCiclovia(const std::string& a, const std::string& b);     // false si es duplicada
    bool existeVertice(const std::string& codigo) const;
    int getCantidadVertices() const;

    // BFS. Devuelve la cantidad de tramos del recorrido mas corto y arma
    // el camino en "camino" (ej: "PLZ -> CAB -> PAL").
    // Devuelve -1 si no hay recorrido y -2 si alguna estacion no esta en el grafo.
    int caminoMasCorto(const std::string& origen, const std::string& destino,
                       std::string& camino) const;
};

#endif