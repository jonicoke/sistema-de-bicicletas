# Sistema de Bicicletas Públicas

**Integrantes:** Alan González y Jonathan Giacomini  

**Materia:** Algoritmos - Comisión 1 - 2026  

**Universidad:** Universidad Nacional de Hurlingham (UNAHUR)

Sistema de gestión de bicicletas públicas desarrollado en **C++17** con programación orientada a objetos, memoria dinámica y estructuras de datos implementadas manualmente. Permite gestionar estaciones, bicicletas, viajes, colas de espera, rutas de ciclovías, reparaciones y propuestas de redistribución mediante backtracking.

Se implementan una **tabla hash con encadenamiento**, **listas y colas enlazadas**, **min-heap**, **árbol binario de búsqueda (ABB)** y **grafo con listas de adyacencia**. No se utilizan `vector`, `map`, `unordered_map`, `queue`, `priority_queue` ni `set` para implementar estas estructuras. Se utiliza `std::string` y la biblioteca estándar para entrada/salida y operaciones auxiliares permitidas.

**---**

## 1. Compilación y ejecución

Requiere `g++` con soporte para C++17. Ejecutar desde la raíz del proyecto:

```bash

g++ -std=c++17 -Wall -Wextra -g -O0 main.cpp cpp/*.cpp -o programa

./programa

```

El proyecto emplea rutas relativas (`datos/...`). Por eso debe ejecutarse desde la raíz, donde están `main.cpp` y la carpeta `datos/`.

### Estructura

```text

TP/

├── main.cpp               # Menú y lectura de datos

├── header/                # Declaraciones .h

├── cpp/                   # Implementaciones .cpp

├── datos/                 # TXT de entrada

└── README.md

```

### Archivos de entrada

Cada línea contiene un registro con campos separados por espacios:

| Archivo | Formato |

|---|---|

| Estaciones | `codigo nombre barrio capacidad_anclajes bicis_disponibles` |

| Bicicletas | `bici_id codigo_estacion tipo estado kilometros` |

| Viajes | `viaje_id bici_id dni_usuario estacion_origen estacion_destino fecha duracion_min` |

| Ciclovías | `estacion_a estacion_b` |

Las fechas de viajes se almacenan como enteros `AAAAMMDD`. Los tipos de bicicleta son `Mecanica` y `Electrica`, y sus estados son `OK` y `Averiada` (el lector también reconoce las variantes utilizadas en los datos de prueba).

**Orden de carga:** estaciones → bicicletas → viajes → ciclovías. Se cargan primero las estaciones porque la inserción de bicicletas y ciclovías comprueba que existan. En la carga inicial, las bicicletas no vuelven a incrementar los contadores `bicisDisponibles` leídos del archivo de estaciones (`esManual = false`).

### Operaciones disponibles en el menú

El menú permite cargar archivos, consultar viajes y minutos por DNI, mostrar/agregar/eliminar estaciones, ordenar por capacidad o disponibilidad, detectar baja ocupación, administrar retiros y devoluciones, consultar bicicletas por kilómetros, insertar y buscar bicicletas, recorrer el ABB en orden, contar averiadas, consultar la altura del árbol, administrar ciclovías, calcular rutas BFS, registrar viajes, gestionar el taller, mostrar estadísticas hash y ejecutar redistribución por backtracking.

**---**

## 2. Módulos

| Módulo | Responsabilidad |

|---|---|

| `Estacion` / `ColaEspera` | Datos de estación y cola FIFO independiente de usuarios por estación; retirar, devolver y calcular baja ocupación. |

| `TablaHash` / `NodoHash` | Índice de estaciones por código mediante encadenamiento; contabiliza colisiones y factor de carga. |

| `Grafo` / `NodoAdyacente` | Red de ciclovías bidireccionales mediante listas de adyacencia; BFS para minimizar cantidad de tramos. |

| `ColaEnteros` | Cola circular de índices de vértices utilizada por BFS. |

| `MinHeap` / `OrdenTaller` | Órdenes de reparación con prioridades 1 a 3; inserción y extracción de la más urgente. |

| `Bicicleta` | Información individual, tipo, estado y kilometraje; permite actualizar el estado después de una reparación (según la versión actualizada del código). |

| `AbbBicicleta` / `NodoABB` | Registro por `biciId`; inserción, búsqueda, recorrido inorder, conteos, altura y eliminación opcional. |

