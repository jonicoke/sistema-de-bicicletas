#ifndef MIN_HEAP_H
#define MIN_HEAP_H
struct OrdenTaller {
    int biciId;
    int prioridad;   // 1 = más urgente, 3 = menos urgente
};

class MinHeap {
private:
    OrdenTaller* arreglo;
    int capacidad;
    int tamanio;

    int padre(int i) const { return (i - 1) / 2; }
    int izquierdo(int i) const { return 2 * i + 1; }
    int derecho(int i) const { return 2 * i + 2; }

    void intercambiar(OrdenTaller& a, OrdenTaller& b);
    void flotar(int i);
    void hundir(int i);
    void agrandar();

public:
    MinHeap(int capacidadInicial = 16);
    ~MinHeap();

    MinHeap(const MinHeap&) = delete;
    MinHeap& operator=(const MinHeap&) = delete;

    void insertar(int biciId, int prioridad);
    OrdenTaller extraerMasUrgente();
    bool estaVacia() const;
    void mostrarHeap() const;
    static int calcularPrioridad(bool esElectrica, int kilometros);
};

#endif