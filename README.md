# Sistema de Bicicletas Públicas

**Integrantes:** Alan González, Jonathan Giacomini.

**Materia / Comisión / Año:** Algoritmos - Comisión 1 - 2026

Sistema en C++ orientado a objetos que gestiona una red de estaciones de bicicletas públicas de la ciudad de Buenos Aires: estaciones, bicicletas, viajes de los usuarios y recorridos por ciclovías. Usa memoria dinámica y estructuras de datos implementadas a mano (tabla hash, cola, min-heap, árbol binario de búsqueda y grafo). **No se usan** `vector`, `map`, `unordered_map`, `queue`, `priority_queue` ni `set` de la STL. Sí se usa `std::string` y la STL para leer archivos.

---

## 1. Compilación y ejecución

Requiere `g++` con soporte de C++17.

```bash
g++ -std=c++17 -Wall -Wextra main.cpp cpp/*.cpp -o programa
./programa
```

El programa debe ejecutarse **desde la raíz del proyecto**, porque lee los archivos con rutas relativas (`datos/...`).

### Estructura del proyecto

```
TP/
├── main.cpp          Menú por consola y carga de archivos
├── header/           Archivos .h (declaraciones)
├── cpp/              Archivos .cpp (implementaciones)
├── datos/            estaciones.txt, bicicletas.txt, viajes.txt, ciclovias.txt
└── README.md
```

### Archivos de entrada

Un registro por línea, con los campos separados por espacios.

| Archivo | Formato |
|---|---|
| `estaciones.txt` | `codigo nombre barrio capacidad_anclajes bicis_disponibles` |
| `bicicletas.txt` | `bici_id codigo_estacion tipo estado kilometros` |
| `viajes.txt` | `viaje_id bici_id dni_usuario estacion_origen estacion_destino fecha duracion_min` |
| `ciclovias.txt` | `estacion_a estacion_b` |

**Orden de carga:** estaciones → bicicletas → viajes → ciclovías. Es necesario porque al agregar una bicicleta o una ciclovía se valida que la estación exista.

### Verificación de memoria

```bash
valgrind --leak-check=full ./programa
```

Resultado obtenido: 
```
==2732==
==2732== HEAP SUMMARY:
==2732==     in use at exit: 0 bytes in 0 blocks
==2732==   total heap usage: 163 allocs, 163 frees, 83,622 bytes allocated
==2732==
==2732== All heap blocks were freed -- no leaks are possible
==2732==
==2732== For lists of detected and suppressed errors, rerun with: -s
==2732== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```
---

## 2. Módulos

| Módulo | Archivos | Descripción |
|---|---|---|
| **Estacion** | `Estacion.h/.cpp` | Datos de una estación (código, nombre, barrio, capacidad, bicis disponibles). Contiene su cola de espera por composición. Implementa retirar y devolver bicicletas y la regla de baja ocupación (< 20 %) |
| **ColaEspera** | `ColaEspera.h/.cpp` | Cola FIFO de DNIs, enlazada con punteros a primero y último. Incluye destructor, constructor de copia y `operator=` (copia profunda) |
| **TablaHash** | `TablaHash.h/.cpp` | Tabla hash que indexa las estaciones por código. Resolución de colisiones por encadenamiento. Cuenta colisiones y calcula el factor de carga |
| **Grafo** | `Grafo.h/.cpp` | Grafo no dirigido de estaciones (lista de adyacencia). Los vértices son códigos de estación y las aristas son las ciclovías. Implementa BFS para el camino de menos tramos y baja lógica de vértices |
| **ColaEnteros** | `ColaEnteros.h/.cpp` | Cola circular de enteros sobre arreglo. La usa el BFS |
| **MinHeap** | `MinHeap.h/.cpp` | Cola de prioridad del taller de reparaciones. Arreglo dinámico que crece al llenarse. Guarda órdenes (`OrdenTaller`: id de bici + prioridad) |
| **Bicicleta** | `Bicicleta.h/.cpp` | Datos de una bicicleta, con los enums `TipoBici` (MECANICA, ELECTRICA) y `EstadoBici` (OK, AVERIADA) |
| **AbbBicicleta** | `AbbBicicleta.h/.cpp`, `NodoLista.h` | Árbol binario de búsqueda de bicicletas ordenado por `biciId`. Inserta, busca, lista inorder, cuenta averiadas, calcula la altura, lista por kilómetros, elimina (tres casos) y libera la memoria en postorder |
| **Viaje / NodoViaje / ListaViajes** | `Viaje.h/.cpp`, `NodoViaje.h/.cpp`, `ListaViajes.h/.cpp` | Viajes de los usuarios guardados en una lista enlazada con puntero al último nodo (agregar al final en O(1)) y copia profunda |
| **SistemaBicicletas** | `SistemaBicicletas.h/.cpp` | Clase central. Contiene (por composición) la tabla hash, el grafo, el heap, el árbol y la lista de viajes, y ofrece todas las funcionalidades al menú |
| **main** | `main.cpp` | Carga de los archivos y menú por consola que permite probar cada funcionalidad |

