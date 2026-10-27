#include "../header/TablaHash.h"

using namespace std;

TablaHash::TablaHash(int tamanio) {
    this->tamanioTabla = tamanio;
    this->cantidad = 0;
    this->colisiones = 0;
    this->tabla = new NodoHash*[tamanioTabla];
    for (int i = 0; i < tamanioTabla; i++) {
        tabla[i] = nullptr;
    }
}

TablaHash::~TablaHash() {
    for (int i = 0; i < tamanioTabla; i++) {
        NodoHash* actual = tabla[i];
        while (actual != nullptr) {
            NodoHash* siguiente = actual->siguiente;
            delete actual;
            actual = siguiente;
        }
    }
    delete[] tabla;
}

int TablaHash::funcionHash(const string& codigo) const {
    unsigned long h = 0;
    for (size_t i = 0; i < codigo.length(); i++) {
        h = h * 31 + static_cast<unsigned char>(codigo[i]);
    }
    return static_cast<int>(h % tamanioTabla);
}

bool TablaHash::insertar(const Estacion& estacion) {
    if (buscar(estacion.getCodigo()) != nullptr) {
        return false;
    }
    int pos = funcionHash(estacion.getCodigo());

    if (tabla[pos] != nullptr) {
        colisiones++;      
    }

    NodoHash* nuevo = new NodoHash(estacion);
    nuevo->siguiente = tabla[pos]; 
    tabla[pos] = nuevo;
    cantidad++;
    return true;
}

Estacion* TablaHash::buscar(const string& codigo) const {
    NodoHash* actual = tabla[funcionHash(codigo)];
    while (actual != nullptr) {
        if (actual->estacion.getCodigo() == codigo) {
            return &(actual->estacion);
        }
        actual = actual->siguiente;
    }
    return nullptr;
}

bool TablaHash::eliminar(const string& codigo) {
    int pos = funcionHash(codigo);
    NodoHash* actual = tabla[pos];
    NodoHash* anterior = nullptr;

    while (actual != nullptr) {
        if (actual->estacion.getCodigo() == codigo) {
            if (anterior == nullptr) {
                tabla[pos] = actual->siguiente;
            } else {
                anterior->siguiente = actual->siguiente;
            }
            delete actual;
            cantidad--;
            return true;
        }
        anterior = actual;
        actual = actual->siguiente;
    }
    return false;
}

int TablaHash::getCantidad() const { return cantidad; }
int TablaHash::getColisiones() const { return colisiones; }
int TablaHash::getTamanioTabla() const { return tamanioTabla; }
double TablaHash::getFactorCarga() const {
    return static_cast<double>(cantidad) / tamanioTabla;
}

Estacion** TablaHash::obtenerEstaciones(int& cantidadOut) const {
    cantidadOut = cantidad;
    if (cantidad == 0) {
        return nullptr;
    }
    Estacion** resultado = new Estacion*[cantidad];
    int k = 0;
    for (int i = 0; i < tamanioTabla; i++) {
        NodoHash* actual = tabla[i];
        while (actual != nullptr) {
            resultado[k++] = &(actual->estacion);
            actual = actual->siguiente;
        }
    }
    return resultado;
}