#ifndef NODOLISTA_H
#define NODOLISTA_H

#include "Bicicleta.h"

// Nodo de lista simplemente enlazada.
// Guarda un puntero a una bicicleta que pertenece al arbol (no es copia).
struct NodoLista {
    Bicicleta* dato;
    NodoLista* sig;

    explicit NodoLista(Bicicleta* b) : dato(b), sig(nullptr) {}
};

// Libera SOLO los nodos de la lista. Las bicicletas siguen vivas en el arbol.
inline void liberarLista(NodoLista* cabeza) {
    while (cabeza != nullptr) {
        NodoLista* aux = cabeza;
        cabeza = cabeza->sig;
        delete aux;
    }
}

#endif