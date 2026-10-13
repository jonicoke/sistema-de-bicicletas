#include "../header/NodoViaje.h"

NodoViaje::NodoViaje(const Viaje& viaje, NodoViaje* siguiente) {
    this->viaje = viaje;
    this->siguiente = siguiente;
}

Viaje NodoViaje::getViaje() const {
    return viaje;
}

NodoViaje* NodoViaje::getSiguiente() const {
    return siguiente;
}

void NodoViaje::setSiguiente(NodoViaje* siguiente) {
    this->siguiente = siguiente;
}