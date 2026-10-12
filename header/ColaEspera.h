#ifndef COLAESPERA_H
#define COLAESPERA_H

using namespace std;

// Estructura del NodoCola
struct NodoCola {
    int dniUsuario;
    NodoCola* siguiente;
};

class ColaEspera {
private:
    NodoCola* primero;
    NodoCola* ultimo;

public:
    // Constructor vacío para inicializar la cola
    ColaEspera();
    
    // Destructor (Crucial para liberar memoria y pasar las pruebas de Valgrind)
    ~ColaEspera();

    // Métodos principales de la cola (FIFO)
    void encolar(int dni);
    int desencolar();
    bool estaVacia() const;
    
    // Método auxiliar opcional para ver quién es el primero sin sacarlo
    int verFrente() const;
};

#endif