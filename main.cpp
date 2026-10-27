#include <iostream>
#include <fstream>
#include <string>

// Header del proyecto
#include "header/SistemaBicicletas.h"
#include "header/Estacion.h"
#include "header/Viaje.h"

using namespace std;

// Carga de datos



void cargarEstaciones(SistemaBicicletas& sistema) {
    // ifstream sirve para el manejo de archivos con otras carpetas
    ifstream archivo("datos/estaciones.txt.txt"); // Le estamos diciendo al sistema que abra de la carpeta datos el archivo estaciones.txt

    if (!archivo.is_open()) { // si no abre el archivo, intentamos abrirlo de nuevo
        archivo.open("datos/estaciones.txt.txt");
    }

    if (!archivo.is_open()) { // si no abre de nuevo tiramos una advertencia y cortamos el programa
        cout << "ADVERTENCIA. No se pudo abrir el archivo de estaciones." << endl;
        return;
    }

    string codigo,nombre,barrio;
    int capacidad, bicisDisponibles;

    int cargadas = 0;

    while (archivo >> codigo >> nombre >> barrio >> capacidad >> bicisDisponibles) {
        // Cramos una instacia de estacion con los datos leidos
        Estacion estacion(codigo, nombre, barrio, capacidad, bicisDisponibles);
        // Y lo agreamos al sistema
        sistema.agregarEstacion(estacion);
        cargadas++;
    }

    archivo.close();
    cout << "Estaciones cargadas correctamente, son un total de : " << cargadas << endl;

}

void cargarViaje(SistemaBicicletas& sistema) {
    ifstream archivo("datos/viajes.txt");
    if (!archivo.is_open()) {
        archivo.open("datos/viajes.txt.txt");
    }

    if (!archivo.is_open()) {
        cout << "ADVERTENCIA. No se pudo abrir el archivo de viajes." << endl;
        return;
    }

    int idViaje, idBici, dniUsuario, fecha, duracion;
    string codOrigen, codDestino;
    int cargados = 0;

    // Pasamos directamente codOrigen y codDestino (strings)
    while (archivo >> idViaje >> idBici >> dniUsuario >> codOrigen >> codDestino >> fecha >> duracion) {
        Viaje viaje(idViaje, idBici, dniUsuario, codOrigen, codDestino, fecha, duracion);
        sistema.registrarViaje(viaje);
        cargados++;
    }

    archivo.close();
    cout << "Viajes cargados correctamente. Total: " << cargados << endl;
}


void cargarBicicletas(SistemaBicicletas& sistema) {
    ifstream archivo("datos/bicicletas.txt");
    if (!archivo.is_open()) {
        archivo.open("datos/bicicletas.txt.txt");
    }

    if (!archivo.is_open()) {
        cout << "ADVERTENCIA: No se pudo abrir el archivo de bicicletas." << endl;
        return;
    }

    int id, km;
    string codEstacion, strTipo, strEstado;
    int cargadas = 0;

    while (archivo >> id >> codEstacion >> strTipo >> strEstado >> km) {
        TipoBici tipo = (strTipo == "Electrica" || strTipo == "ELECTRICA") ? TipoBici::ELECTRICA : TipoBici::MECANICA;
        EstadoBici estado = (strEstado == "OK" || strEstado == "ok") ? EstadoBici::OK : EstadoBici::AVERIADA;

        // Se pasa esManual = false por defecto
        sistema.agregarBicicleta(id, codEstacion, tipo, estado, km, false);
        cargadas++;
    }

    archivo.close();
    cout << "Bicicletas cargadas correctamente. Total: " << cargadas << endl;
}

void cargarCiclovias(SistemaBicicletas& sistema) {
    ifstream archivo("datos/ciclovias.txt.txt");
    if (!archivo.is_open()) {
        archivo.open("datos/ciclovias.txt");
    }

    if (!archivo.is_open()) {
        cout << "ADVERTENCIA. No se pudo abrir el archivo de ciclovias." << endl;
        return;
    }

    string codOrigen, codDestino;
    int cargadas = 0;

    // Formato del txt: CAB MON
    while (archivo >> codOrigen >> codDestino) {
        sistema.agregarCiclovia(codOrigen, codDestino);
        cargadas++;
    }

    archivo.close();
    cout << "Ciclovias cargadas correctamente, son un total de : " << cargadas << endl;
}

// Interfaz

