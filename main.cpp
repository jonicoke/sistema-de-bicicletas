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
    ifstream archivo("datos/viajes.txt.txt");

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

    while (archivo >> idViaje >> idBici >> dniUsuario >> codOrigen >> codDestino >> fecha >> duracion) {
        Viaje viaje(idViaje, idBici, dniUsuario, codOrigen, codDestino, fecha, duracion);
        sistema.registrarViaje(viaje);
        cargados++;
    }

    archivo.close();
    cout << "Viajes cargados correctamente, son un total de : " << cargados << endl;
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
    cout << " 6. Listar estaciones con baja ocupacion" << endl;
    cout << " 7. Retirar bicicleta (Gestion de Cola)" << endl;
    cout << " 8. Devolver bicicleta" << endl;
    cout << " 9. Contar viajes desde estacion entre fechas" << endl;
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
                cargarViaje(sistema);
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