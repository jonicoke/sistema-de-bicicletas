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
// Método para retirar una bicicleta de una estación.
void Estacion::retirarBicicleta(int dniUsuario)
{
    // Si hay bicicletas disponibles, se retira una y se decrementa el contador
    if (this->bicisDisponibles > 0) {
         this->bicisDisponibles--;
    } else {
        // Si no hay bicicletas disponibles, el usuario se agrega a la cola de espera
        this->colaEspera.encolar(dniUsuario);
    }
}
// Método para devolver una bicicleta a una estación.
void Estacion::devolverBicicleta()
{
    // Verificamos si hay lugar para devolver la bicicleta
    if (this->bicisDisponibles >= this->capacidadAnclajes) {
        return; // No hay lugar para devolver la bicicleta
    }

    // B.4 Asignar al primero de la cola si hay gente esperando
    if (!this->colaEspera.estaVacia()) {
        // Si hay usuarios en la cola, se le da la bicicleta al primero
        this->colaEspera.desencolar();
    } else {
        // Si no hay usuarios en la cola, incrementa el contador de bicicletas disponibles
        this->bicisDisponibles++;
    }
}