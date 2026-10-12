#include "../header/Estacion.h"

// Creación del objeto Estacion con valores dados por parámetro.
Estacion::Estacion(const string &codigo, const string &nombre, const string &barrio, int capacidadAnclajes)
{
    this->codigo = codigo;
    this->nombre = nombre;
    this->barrio = barrio;
    this->capacidadAnclajes = capacidadAnclajes;
    this->bicisDisponibles = 0;
}
