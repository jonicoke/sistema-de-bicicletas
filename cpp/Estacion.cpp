#include "../header/Estacion.h"
#include <iostream>
// Creación del objeto Estacion con valores dados por parámetro.
Estacion::Estacion(const string &codigo, const string &nombre, const string &barrio, int capacidadAnclajes)
{
    this->codigo = codigo;
    this->nombre = nombre;
    this->barrio = barrio;
    this->capacidadAnclajes = capacidadAnclajes;
    this->bicisDisponibles = 0;
}
void Estacion::mostrarInformacion()
{
    cout << "Codigo: " << codigo << endl;
    cout << "Nombre: " << nombre << endl;
    cout << "Barrio: " << barrio << endl;
    cout << "Capacidad de anclajes: " << capacidadAnclajes << endl;
    cout << "Bicicletas disponibles: " << bicisDisponibles << endl;
}