### Funcionalidades y dónde están

| Punto del enunciado | Método de `SistemaBicicletas` |
|---|---|
| A. Mostrar una estación | `mostrarInformacionDeEstacion` |
| A. Agregar una estación | `agregarEstacion` |
| A. Eliminar una estación | `eliminarEstacion` |
| A. Listar ordenadas por capacidad o bicis disponibles | `listarEstacionesOrdenadas` |
| A. Estaciones con menos del 20 % de ocupación | `listarEstacionesConBajaOcupacion` |
| A. Recorrido con menos tramos (BFS) | `calcularRecorridoPorCiclovia` (carga de ciclovías: `agregarCiclovia`) |
| A. Colisiones y factor de carga | `mostrarEstadisticasTabla` |
| B1. Viajes desde una estación en un rango de fechas | `contarViajesDesdeEstacion` |
| B2. Viajes de un usuario y minutos pedaleados | `buscarViajesDeUsuario`, `mostrarViajesDeUsuario`, `mostrarMinutosPedaleadosDeUsuario` |
| B3. Bicicletas con más de X km | `listarBicicletasConXKm` |
| B4. Cola de espera por estación | `retirarBicicleta`, `devolverBicicleta` |
| B5. Taller (min-heap) | `enviarBicicletaATaller`, `atenderTaller` |
| C. Registro en el ABB | `agregarBicicleta` y los métodos de `AbbBicicleta` |

---

## 3. Decisiones de diseño

- **Las estaciones forman un grafo.** Las ciclovías se modelan con la clase `Grafo`, cuyos vértices son los códigos de las estaciones y cuyas aristas se guardan en ambos sentidos (las conexiones son bidireccionales). Las ciclovías repetidas, incluso las que aparecen al revés (`PLZ CAB` y `CAB PLZ`), se ignoran. 
- **Eliminar una estación** la quita de la tabla hash y del grafo junto con todas sus ciclovías (baja lógica del vértice, que se marca como inactivo). Así ningún recorrido puede pasar por una estación inexistente. Sus bicicletas no se reubican, como indica el enunciado.
- **Los viajes guardan los códigos de estación como texto**, no punteros. De esa forma siguen siendo válidos aunque la estación se elimine.
- **`bicisDisponibles`** es un dato propio de la estación, tomado de `estaciones.txt`, que se actualiza al retirar y devolver. Las bicicletas del ABB son el registro general.
- **Ocupación menor al 20 %** se calcula con enteros (`bicis * 100 < capacidad * 20`) para evitar errores de redondeo. El 20 % exacto **no** entra.
- **Taller de reparaciones.** Solo entran bicicletas averiadas. Criterio de prioridad (1 = más urgente):
  - Eléctricas: siempre prioridad **1**, como pide el enunciado.
  - Mecánicas averiadas, según los kilómetros: más de 3000 km → **1**, más de 1500 km → **2**, el resto → **3**.

  El enunciado no fija cómo se asigna la prioridad a las mecánicas, por eso se eligió este criterio: las que más se usaron son las más desgastadas.
- **Copia profunda.** `ColaEspera` y `ListaViajes` tienen constructor de copia y `operator=`, porque manejan memoria dinámica y se copian (por ejemplo, al insertar una `Estacion` en la tabla hash). Sin eso, dos objetos compartirían los mismos nodos y se liberarían dos veces. `TablaHash`, `Grafo`, `MinHeap`, `AbbBicicleta` y `ColaEnteros` tienen la copia deshabilitada.

