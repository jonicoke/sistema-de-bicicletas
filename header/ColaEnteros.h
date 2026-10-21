#ifndef COLAENTEROS_H
#define COLAENTEROS_H

class ColaEnteros {
private:
    int* datos;
    int capacidad;
    int frente;
    int cantidad;

public:
    ColaEnteros(int capacidad);
    ~ColaEnteros();

    ColaEnteros(const ColaEnteros&) = delete;
    ColaEnteros& operator=(const ColaEnteros&) = delete;

    void encolar(int valor);
    int desencolar();
    bool estaVacia() const;
};

#endif