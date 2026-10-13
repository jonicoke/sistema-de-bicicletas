#include "../header/MinHeap.h"
#include <iostream>
#include <stdexcept>

MinHeap::MinHeap(int capacidadInicial) {
    this->capacidad = capacidadInicial;
    this->tamanio = 0;
    this->arreglo = new OrdenTaller[capacidad];
}

MinHeap::~MinHeap() {
    delete[] arreglo;
}

bool MinHeap::estaVacia() const {
    return tamanio == 0;
}

void MinHeap::intercambiar(OrdenTaller& a, OrdenTaller& b) {
    OrdenTaller temp = a;
    a = b;
    b = temp;
}

// Duplica el tamaño del arreglo cuando se llena
void MinHeap::agrandar() {
    int nuevaCapacidad = capacidad * 2;
    OrdenTaller* nuevo = new OrdenTaller[nuevaCapacidad];
    for (int i = 0; i < tamanio; i++) {
        nuevo[i] = arreglo[i];
    }
    delete[] arreglo;
    arreglo = nuevo;
    capacidad = nuevaCapacidad;
}

void MinHeap::insertar(int biciId, int prioridad) {
    if (tamanio == capacidad) {
        agrandar();
    }
    arreglo[tamanio].biciId = biciId;
    arreglo[tamanio].prioridad = prioridad;
    tamanio++;
    flotar(tamanio - 1);
}

void MinHeap::flotar(int i) {
    while (i != 0 && arreglo[padre(i)].prioridad > arreglo[i].prioridad) {
        intercambiar(arreglo[padre(i)], arreglo[i]);
        i = padre(i);
    }
}

OrdenTaller MinHeap::extraerMasUrgente() {
    if (tamanio == 0) {
        throw std::runtime_error("El taller esta vacio.");
    }
    OrdenTaller raiz = arreglo[0];
    arreglo[0] = arreglo[tamanio - 1];
    tamanio--;
    if (tamanio > 0) {
        hundir(0);
    }
    return raiz;
}

void MinHeap::hundir(int i) {
    int menor = i;
    int izq = izquierdo(i);
    int der = derecho(i);

    if (izq < tamanio && arreglo[izq].prioridad < arreglo[menor].prioridad) {
        menor = izq;
    }
    if (der < tamanio && arreglo[der].prioridad < arreglo[menor].prioridad) {
        menor = der;
    }
    if (menor != i) {
        intercambiar(arreglo[i], arreglo[menor]);
        hundir(menor);
    }
}

void MinHeap::mostrarHeap() const {
    std::cout << "Taller (Min-Heap): ";
    for (int i = 0; i < tamanio; i++) {
        std::cout << "[bici " << arreglo[i].biciId << ", prio " << arreglo[i].prioridad << "] ";
    }
    std::cout << std::endl;
}
int MinHeap::calcularPrioridad(bool esElectrica, int kilometros) {
    if (esElectrica) return 1;
    if (kilometros > 3000) return 1;
    if (kilometros > 1500) return 2;
    return 3;
}