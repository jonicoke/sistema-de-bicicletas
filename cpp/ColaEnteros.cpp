#include "../header/ColaEnteros.h"
#include <stdexcept>

ColaEnteros::ColaEnteros(int capacidad) {
    this->capacidad = capacidad;
    this->frente = 0;
    this->cantidad = 0;
    this->datos = new int[capacidad];
}

ColaEnteros::~ColaEnteros() {
    delete[] datos;
}

bool ColaEnteros::estaVacia() const {
    return cantidad == 0;
}

// Arreglo circular: el final se calcula con aritmetica modular
void ColaEnteros::encolar(int valor) {
    if (cantidad == capacidad) {
        throw std::runtime_error("La cola de enteros esta llena.");
    }
    datos[(frente + cantidad) % capacidad] = valor;
    cantidad++;
}

int ColaEnteros::desencolar() {
    if (estaVacia()) {
        throw std::runtime_error("La cola de enteros esta vacia.");
    }
    int valor = datos[frente];
    frente = (frente + 1) % capacidad;
    cantidad--;
    return valor;
}