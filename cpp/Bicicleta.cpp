#include "../header/Bicicleta.h"
#include <iostream>

using namespace std;

Bicicleta::Bicicleta(int biciId, const string& codigoEstacion,
                     TipoBici tipo, EstadoBici estado, int kilometros) {
    this->biciId = biciId;
    this->codigoEstacion = codigoEstacion;
    this->tipo = tipo;
    this->estado = estado;
    this->kilometros = kilometros;
}

int Bicicleta::getBiciId() const {
     return biciId; 
}

const string& Bicicleta::getCodigoEstacion() const { 
    return codigoEstacion; 
}

TipoBici Bicicleta::getTipo() const { 
    return tipo; 
}

EstadoBici Bicicleta::getEstado() const { 
    return estado; 
}

int Bicicleta::getKilometros() const { 
    return kilometros; 
}

bool Bicicleta::estaAveriada() const { 
    return estado == EstadoBici::AVERIADA; 
}

bool Bicicleta::esElectrica() const { 
    return tipo == TipoBici::ELECTRICA; 
}

bool Bicicleta::tieneMasKilometros(int x) const { 
    return kilometros > x; 
}

void Bicicleta::mostrar() const {
    cout << "Bici " << biciId
         << " | Estacion: " << codigoEstacion
         << " | Tipo: " << (esElectrica() ? "Electrica" : "Mecanica")
         << " | Estado: " << (estaAveriada() ? "Averiada" : "OK")
         << " | Km: " << kilometros << endl;
}