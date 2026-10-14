#include "../header/AbbBicicleta.h"
#include <algorithm>

AbbBicicleta::AbbBicicleta() : raiz(nullptr), cantidad(0) {}

AbbBicicleta::~AbbBicicleta() {
    destruirRec(raiz);
    raiz = nullptr;
}

void AbbBicicleta::destruirRec(NodoABB* nodo) {
    if (nodo == nullptr) return;
    destruirRec(nodo->izq);
    destruirRec(nodo->der);
    delete nodo->dato;
    delete nodo;
}

int AbbBicicleta::cantidadBicicletas() const {
    return cantidad;
}

// ---------- insertar ----------
NodoABB* AbbBicicleta::insertarRec(NodoABB* nodo, Bicicleta* nueva, bool& insertado) {
    if (nodo == nullptr) {
        insertado = true;
        return new NodoABB(nueva);
    }
    if (nueva->getBiciId() < nodo->dato->getBiciId()) {
        nodo->izq = insertarRec(nodo->izq, nueva, insertado);
    } else if (nueva->getBiciId() > nodo->dato->getBiciId()) {
        nodo->der = insertarRec(nodo->der, nueva, insertado);
    }
    // si el id es igual, no se inserta (insertado queda en false)
    return nodo;
}

bool AbbBicicleta::insertar(const Bicicleta& bici) {
    Bicicleta* nueva = new Bicicleta(bici);  // el arbol guarda su propia copia
    bool insertado = false;
    raiz = insertarRec(raiz, nueva, insertado);
    if (insertado) {
        cantidad++;
    } else {
        delete nueva;  // id duplicado: liberamos la copia que no se uso
    }
    return insertado;
}

// ---------- buscar ----------
Bicicleta* AbbBicicleta::buscarRec(NodoABB* nodo, int id) const {
    if (nodo == nullptr) return nullptr;
    if (id == nodo->dato->getBiciId()) return nodo->dato;
    if (id < nodo->dato->getBiciId()) return buscarRec(nodo->izq, id);
    return buscarRec(nodo->der, id);
}

Bicicleta* AbbBicicleta::buscar(int bicId) const {
    return buscarRec(raiz, bicId);
}

// ---------- listarInorder ----------
void AbbBicicleta::inorderRec(NodoABB* nodo, std::vector<Bicicleta*>& resultado) const {
    if (nodo == nullptr) return;
    inorderRec(nodo->izq, resultado);
    resultado.push_back(nodo->dato);
    inorderRec(nodo->der, resultado);
}

std::vector<Bicicleta*> AbbBicicleta::listarInorder() const {
    std::vector<Bicicleta*> resultado;
    inorderRec(raiz, resultado);
    return resultado;
}

// ---------- contarAveriadas ----------
int AbbBicicleta::contarAveriadasRec(NodoABB* nodo) const {
    if (nodo == nullptr) return 0;
    int suma = nodo->dato->estaAveriada() ? 1 : 0;
    return suma + contarAveriadasRec(nodo->izq) + contarAveriadasRec(nodo->der);
}

int AbbBicicleta::contarAveriadas() const {
    return contarAveriadasRec(raiz);
}

// ---------- altura ----------
int AbbBicicleta::alturaRec(NodoABB* nodo) const {
    if (nodo == nullptr) return 0;
    return 1 + std::max(alturaRec(nodo->izq), alturaRec(nodo->der));
}

int AbbBicicleta::altura() const {
    return alturaRec(raiz);
}

// ---------- listarConMasDeKm ----------
// Recorrido inorder: agrega al final de la lista (usando 'cola') las bicis
// que superan los kilometros pedidos. Queda ordenada por biciId.
void AbbBicicleta::listarConMasDeKmRec(NodoABB* nodo, int kilometros,
                                       NodoLista*& cabeza, NodoLista*& cola) const {
    if (nodo == nullptr) return;

    listarConMasDeKmRec(nodo->izq, kilometros, cabeza, cola);

    if (nodo->dato->tieneMasKilometros(kilometros)) {
        NodoLista* nuevo = new NodoLista(nodo->dato);
        if (cabeza == nullptr) {
            cabeza = nuevo;
            cola = nuevo;
        } else {
            cola->sig = nuevo;
            cola = nuevo;
        }
    }

    listarConMasDeKmRec(nodo->der, kilometros, cabeza, cola);
}

NodoLista* AbbBicicleta::listarConMasDeKm(int kilometros) const {
    NodoLista* cabeza = nullptr;
    NodoLista* cola = nullptr;
    listarConMasDeKmRec(raiz, kilometros, cabeza, cola);
    return cabeza;
}

// ---------- eliminar ----------
NodoABB* AbbBicicleta::eliminarRec(NodoABB* nodo, int id, bool& eliminado) {
    if (nodo == nullptr) return nullptr;

    if (id < nodo->dato->getBiciId()) {
        nodo->izq = eliminarRec(nodo->izq, id, eliminado);
    } else if (id > nodo->dato->getBiciId()) {
        nodo->der = eliminarRec(nodo->der, id, eliminado);
    } else {
        eliminado = true;

        // Caso 1 y 2: sin hijos o con un solo hijo
        if (nodo->izq == nullptr || nodo->der == nullptr) {
            NodoABB* hijo = (nodo->izq != nullptr) ? nodo->izq : nodo->der;
            delete nodo->dato;
            delete nodo;
            return hijo;
        }

        // Caso 3: dos hijos -> sucesor inorder (minimo del subarbol derecho)
        NodoABB* sucesor = nodo->der;
        while (sucesor->izq != nullptr) {
            sucesor = sucesor->izq;
        }

        // Intercambiamos los datos y eliminamos el nodo del sucesor
        std::swap(nodo->dato, sucesor->dato);
        bool aux = false;
        nodo->der = eliminarRec(nodo->der, id, aux);
    }
    return nodo;
}

bool AbbBicicleta::eliminar(int id) {
    bool eliminado = false;
    raiz = eliminarRec(raiz, id, eliminado);
    if (eliminado) cantidad--;
    return eliminado;
}