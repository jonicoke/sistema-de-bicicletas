#ifndef NODOVIAJE_H
#define NODOVIAJE_H

#include "Viaje.h"

class NodoViaje {
private:
    Viaje viaje;
    NodoViaje* siguiente;

public:

    NodoViaje(const Viaje& viaje, NodoViaje* siguiente = nullptr);

    Viaje getViaje() const;
    NodoViaje* getSiguiente() const;
    void setSiguiente(NodoViaje* siguiente);
};

#endif