void mostrarMenu() {
    cout << "\n==========================================" << endl;
    cout << "    SISTEMA DE GESTION DE BICICLETAS     " << endl;
    cout << "==========================================" << endl;
    cout << " 1. Cargar datos desde archivos (.txt)" << endl;
    cout << " 2. Buscar viajes de un usuario (por DNI)" << endl;
    cout << " 3. Consultar minutos pedaleados por DNI" << endl;
    cout << " 4. Mostrar informacion de una estacion" << endl;
    cout << " 5. Listar estaciones ordenadas" << endl;
    cout << " 6. Listar estaciones con baja ocupacion (<20%)" << endl;
    cout << " 7. Retirar bicicleta (Gestion de Cola)" << endl;
    cout << " 8. Devolver bicicleta" << endl;
    cout << " 9. Contar viajes desde estacion entre fechas" << endl;
    cout << "10. Consultar bicicletas con mas de X Km (ABB)" << endl;
    cout << "11. Registrar bicicleta manualmente (ABB)" << endl;
    cout << "12. Agregar tramo de ciclovia manualmente (Grafo)" << endl;
    cout << "13. Registrar viaje manualmente (Lista)" << endl;
    cout << "14. Agregar estacion manualmente (Tabla Hash)" << endl;
    cout << "15. Calcular recorrido mas corto por ciclovia (BFS)" << endl;
    cout << "16. Gestion de Taller (Enviar / Atender)" << endl;
    cout << "17. Mostrar estadisticas de la Tabla Hash" << endl;
    cout << " 0. Salir" << endl;
    cout << "------------------------------------------" << endl;
    cout << "Ingrese una opcion: ";
}