| `NodoLista` | Lista auxiliar de referencias a bicicletas filtradas por kilometraje. Sus nodos no son propietarios de las bicicletas. |

| `Viaje` / `NodoViaje` / `ListaViajes` | Historial de viajes en lista enlazada con inserción al final y copia profunda. |

| `EstacionFaltante` | Estructura auxiliar con código de estación y faltante de bicicletas para el backtracking. |

| `SistemaBicicletas` | Coordinador de las estructuras y operaciones del sistema. |

| `main.cpp` | Lectura de los cuatro TXT y menú interactivo para probar funcionalidades. |

### Funcionalidades y métodos principales

| Apartado | Funcionalidad | Método |

|---|---|---|

| A | Consultar, agregar, eliminar estación | `mostrarInformacionDeEstacion`, `agregarEstacion`, `eliminarEstacion` |

| A | Ordenar y detectar baja ocupación | `listarEstacionesOrdenadas`, `listarEstacionesConBajaOcupacion` |

| A | Ruta con menor cantidad de tramos | `calcularRecorridoPorCiclovia` |

| A | Estadísticas hash | `mostrarEstadisticasTabla` |

| B.1 | Viajes desde una estación entre fechas | `contarViajesDesdeEstacion` |

| B.2 | Viajes y minutos por DNI | `mostrarViajesDeUsuario`, `mostrarMinutosPedaleadosDeUsuario` |

| B.3 | Bicicletas con más de X km | `listarBicicletasConXKm` |

| B.4 | Cola de espera | `retirarBicicleta`, `devolverBicicleta` |

| B.5 | Taller | `enviarBicicletaATaller`, `atenderTaller` |

| C | Insertar, buscar, listar, contar averiadas y altura | `agregarBicicleta`, `buscarBicicleta`, `listarBicicletasOrdenadas`, `mostrarCantidadAveriadas`, `mostrarAlturaABB` |

| D | Redistribución | `redistribuirBicicletasBacktracking` |

**---**

## 3. Decisiones de diseño

- **Tabla hash y grafo:** las estaciones se indexan por código; al agregarlas también se crea un vértice del grafo y al eliminarlas se dan de baja sus conexiones. Las ciclovías son bidireccionales; se ignoran los duplicados.

- **Historial:** `Viaje` guarda códigos de estaciones como `string` y el ID de bicicleta, en lugar de punteros a objetos. Así el historial no depende de la permanencia de esas estaciones en memoria.

- **Disponibilidad:** `bicisDisponibles` es un contador por estación, separado del registro individual en el ABB. Retirar una bicicleta disminuye el contador; si es cero, el DNI se encola. Al devolverla, se asigna al primer usuario en espera o se incrementa la disponibilidad si no hay espera y hay capacidad.

- **Baja ocupación:** se comprueba `bicisDisponibles * 100 < capacidadAnclajes * 20`. Una estación exactamente al 20 % no se incluye.

- **Prioridades del taller:** las bicicletas eléctricas averiadas tienen prioridad 1. Para las mecánicas averiadas: más de 3000 km → 1; más de 1500 km → 2; hasta 1500 km → 3. Es una regla propia porque la consigna no especifica los criterios de las mecánicas.

- **Duplicados del taller:** se recorre el arreglo del heap para evitar insertar la misma bicicleta dos veces. Tras extraer una orden, el sistema busca la bicicleta en el ABB y actualiza su estado a `OK` (si está disponible en el ABB).

- **Memoria y copias:** `ColaEspera` y `ListaViajes` implementan copia profunda. Las estructuras que administran memoria dinámica sin semántica de copia (`TablaHash`, `Grafo`, `MinHeap`, `AbbBicicleta`, `ColaEnteros`, `SistemaBicicletas`) deshabilitan la copia.

- **Backtracking:** se calculan automáticamente los faltantes positivos como `capacidadAnclajes - bicisDisponibles`. Para cada estación se exploran las dos decisiones: incluirla o no. Se imprimen los subconjuntos cuyo faltante total coincide exactamente con la capacidad `C` del camión. El método **no modifica** el estado de las estaciones ni redistribuye físicamente las bicicletas: calcula combinaciones posibles.

**---**

## 4. Tabla hash: colisiones y factor de carga

