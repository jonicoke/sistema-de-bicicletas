#include "../header/Estacion.h"
#include <iostream>
// Creación del objeto Estacion con valores dados por parámetro.
Estacion::Estacion(const string &codigo, const string &nombre, const string &barrio, int capacidadAnclajes, int bicisDisponibles)
{
    this->codigo = codigo;
    this->nombre = nombre;
    this->barrio = barrio;
    this->capacidadAnclajes = capacidadAnclajes;
    this->bicisDisponibles = bicisDisponibles;
}
void Estacion::mostrarInformacion(){
    cout << "Codigo de estacion : " << codigo << endl;
    cout << "Nombre de estacion : " << nombre << endl;
    cout << "Barrio de estacion : " << barrio << endl;
    cout << "Capacidad de anclaje : " << capacidadAnclajes << endl;
    cout << "Bicis disponibles : " << bicisDisponibles << endl;
}


string Estacion::getCodigo() const {
    return codigo;
}

const string& Estacion::getNombre() const {
    return nombre;
}

const string& Estacion::getBarrio() const {
    return barrio;
}

int Estacion::getCapacidadAnclajes() const {
    return capacidadAnclajes;
}

int Estacion::getBicisDisponibles() const {
    return bicisDisponibles;
}

const ColaEspera& Estacion::getColaEspera() const {
    return colaEspera;
}

bool Estacion::tieneBajaOcupacion() const {
    return this->bicisDisponibles * 100 < this->capacidadAnclajes * 20;
}
// Método para retirar una bicicleta de una estación.
bool Estacion::retirarBicicleta(int dniUsuario) {
    if (bicisDisponibles > 0) {
        bicisDisponibles--;
        return true;
    }
    colaEspera.encolar(dniUsuario);
    return false;
}
// Método para devolver una bicicleta a una estación.
bool Estacion::devolverBicicleta(int& dniAsignado) {
    dniAsignado = 0;
    if (colaEspera.estaVacia()) {
        if (bicisDisponibles >= capacidadAnclajes) return false;
        bicisDisponibles++;
    } else {
        dniAsignado = colaEspera.desencolar();
    }
    return true;
}