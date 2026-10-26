#ifndef LISTAVIAJES_H
#define LISTAVIAJES_H

#include "NodoViaje.h"

class ListaViajes {
private:
    NodoViaje* cabeza;
    NodoViaje* ultimo; // para añadir al finalsin recorrer toda la lista
    int tamanio;

    void vaciar();
public:
    ListaViajes();
    ~ListaViajes();
    ListaViajes(const ListaViajes& otra);
    ListaViajes& operator=(const ListaViajes& otra);

    void agregarFinal(const Viaje& viaje);
    NodoViaje* getCabeza() const;
    int getTamanio() const;
    bool estaVacia() const;
};

#endif