- **Función hash:** polinómica sobre el código de estación: `h = h * 31 + caracter`, y luego `h % tamanioTabla`.
- **Tamaño:** 37 posiciones, inicializadas desde `SistemaBicicletas` con `new TablaHash(37)`.
- **Resolución de colisiones:** encadenamiento separado mediante listas enlazadas. La inserción en una posición ocupada incrementa el contador de colisiones en una unidad; el contador es acumulativo y no disminuye al eliminar estaciones.
- **Factor de carga:** `cantidadDeEstaciones / tamanioTabla`.

### Resultados con los archivos de prueba actuales

Se utilizaron las 12 estaciones de `estaciones.txt`, cargadas una sola vez, sin altas ni bajas posteriores:

| Dato | Valor |
|---|---:|
| Estaciones cargadas | 12 |
| Tamaño de la tabla | 37 |
| Colisiones | 1 |
| Factor de carga final | 0,324324 (aprox.) |
| Porcentaje de carga | 32,43 % |

La colisión se produce entre **MON** y **NUN**, que generan el índice **8**. El encadenamiento permite almacenar y buscar ambas estaciones sin perder información.

Estos valores se calcularon con la función hash y el conjunto actual de códigos; deben contrastarse con la **opción 22** del menú. Si se modifica el conjunto de estaciones o se vuelve a cargar, las estadísticas pueden variar.

---

## 5. Consignas de análisis: complejidad temporal

**Notación:** `n` = cantidad de elementos relevantes de la estructura analizada; `m` = tamaño de la tabla hash (37 en la configuración actual); `V` = vértices activos/registrados del grafo; `E` = aristas o conexiones; `h` = altura del ABB; `t` = órdenes pendientes en el taller; `k` = estaciones con faltantes positivos. Se consideran operaciones elementales de comparación y acceso de cadenas de longitud acotada; si los códigos varían mucho de longitud, también hay que considerar ese costo.

### A. Gestión de estaciones

| Operación | Complejidad | Explicación |

|---|---|---|

| Buscar / mostrar estación en hash | O(1) promedio; O(n) peor caso | Las colisiones pueden concentrar elementos en una cadena. |

| Agregar estación al sistema | O(n + V) peor caso | Inserta en la tabla y verifica/agrega el vértice con una búsqueda lineal en `Grafo::indiceDe`; una ampliación del grafo puede copiar sus arreglos. |

| Eliminar estación del sistema | O(n + V + E) cota conservadora | Elimina del hash y del grafo; quitar un vértice implica eliminar conexiones inversas recorriendo listas. |

| Listar ordenadas por selección | O(m + n²) | Recorre buckets, arma arreglo y aplica dos ciclos de selección. |

| Listar con menos de 20 % | O(m + n) | Recorre las posiciones de la tabla y filtra todas las estaciones. |

| Calcular recorrido más corto (BFS) | O(V + E) | Se visitan vértices y aristas como máximo una vez en el recorrido, con listas de adyacencia. |

**Justificación.** La tabla hash permite consultar estaciones rápidamente por código, con resolución de colisiones mediante encadenamiento separado. El grafo representa las ciclovías y BFS encuentra el camino con menor número de tramos en una red no ponderada. El ordenamiento por selección se implementa manualmente porque la tabla hash no mantiene orden por capacidad ni disponibilidad.

**Matiz:** la cota O(V + E) corresponde a la búsqueda BFS y a las búsquedas lineales de índices incluidas en `caminoMasCorto`; no es una afirmación de O(1) para todas las operaciones del grafo.

### B. Viajes, colas y taller

| Operación | Complejidad | Explicación |

|---|---|---|

| B.1 — Contar viajes por estación y fechas | O(n) | Recorre la lista global una vez. |

| B.2 — Buscar/mostrar viajes de DNI | O(n) | Filtra la lista global y muestra los coincidientes. |

| B.2 — Sumar minutos de DNI | O(n) | Filtra los viajes en una lista temporal y recorre esa lista para sumar. |

| B.3 — Bicicletas con más de X km | O(n) | Recorre todo el ABB: está ordenado por ID, no por kilómetros. |

| B.4 — Encolar/desencolar DNI | O(1) | Cola enlazada con punteros a primero y último. |

| B.5 — Insertar en min-heap **con control de duplicados** | **O(t)** peor caso | Recorre el heap buscando el ID, después aplica `flotar` en O(log t). |