---

## 4. Tabla hash: colisiones y factor de carga

- **Función hash:** polinómica sobre los caracteres del código, `h = h * 31 + caracter`, módulo el tamaño de la tabla.
- **Tamaño de la tabla:** 31 posiciones.
- **Resolución de colisiones:** encadenamiento (cada posición guarda una lista enlazada; las inserciones se hacen al inicio).
- **Colisiones:** se cuenta una colisión cada vez que se inserta una estación en una posición que ya tenía alguna. El contador no se descuenta al eliminar.
- **Factor de carga:** estaciones guardadas ÷ tamaño de la tabla.

Resultados con los datos de prueba:

| Dato | Valor |
|---|---|
| Estaciones cargadas | 4 |
| Tamaño de la tabla | 31 |
| Colisiones | 0 |
| Factor de carga final | 0.129032 |

*(Se obtienen con la opción de estadísticas de la tabla del menú, o con `mostrarEstadisticasTabla()`, después de cargar `estaciones.txt`.)*

---

## 5. Consignas de análisis

Notación: **n** = cantidad de elementos de la estructura que se recorre, **m** = tamaño de la tabla hash, **V** = estaciones del grafo, **E** = ciclovías, **h** = altura del árbol.

### Sección A — Gestión de estaciones

| Punto | Complejidad |
|---|---|
| Mostrar / agregar / eliminar una estación | **O(1)** promedio, O(n) en el peor caso |
| Listar ordenadas (selección) | **O(m + n²)**, es decir, O(n²) |
| Menos del 20 % de ocupación | **O(m + n)** |
| Recorrido más corto (BFS) | **O(V + E)** |

**Tabla hash (requisito estructural).** Se eligió para encontrar una estación por su código sin recorrerlas todas: con una buena función hash y poca carga, buscar, insertar y eliminar cuestan O(1) en promedio. El encadenamiento resuelve las colisiones sin mover elementos, y el peor caso (todos los códigos en la misma posición) degrada a O(n).

**Ordenamiento (selección).** La tabla hash no guarda las estaciones en ningún orden, así que se copian los punteros a un arreglo dinámico (O(m + n)) y se ordena ese arreglo por selección, que son dos `for` anidados: O(n²). Se eligió porque es simple y alcanza para la cantidad de estaciones de una red; el costo no se nota con pocos datos.

**Baja ocupación.** Hay que revisar todas las estaciones porque la condición depende de dos campos (disponibles y capacidad), y el índice por código no ayuda con eso.

**BFS.** Explora el grafo por niveles: primero las estaciones a 1 tramo, después las de 2 tramos, etc. Por eso la primera vez que llega al destino lo hace con la menor cantidad de tramos. Se usa una lista de adyacencia para recorrer solo las ciclovías que existen, y cada estación entra una sola vez a la cola, lo que da O(V + E). El camino se reconstruye siguiendo un arreglo de "padres" desde el destino hacia el origen. Si la cola se vacía sin llegar, no existe recorrido y se informa.

### Sección B — Viajes y colas de espera

**B1. Viajes desde una estación en un rango de fechas — O(n)**, con n = cantidad de viajes.
Se recorre la lista de viajes una vez, contando los que salieron de la estación pedida y cuya fecha está en el rango. Hay que mirar todos los viajes porque se filtra por dos criterios a la vez (estación y fecha) y la lista no está ordenada por ninguno. Se eligió una lista enlazada porque agregar un viaje al final cuesta O(1) al cargar el archivo (con puntero al último nodo) y no se necesita acceso por posición. Las fechas AAAAMMDD se comparan directamente como enteros.

**B2. Viajes de un usuario y minutos pedaleados — O(n)**, con n = cantidad de viajes.
Se recorre la lista global filtrando por DNI y sumando las duraciones. Es un recorrido completo porque no hay un índice por usuario. La lista enlazada es suficiente para este caso de uso.

