#ifndef LISTAVIAJES_H
#define LISTAVIAJES_H

#include "NodoViaje.h"

class ListaViajes {
private:
    NodoViaje* cabeza;
    int tamanio;

public:
    ListaViajes();
    ~ListaViajes();
    
    void agregarFinal(const Viaje& viaje);
    NodoViaje* getCabeza() const;
    int getTamanio() const;
    bool estaVacia() const;
};

#endif