| B.5 — Extraer orden más urgente | O(log t) | Se quita la raíz y se restaura el heap con `hundir`. |

| B.5 — Atender taller (operación completa del sistema) | O(log t + h) | Extrae del heap, busca la bicicleta en el ABB y actualiza su estado. |

**Justificación.** La lista enlazada de viajes permite registrar al final en O(1) (sin contar la comprobación de ID duplicado que realiza `registrarViaje`, O(n)). La cola de espera es FIFO porque debe atender al primer usuario que llegó. El min-heap coloca la orden más urgente en la raíz y ofrece inserción/extracción O(log t) **para las operaciones puras de heap**, pero el control lineal de duplicados aumenta el costo de inserción real a O(t).

**Aclaración:** `enviarBicicletaATaller()` también busca la bicicleta en el ABB antes de insertarla; la operación completa cuesta O(h + t) en el peor caso.

### C. Árbol binario de búsqueda

| Operación | Complejidad |

|---|---|

| Insertar, buscar o eliminar por `biciId` | O(h); O(log n) si el árbol está equilibrado, O(n) en el peor caso |

| Recorrido inorder | O(n) |

| Contar averiadas | O(n) |

| Calcular altura | O(n) |

| Filtrar bicicletas por kilometraje | O(n) |

| Liberar memoria (postorder) | O(n) |

**Ejemplo solicitado.** Si las bicicletas llegan ordenadas por ID (101, 102, 103, 104, 105), el ABB queda degenerado hacia la derecha:

```text

101

  \\

  102

    \\

    103

      \\

      104

        \\

        105

```

La altura es **5** porque la implementación cuenta nodos (árbol vacío = 0). Buscar 105 requiere hasta cinco comparaciones, y la búsqueda se degrada a O(n). Si se insertan en el orden 103, 101, 105, 102, 104, la altura resulta 3 y la búsqueda puede realizarse en O(log n). Para garantizar balance en general haría falta un ABB autobalanceado, que no se solicita. La eliminación de nodos hoja, con un hijo y con dos hijos fue implementada como funcionalidad opcional.

### D. Redistribución por backtracking

**Entrada utilizada:** capacidad positiva `C` del camión y conjunto de estaciones almacenadas en el sistema. Para cada estación se calcula automáticamente el faltante positivo `capacidadAnclajes - bicisDisponibles`; las estaciones llenas se omiten.

**Salida:** todos los subconjuntos de estaciones cuyos faltantes suman exactamente `C`. Si ninguno existe, se informa.

**Algoritmo:** `redistribuirBicicletasBacktracking()` obtiene las estaciones de la tabla hash y construye un arreglo temporal de `EstacionFaltante`. La función auxiliar recursiva `backtrackingRedistribucion()` evalúa, para cada estación, incluirla (restando su faltante) o excluirla (dejando la capacidad restante intacta). Si la capacidad restante llega a 0, imprime la solución; si se vuelve negativa o se agotan las estaciones, corta esa rama. Se liberan los arreglos temporales al finalizar.

**Complejidad temporal:** con `k` estaciones candidatas, la búsqueda explora hasta **O(2^k)** ramas/estados en el peor caso. Si se contabiliza el costo de imprimir todos los subconjuntos encontrados (hasta `k` nombres por solución), una cota superior total es **O(k·2^k)**, además de O(m + n) para obtener y filtrar las estaciones. **Espacio auxiliar:** O(k), por la profundidad recursiva y los arreglos de candidatos y solución. Los faltantes positivos permiten podar ramas cuya suma supera `C`.

**Ejemplo de prueba independiente:** si `C = 10` y los faltantes son `PLZ=3`, `CAB=7`, `RET=5`, `CON=2`, `BOE=5`, deben aparecer `{PLZ, CAB}`, `{RET, BOE}`, `{PLZ, RET, CON}` y `{PLZ, CON, BOE}` (sin importar el orden de impresión).

**Limitación del diseño:** a diferencia de una interfaz que pide una lista arbitraria de faltantes, esta versión deriva los faltantes de las estaciones existentes. El backtracking solo calcula combinaciones; no registra movimientos de bicicletas ni crea una clase `Camion`.

**---**

## 6. Verificación funcional y memoria

### Casos de prueba recomendados

1\. Cargar los cuatro TXT una vez y comprobar que cada estructura contiene los registros esperados.

