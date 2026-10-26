#ifndef ABBBICICLETA_H
#define ABBBICICLETA_H

#include "Bicicleta.h"
#include "NodoLista.h"

// Nodo del arbol binario de busqueda (ordenado por biciId)
struct NodoABB {
    Bicicleta* dato;
    NodoABB* izq;
    NodoABB* der;

    explicit NodoABB(Bicicleta* b) : dato(b), izq(nullptr), der(nullptr) {}
};

class AbbBicicleta {
private:
    NodoABB* raiz;
    int cantidad;

    // Auxiliares recursivos
    NodoABB* insertarRec(NodoABB* nodo, Bicicleta* nueva, bool& insertado);
    NodoABB* eliminarRec(NodoABB* nodo, int id, bool& eliminado);
    Bicicleta* buscarRec(NodoABB* nodo, int id) const;
    void inorderRec(NodoABB* nodo) const;
    int contarAveriadasRec(NodoABB* nodo) const;
    int alturaRec(NodoABB* nodo) const;
    void listarConMasDeKmRec(NodoABB* nodo, int kilometros,
                             NodoLista*& cabeza, NodoLista*& cola) const;
    void destruirRec(NodoABB* nodo);

    // No se permite copiar el arbol (maneja memoria dinamica)
    AbbBicicleta(const AbbBicicleta&) = delete;
    AbbBicicleta& operator=(const AbbBicicleta&) = delete;

public:
    AbbBicicleta();
    ~AbbBicicleta();

    int cantidadBicicletas() const;
    bool insertar(const Bicicleta& bici);           // guarda una copia; false si el biciId ya existe
    Bicicleta* buscar(int bicId) const;             // nullptr si no existe
    
    void listarInorder() const;    int contarAveriadas() const;
    int altura() const;                             // arbol vacio = 0

    // Devuelve una lista (ordenada por biciId) con las bicis de mas de 'kilometros' km.
    // Lista vacia = nullptr. El que llama debe liberarla con liberarLista().
    NodoLista* listarConMasDeKm(int kilometros) const;

    bool eliminar(int id);                          // false si no existe
};

#endif