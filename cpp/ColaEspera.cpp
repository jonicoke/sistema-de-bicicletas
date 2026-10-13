#include "../header/ColaEspera.h" 
#include <stdexcept>    // Para manejar errores si intentan desencolar una cola vacía

// Constructor :para inicializar la cola vacía
ColaEspera::ColaEspera() {
    this->primero = nullptr;
    this->ultimo = nullptr;
}
ColaEspera::ColaEspera(const ColaEspera& otra) : primero(nullptr), ultimo(nullptr) {
    for (NodoCola* a = otra.primero; a != nullptr; a = a->siguiente)
        encolar(a->dniUsuario);
}

ColaEspera& ColaEspera::operator=(const ColaEspera& otra) {
    if (this != &otra) {
        while (!estaVacia()) desencolar();
        for (NodoCola* a = otra.primero; a != nullptr; a = a->siguiente)
            encolar(a->dniUsuario);
    }
    return *this;
}


// Destructor: para liberar toda la memoria cuando se destruye la cola
ColaEspera::~ColaEspera() {
    // Mientras haya nodos, los vamos desencolando 
    while (!estaVacia()) {
        desencolar();
    }
}

// Verifica si la fila está vacía
bool ColaEspera::estaVacia() const {
    return (this->primero == nullptr);
}

// Encolar: Cuando un usuario llega, lo asignamos al final de la fila
void ColaEspera::encolar(int dniUsuario) {
    // Creamos un nuevo nodoCola (pedimos espacio dem emoria) para el nuevo usuario
    NodoCola* nuevoNodo = new NodoCola;
    nuevoNodo->dniUsuario = dniUsuario;
    nuevoNodo->siguiente = nullptr; // Como es el último, le asignamos nullptr al siguiente

    if (estaVacia()) {
        // Si la cola está vacía, este usuario es el primero y también el último
        this->primero = nuevoNodo;
        this->ultimo = nuevoNodo;
    } else {
        // Si ya había gente, lo ponemos atrás del último actual
        this->ultimo->siguiente = nuevoNodo;
        // Y ahora este usuario pasa a ser el último
        this->ultimo = nuevoNodo;
    }
}

// Desencolar: Se le da la bici al primero de la fila y se retira
int ColaEspera::desencolar() {
    if (estaVacia()) {
        // Si la cola está vacía, no hay nadie a quien darle la bici, lanzamos un error
        throw runtime_error("Error: La cola está vacía.");
    }

    // Guardamos los datos del primero antes de borrarlo
    NodoCola* nodoAAtender = this->primero;
    int dni = nodoAAtender->dniUsuario;

    // El nuevo primero pasa a ser el que estaba segundo en la fila
    this->primero = this->primero->siguiente;

    // Si al sacar a esta persona la cola quedó vacía, el último también es nulo
    if (this->primero == nullptr) {
        this->ultimo = nullptr;
    }

    // Liberamos la memoria del nodo viejo para Valgrind
    delete nodoAAtender;

    return dni; // Devolvemos el DNI de la persona que acabamos de sacar
}

// 6. Ver quién está primero sin sacarlo de la fila
int ColaEspera::verPrimero() const {
    if (estaVacia()) {
        throw runtime_error("La cola está vacía.");
    }
    return this->primero->dniUsuario;
}