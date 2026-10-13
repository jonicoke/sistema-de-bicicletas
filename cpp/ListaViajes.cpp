#include "../header/ListaViajes.h"

ListaViajes::ListaViajes() {
    this->cabeza = nullptr;
    this->tamanio = 0;
}

ListaViajes::~ListaViajes() {
    NodoViaje* actual = cabeza;
    while (actual != nullptr) {
        NodoViaje* borrar = actual;
        actual = actual->getSiguiente();
        delete borrar;
    }
}

void ListaViajes::agregarFinal(const Viaje& viaje) {
    NodoViaje* nuevoNodo = new NodoViaje(viaje);

    if (estaVacia()) {
        cabeza = nuevoNodo;
    } else {
        NodoViaje* actual = cabeza;
        while (actual->getSiguiente() != nullptr) {
            actual = actual->getSiguiente();
        }
        actual->setSiguiente(nuevoNodo);
    }
    tamanio++;
}

NodoViaje* ListaViajes::getCabeza() const {
    return cabeza;
}

int ListaViajes::getTamanio() const {
    return tamanio;
}

bool ListaViajes::estaVacia() const {
    return cabeza == nullptr;
}