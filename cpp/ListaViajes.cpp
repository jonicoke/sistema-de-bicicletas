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

// Constructor de Copia
ListaViajes::ListaViajes(const ListaViajes& otra) {
    this->cabeza = nullptr;
    this->tamanio = 0;

    // Recorremos la lista (otra lista ) y agregamos una copia de cada viaje
    NodoViaje* actual = otra.cabeza;
    while (actual != nullptr) {
        this->agregarFinal(actual->getViaje());
        actual = actual->getSiguiente();
    }
}


ListaViajes& ListaViajes::operator=(const ListaViajes& otra) {

    if (this != &otra) {
        // Liberar la memoria actual de 'this'
        NodoViaje* actual = cabeza;
        while (actual != nullptr) {
            NodoViaje* borrar = actual;
            actual = actual->getSiguiente();
            delete borrar;
        }
        cabeza = nullptr;
        tamanio = 0;

        // Copiar los nodos de la otra lista
        actual = otra.cabeza;
        while (actual != nullptr) {
            this->agregarFinal(actual->getViaje());
            actual = actual->getSiguiente();
        }
    }
    return *this;
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