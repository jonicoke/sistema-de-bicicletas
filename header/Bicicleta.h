#ifndef BICICLETA_H
#define BICICLETA_H

#include <string>

enum class TipoBici {
    ELECTRICA,
    MECANICA
};

enum class EstadoBici {
    OK,
    AVERIADA
};

class Bicicleta {
private:
    int biciId;
    std::string codigoEstacion;
    TipoBici tipo;
    EstadoBici estado;
    int kilometros;

public:
    Bicicleta(int biciId, const std::string& codigoEstacion,
              TipoBici tipo, EstadoBici estado, int kilometros);

    int getBiciId() const;
    const std::string& getCodigoEstacion() const;
    TipoBici getTipo() const;
    EstadoBici getEstado() const;
    int getKilometros() const;

    bool estaAveriada() const;
    bool esElectrica() const;
    bool tieneMasKilometros(int x) const; 
    void mostrar() const;
};

#endif