#include "../header/Viaje.h"
#include "../header/Estacion.h"
#include <iostream>
// DATO INTERESANTE SOBRE CONST EN METODOS
//Adelante (const Estacion*): Protege a la Estación. Quien reciba el puntero solo va a poder leer datos, sin modificar la estación original.
//Atrás (... const): Protege al Viaje. Garantiza que llamar al método no va a alterar ningún atributo interno de la clase Viaje.
// Ambos (const Estacion* getEstacionOrigen() const): Protege a ambos. Garantiza que el método no modifica el Viaje y que tampoco te deja modificar la Estacion apuntada.
using namespace std;

Viaje::Viaje() {
    this->viajeId = 0;
    this->biciId = 0;
    this->dniUsuario = 0;
    this->estacionOrigen = nullptr;
    this->estacionDestino = nullptr;
    this->fecha = 0;
    this->duracionMin = 0;
}

Viaje::Viaje(int viajeId, int biciId, int dniUsuario,
             Estacion* estacionOrigen, Estacion* estacionDestino,
             int fecha, int duracionMin) {
    this->viajeId = viajeId;
    this->biciId = biciId;
    this->dniUsuario = dniUsuario;
    this->estacionOrigen = estacionOrigen;
    this->estacionDestino = estacionDestino;
    this->fecha = fecha;
    this->duracionMin = duracionMin;
}

Viaje::~Viaje() {
// Queda vacío intencionalmente.
// No se libera memoria de estacionOrigen ni estacionDestino
// porque su ciclo de vida lo maneja la TablaHash.
}
void Viaje::mostrarInformacion() {
    cout << "Codigo de viaje   : " << viajeId << endl;
    cout << "Codigo de bici    : " << biciId << endl;
    cout << "DNI del usuario   : " << dniUsuario << endl;
    // Mostramos el código usando el getter de Estación si el puntero no es null
    cout << "Estacion origen   : " << (estacionOrigen != nullptr ? estacionOrigen->getCodigo() : "N/A") << endl;
    cout << "Estacion destino  : " << (estacionDestino != nullptr ? estacionDestino->getCodigo() : "N/A") << endl;
    cout << "Fecha del viaje   : " << fecha << endl;
    cout << "Duracion del viaje: " << duracionMin << " minutos." << endl;
}

bool Viaje::estaEnRango(int fechaDesde, int fechaHasta) const {
    return (fecha >= fechaDesde && fecha <= fechaHasta);
}

int Viaje::getViajeId() const {
    return viajeId;
}

int Viaje::getBiciId() const {
    return biciId;
}

int Viaje::getDniUsuario() const {
    return dniUsuario;
}

Estacion* Viaje::getEstacionOrigen() const {
    return estacionOrigen;
}

Estacion* Viaje::getEstacionDestino() const {
    return estacionDestino;
}

int Viaje::getFecha() const {
    return fecha;
}

int Viaje::getDuracionMin() const {
    return duracionMin;
}