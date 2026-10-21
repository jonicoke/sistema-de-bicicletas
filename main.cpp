#include <iostream>
#include "./header/MinHeap.h"
using namespace std;

int main() {
    MinHeap heap;
    Bicicleta b1(101, "PLZ", TipoBici::MECANICA,  EstadoBici::AVERIADA, 1000);  // prio 3
    Bicicleta b2(102, "PAL", TipoBici::MECANICA,  EstadoBici::AVERIADA, 2000);  // prio 2
    Bicicleta b3(103, "CAB", TipoBici::ELECTRICA, EstadoBici::AVERIADA, 500);   // prio 1 (electrica)
    Bicicleta b4(104, "PLZ", TipoBici::MECANICA,  EstadoBici::AVERIADA, 3500);  // prio 1 (km)
    Bicicleta b5(105, "PLZ", TipoBici::MECANICA,  EstadoBici::OK,       9000);  // no entra

    cout << "b5 (OK) insertada? " << (heap.insertar(b5) ? "si" : "no") << endl;
    heap.insertar(b1); heap.insertar(b2); heap.insertar(b3); heap.insertar(b4);

    while (!heap.estaVacia()) {
        OrdenTaller o = heap.extraerMasUrgente();
        cout << "Bici " << o.biciId << " - prioridad " << o.prioridad << endl;
    }
    return 0;
}