**B3. Bicicletas con más de X km — O(n)**, con n = cantidad de bicicletas.
Se recorre el árbol completo en inorder y se agregan a una lista las bicicletas que superan X km, por lo que el resultado sale ordenado por ID. Hay que visitar todos los nodos porque el árbol está ordenado por `biciId` y no por kilómetros, así que no se puede descartar ninguna rama. La lista guarda punteros a las bicicletas del árbol (sin copiarlas) y se libera dentro del mismo método.

**B4. Cola de espera por estación — encolar y desencolar: O(1).**
La regla del enunciado ("se le asigna al primero de la cola") es exactamente el comportamiento FIFO, por eso se usó una cola. Con una lista enlazada que guarda punteros al primero y al último nodo, encolar al final y sacar del frente no requieren recorrer nada. Cada estación tiene su propia cola (composición).

**B5. Cola de prioridad del taller (min-heap) — insertar y extraer: O(log n).**
Un heap da acceso inmediato a la bicicleta más urgente (la raíz) y mantiene el orden con pocos intercambios: al insertar, el elemento "flota" hacia arriba, y al extraer, el último "se hunde" desde la raíz. Como es un árbol completo, ambos recorridos son como máximo de altura log n. Un arreglo ordenado costaría O(n) por inserción, y una lista sin ordenar costaría O(n) por extracción. Solo se implementan las dos operaciones que pide el enunciado.

### Sección C — Árbol binario de búsqueda

| Operación | Complejidad |
|---|---|
| Insertar, buscar, eliminar | **O(h)**: O(log n) si está balanceado, O(n) en el peor caso |
| Listar inorder, contar averiadas, calcular altura, liberar (postorder) | **O(n)** |

**Pregunta de análisis: ¿qué altura tendría el árbol si las bicicletas se cargaran ordenadas por ID? ¿Cómo afecta al costo de la búsqueda?**

Si las bicicletas se cargan ya ordenadas por ID, cada nueva bicicleta es mayor que todas las anteriores y siempre se inserta como hijo derecho del último nodo. El árbol se degenera en una cadena, equivalente a una lista enlazada.

Ejemplo con 5 bicicletas, cargadas en el orden 101, 102, 103, 104, 105:

```
101
  \
  102
    \
    103
      \
      104
        \
        105          altura = 5
```

Buscar la bicicleta 105 obliga a comparar con los 5 nodos: el costo es **O(n)**, igual que recorrer una lista.

Con las mismas 5 bicicletas cargadas en el orden 103, 101, 105, 102, 104 el árbol queda balanceado:

```
        103
       /   \
     101   105
       \   /
      102 104        altura = 3
```

Acá la búsqueda necesita como máximo 3 comparaciones: **O(log n)**. Conclusión: la forma del árbol depende del orden de inserción, y con datos ordenados se pierde la ventaja del ABB. Para evitarlo harían falta árboles autobalanceados (AVL, rojo-negro), que quedan fuera del alcance del TP.

*(El método de eliminación opcional contempla los tres casos: nodo hoja, nodo con un hijo y nodo con dos hijos, usando el sucesor inorder.)*

### Sección D — Redistribución con backtracking

**[COMPLETAR por quien implementó el punto D: descripción del algoritmo y complejidad en el peor caso.]**

*(Referencia: si el algoritmo decide, para cada una de las n estaciones, si se incluye o no en el camión, explora hasta 2ⁿ subconjuntos, por lo que el peor caso es O(2ⁿ). Verificar contra la implementación real, y mencionar la poda si se usa.)*

---

## 6. Diagrama UML

El diagrama de clases (asociaciones y composiciones) está en: **[COMPLETAR: ruta del archivo, por ejemplo `docs/UML.drawio` o `docs/UML.png`]**.

Composiciones principales:
`SistemaBicicletas` ◆— `TablaHash`, `Grafo`, `MinHeap`, `AbbBicicleta`, `ListaViajes` · `TablaHash` ◆— `NodoHash` ◆— `Estacion` · `Estacion` ◆— `ColaEspera` ◆— `NodoCola` · `MinHeap` ◆— `OrdenTaller` · `Grafo` ◆— `NodoAdyacente` · `AbbBicicleta` ◆— `NodoABB` ◆— `Bicicleta` · `ListaViajes` ◆— `NodoViaje` ◆— `Viaje`.