2\. Mostrar la tabla hash, buscar estaciones existentes/inexistentes y provocar una colisión controlada; verificar que la búsqueda siga funcionando.

3\. Listar estaciones por cada criterio; probar las de ocupación menor que 20 % y exactamente igual al 20 %.

4\. Buscar rutas BFS: origen = destino, ruta existente, estaciones desconectadas y estación inexistente.

5\. Buscar viajes de un DNI con varios registros y uno sin viajes; comprobar total de minutos y filtro de fechas.

6\. Retirar hasta agotar una estación; agregar dos DNIs a la cola, devolver dos bicicletas y verificar el orden FIFO.

7\. Buscar bicicleta por ID, listar inorder, contar averiadas, calcular altura y filtrar por kilómetros.

8\. Enviar una bicicleta averiada al taller dos veces: la segunda inserción debe rechazarse. Atenderla, verificar que cambie a estado `OK` y que ya no se pueda enviar como averiada.

9\. Ejecutar backtracking con `C` que produzca varias soluciones y con `C` sin soluciones; comprobar que la capacidad total de cada subconjunto sea exactamente `C`.

10\. Salir normalmente para ejecutar destructores; repetir con Valgrind.

### Compilación de diagnóstico

```bash

g++ -std=c++17 -Wall -Wextra -Wpedantic -g -O0 main.cpp cpp/*.cpp -o programa

```

Los requisitos de entrega piden que no haya advertencias con `-Wall -Wextra`. Se agrega `-Wpedantic` como comprobación adicional.

### Valgrind

Desde Linux o WSL, ejecutar el binario desde la raíz del proyecto:

```bash
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --error-exitcode=1 ./programa
```

**Resultado registrado en la ejecución más reciente:**

```text
==753== HEAP SUMMARY:
==753==     in use at exit: 0 bytes in 0 blocks
==753==   total heap usage: 169 allocs, 169 frees, 121,594 bytes allocated
==753==
==753== All heap blocks were freed -- no leaks are possible
==753==
==753== For lists of detected and suppressed errors, rerun with: -s
==753== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```

**Conclusión:** en esa ejecución, Valgrind no detectó errores de memoria ni bloques sin liberar. El número de reservas fue igual al de liberaciones (169). El resultado **no garantiza** que estén cubiertas todas las opciones del menú: para ampliar la verificación, conviene repetirlo usando el ABB, el taller, las colas, el grafo y el backtracking, terminando con la opción `0`.

---

### Verificación adicional (opcional)

```bash

g++ -std=c++17 -Wall -Wextra -g -O1 -fsanitize=address,undefined -fno-omit-frame-pointer main.cpp cpp/*.cpp -o programa_san

./programa_san

```



## 7. Diagrama de clases UML

El diagrama de clases actualizado incluye asociaciones, composiciones y dependencias para `SistemaBicicletas`, la tabla hash, el grafo, las colas, el min-heap, el ABB, el historial de viajes y la estructura auxiliar `EstacionFaltante` del backtracking.

**Archivo:** agregar aquí la ruta real del UML dentro del repositorio; por ejemplo, `docs/DIAGRAMA_UML.pdf` **solo si ese es el nombre y la ubicación utilizados**.

Las relaciones de propiedad reflejan el ciclo de vida: `SistemaBicicletas` libera sus estructuras dinámicas; `TablaHash` administra nodos con estaciones; `Estacion` contiene su cola de espera; `AbbBicicleta` administra sus nodos y bicicletas; `ListaViajes` administra sus nodos con viajes. La relación de `SistemaBicicletas` a `EstacionFaltante` es una **dependencia temporal**, porque se utiliza solamente durante la redistribución.


## 8. Observaciones y alcance

- El sistema gestiona la disponibilidad de bicicletas mediante contadores en `Estacion`, pero no registra individualmente cada retiro/devolución en el ABB.

- El backtracking propone subconjuntos de estaciones; no ejecuta la redistribución real.

- El arreglo del min-heap se ordena por prioridad, no por ID; por eso la detección de duplicados requiere un recorrido lineal.

- El histórico de viajes se conserva mediante IDs y códigos de estación, aun si se elimina alguna estación.

- Las estadísticas hash corresponden al conjunto actual de 12 estaciones; el registro de Valgrind corresponde a una ejecución concreta y debe repetirse si cambia el código.
