#ifndef TABLAHASH_H
#define TABLAHASH_H

#include <string>
#include "Estacion.h"

struct NodoHash {
    Estacion estacion;      
    NodoHash* siguiente;
    NodoHash(const Estacion& e) : estacion(e), siguiente(nullptr) {}
};

class TablaHash {
private:
    NodoHash** tabla;      
    int tamanioTabla;
    int cantidad;        
    int colisiones; 

    int funcionHash(const std::string& codigo) const;

public:
    TablaHash(int tamanio = 31);
    ~TablaHash();

    TablaHash(const TablaHash&) = delete;
    TablaHash& operator=(const TablaHash&) = delete;

    bool insertar(const Estacion& estacion);          
    Estacion* buscar(const std::string& codigo) const; 
    bool eliminar(const std::string& codigo); 

    int getCantidad() const;
    int getColisiones() const;
    double getFactorCarga() const;

    // Devuelve un arreglo nuevo con punteros a todas las estaciones.
    // Quien lo llama debe hacer delete[] del arreglo (no de las estaciones).
    Estacion** obtenerEstaciones(int& cantidadOut) const;
};

#endif