int main() {
    SistemaBicicletas sistema;
    int opcion = -1;

    do {
        mostrarMenu();
        cin >> opcion;

        switch (opcion) {
            case 1: {
                cout << "\n--- CARGANDO DATOS INICIALES ---" << endl;
                cargarEstaciones(sistema);
                cargarBicicletas(sistema); // Cargamos bicis antes de viajes para validaciones
                cargarViaje(sistema);
                cargarCiclovias(sistema);
                break;
            }
            case 2: {
                int dniUsuario;
                cout << "\nIngrese DNI del usuario: ";
                cin >> dniUsuario;

                ListaViajes viajes = sistema.buscarViajesDeUsuario(dniUsuario);
                cout << "Se encontraron " << viajes.getTamanio() << " viaje(s) para el DNI " << dniUsuario << endl;

                NodoViaje* actual = viajes.getCabeza();
                while (actual != nullptr) {
                    actual->getViaje().mostrarInformacion();
                    actual = actual->getSiguiente();
                }
                break;
            }
            case 3: {
                int dniUsuario;
                cout << "\nIngrese DNI del usuario: ";
                cin >> dniUsuario;

                sistema.mostrarMinutosPedaleadosDeUsuario(dniUsuario);
                break;
            }
            case 4: {
                string codEstacion;
                cout << "\nIngrese codigo de estacion: ";
                cin >> codEstacion;

                sistema.mostrarInformacionDeEstacion(codEstacion);
                break;
            }
            case 5: {
                int seleccion;
                cout << "\n--- LISTAR ESTACIONES ORDENADAS ---" << endl;
                cout << "1. Por capacidad de anclajes" << endl;
                cout << "2. Por bicicletas disponibles" << endl;
                cout << "Seleccione una opcion: ";
                cin >> seleccion;

                bool porCapacidad = (seleccion == 1);
                sistema.listarEstacionesOrdenadas(porCapacidad);
                break;
            }
            case 6: {
                cout << "\n--- ESTACIONES CON BAJA OCUPACION (<20%) ---" << endl;
                sistema.listarEstacionesConBajaOcupacion();
                break;
            }
            case 7: {
                string cod;
                int dni;
                cout << "\n--- RETIRAR BICICLETA ---" << endl;
                cout << "Codigo de estacion: "; cin >> cod;
                cout << "DNI del usuario: "; cin >> dni;

                sistema.retirarBicicleta(cod, dni);
                break;
            }
            case 8: {
                string cod;
                cout << "\n--- DEVOLVER BICICLETA ---" << endl;
                cout << "Codigo de estacion: "; cin >> cod;

                sistema.devolverBicicleta(cod);
                break;
            }
            case 9: {
                string codigo;
                int fechaDesde, fechaHasta;
                cout << "\n--- CONTAR VIAJES ENTRE FECHAS ---" << endl;
                cout << "Codigo de estacion: ";
                cin >> codigo;
                cout << "Fecha desde (YYYYMMDD): ";
                cin >> fechaDesde;
                cout << "Fecha hasta (YYYYMMDD): ";
                cin >> fechaHasta;

                sistema.contarViajesDesdeEstacion(codigo, fechaDesde, fechaHasta);
                break;
            }
            case 10: {
                int km;
                cout << "\n--- CONSULTAR BICICLETAS POR KILOMETRAJE (ABB) ---" << endl;
                cout << "Ingrese la cantidad minima de kilometros: ";
                cin >> km;

                // Adaptado a la nueva versión que imprime y libera internamente
                sistema.listarBicicletasConXKm(km);
                break;
            }
            case 11: {
                int id, km, optTipo, optEstado;
                string codEstacion;

                cout << "\n--- REGISTRAR BICICLETA MANUALLY (ABB) ---" << endl;
                cout << "ID de la Bicicleta (int): ";
                cin >> id;
                cout << "Codigo de Estacion actual (ej: CAB): ";
                cin >> codEstacion;
                cout << "Tipo (1. Electrica / 2. Mecanica): ";
                cin >> optTipo;
                cout << "Estado (1. OK / 2. AVERIADA): ";
                cin >> optEstado;
                cout << "Kilometros recorridos: ";
                cin >> km;

                TipoBici tipo = (optTipo == 1) ? TipoBici::ELECTRICA : TipoBici::MECANICA;
                EstadoBici estado = (optEstado == 1) ? EstadoBici::OK : EstadoBici::AVERIADA;

                // Pasamos 'true' para que se sume a las bicicletas disponibles de la estacion
                sistema.agregarBicicleta(id, codEstacion, tipo, estado, km, true);
                break;
            }
            case 12: {
                string codOrigen, codDestino;

                cout << "\n--- AGREGAR TRAMO DE CICLOVIA MANUALLY (GRAFO) ---" << endl;
                cout << "Codigo Estacion Origen (ej: CAB): ";
                cin >> codOrigen;
                cout << "Codigo Estacion Destino (ej: MON): ";
                cin >> codDestino;

                sistema.agregarCiclovia(codOrigen, codDestino);
                break;
            }
            case 13: {
                int idViaje, idBici, dniUsuario, fecha, duracion;
                string codOrigen, codDestino;

                cout << "\n--- REGISTRAR VIAJE MANUALLY ---" << endl;
                cout << "ID del Viaje: "; cin >> idViaje;
                cout << "ID de la Bicicleta: "; cin >> idBici;
                cout << "DNI del Usuario: "; cin >> dniUsuario;
                cout << "Codigo Estacion Origen (ej: CAB): "; cin >> codOrigen;
                cout << "Codigo Estacion Destino (ej: MON): "; cin >> codDestino;
                cout << "Fecha (YYYYMMDD): "; cin >> fecha;
                cout << "Duracion (minutos): "; cin >> duracion;

                Viaje nuevoViaje(idViaje, idBici, dniUsuario, codOrigen, codDestino, fecha, duracion);
                sistema.registrarViaje(nuevoViaje);
                break;
            }
            case 14: {
                string codigo, nombre, barrio;
                int capacidad, bicisDisponibles;

                cout << "\n--- AGREGAR ESTACION MANUALLY ---" << endl;
                cout << "Codigo de Estacion (ej: PAL): "; cin >> codigo;
                cout << "Nombre de Estacion: "; cin >> nombre;
                cout << "Barrio: "; cin >> barrio;
                cout << "Capacidad de Anclajes: "; cin >> capacidad;
                cout << "Bicicletas Disponibles: "; cin >> bicisDisponibles;

                Estacion nuevaEstacion(codigo, nombre, barrio, capacidad, bicisDisponibles);
                sistema.agregarEstacion(nuevaEstacion);
                break;
            }
            case 15: {
                string codOrigen, codDestino;
                cout << "\n--- RECORRIDO MAS CORTO POR CICLOVIA ---" << endl;
                cout << "Codigo Estacion Origen: "; cin >> codOrigen;
                cout << "Codigo Estacion Destino: "; cin >> codDestino;

                sistema.calcularRecorridoPorCiclovia(codOrigen, codDestino);
                break;
            }
            case 16: {
                int subOpcion;
                cout << "\n--- GESTION DE TALLER ---" << endl;
                cout << "1. Enviar bicicleta averiada al taller" << endl;
                cout << "2. Atender bicicleta mas urgente" << endl;
                cout << "Seleccione una opcion: "; cin >> subOpcion;

                if (subOpcion == 1) {
                    int idBici;
                    cout << "Ingrese ID de la bicicleta: "; cin >> idBici;
                    sistema.enviarBicicletaATaller(idBici);
                } else if (subOpcion == 2) {
                    sistema.atenderTaller();
                } else {
                    cout << "Opcion invalida." << endl;
                }
                break;
            }
            case 17: {
                cout << "\n--- ESTADISTICAS DE LA TABLA HASH ---" << endl;
                sistema.mostrarEstadisticasTabla();
                break;
            }
            case 0:
                cout << "\nSaliendo del sistema... ¡Hasta luego!" << endl;
                break;
            default:
                cout << "\nOpcion invalida. Intente de nuevo." << endl;
                break;
        }

    } while (opcion != 0);

    return 0;
}