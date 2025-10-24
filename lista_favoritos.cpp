/*
#include "Lista_favoritos.h"
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;


ListaFavoritos::ListaFavoritos() : numCanciones(0) {
    for (int i = 0; i < MAX_CANCIONES; ++i) {
        canciones[i] = nullptr;
    }
}


ListaFavoritos::~ListaFavoritos() {

}


int ListaFavoritos::buscarCancionEnFavoritos(const string& idCancion) {
    for (int i = 0; i < numCanciones; ++i) {
        if (canciones[i]->getIdentificador() == idCancion) {
            return i;
        }
    }
    return -1;
}


Cancion* buscarCancionEnDB(const string& idCancion, Cancion* dbCanciones, int numDB) {
    for (int i = 0; i < numDB; ++i) {
        if (dbCanciones[i].getIdentificador() == idCancion) {
            cout << "\n--- CANCION ENCONTRADA: " << dbCanciones[i].getNombre() << " ---" << endl;
            return &dbCanciones[i];
        }
    }
    return nullptr;
}


void ListaFavoritos::editar_menu(Cancion* dbCanciones, int numDB) {
    int opcion = 0;
    string idCancion;

    do {
        cout << "\n--- SUBMENU: EDITAR MI LISTA ---" << endl;
        cout << "1. Agregar cancion por ID" << endl;
        cout << "2. Quitar cancion por ID" << endl;
        cout << "3. Volver al menu anterior" << endl;
        cout << "Ingrese su opcion: ";

        if (!(cin >> opcion)) {
            cout << "Entrada invalida. Intente de nuevo." << endl;
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        switch (opcion) {
        case 1:
            cout << "Ingrese el ID de la cancion que desea agregar: ";
            if (cin >> idCancion) {
                agregarCancion(idCancion, dbCanciones, numDB);
            } else {
                cout << "ID invalido." << endl;
                cin.clear();
                cin.ignore(10000, '\n');
            }
            break;
        case 2:
            cout << "Ingrese el ID de la cancion que desea quitar: ";
            if (cin >> idCancion) {
                quitarCancion(idCancion);
            } else {
                cout << "ID invalido." << endl;
                cin.clear();
                cin.ignore(10000, '\n');
            }
            break;
        case 3:
            cout << "Volviendo al menu principal de gestion..." << endl;
            break;
        default:
            cout << "Opcion no reconocida. Intente de nuevo." << endl;
            break;
        }

        if (opcion != 3) {
            cout << "\nPresione ENTER para continuar...";
            cin.ignore();
            cin.get();
        }

    } while (opcion != 3);
}

void ListaFavoritos::gestionar_listas(Cancion* dbCanciones, int numDB) {
    int opcion = 0;

    do {
        cout << "\n=======================================" << endl;
        cout << "   GESTOR DE LISTAS DE REPRODUCCIÓN    " << endl;
        cout << "=======================================" << endl;
        cout << "1. Reproducir mi lista de favoritos" << endl;
        cout << "2. Seguir otra lista de un usuario Premium" << endl;
        cout << "3. Editar/Modificar mi lista de favoritos" << endl;
        cout << "4. Volver al menu principal" << endl;
        cout << "Ingrese su opcion: ";

        if (!(cin >> opcion)) {
            cout << "Entrada invalida. Por favor, ingrese un numero." << endl;
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        switch (opcion) {
        case 1:
            ejecutar_lista();
            break;
        case 2:
            seguir_otra_lista();
            break;
        case 3:
            editar_menu(dbCanciones, numDB);
            break;
        case 4:
            cout << "Saliendo del gestor de listas..." << endl;
            break;
        default:
            cout << "Opcion no valida. Intente de nuevo." << endl;
            break;
        }

        if (opcion != 4) {
            cout << "\nPresione ENTER para continuar...";
            cin.ignore();
            cin.get();
        }

    } while (opcion != 4);
}

void ListaFavoritos::agregarCancion(const string& idCancion, Cancion* dbCanciones, int numDB) {
    if (buscarCancionEnFavoritos(idCancion) != -1) {
        cout << "Mensaje: La cancion con ID " << idCancion << " ya esta en tu lista de favoritos." << endl;
        return;
    }

    Cancion* cancionDB = buscarCancionEnDB(idCancion, dbCanciones, numDB);

    if (cancionDB != nullptr) {
        char opcion;
        cout << "\n¿Desea agregarla a su lista de favoritos (S/N)? ";
        cin >> opcion;

        if (opcion == 'S' || opcion == 's') {
            if (numCanciones < MAX_CANCIONES) {
                canciones[numCanciones] = cancionDB;
                numCanciones++;

                cout << "Mensaje: Cancion agregada a la lista de favoritos." << endl;
            } else {
                cout << "Error: La lista de favoritos esta llena (maximo " << MAX_CANCIONES << ")." << endl;
            }
        } else {
            cout << "Mensaje: Cancion no agregada." << endl;
        }
    } else {
        cout << "Mensaje: La cancion con ID " << idCancion << " no fue encontrada en la base de datos." << endl;
    }
}

void ListaFavoritos::quitarCancion(const string& idCancion) {
    int posicion = buscarCancionEnFavoritos(idCancion);

    if (posicion != -1) {
        cout << "\n--- ELIMINAR DE FAVORITOS ---" << endl;
        cout << "Cancion a eliminar: " << canciones[posicion]->getNombre() << " (ID: " << idCancion << ")" << endl;

        for (int i = posicion; i < numCanciones - 1; ++i) {
            canciones[i] = canciones[i + 1];
        }
        numCanciones--;
        canciones[numCanciones] = nullptr;

        cout << "Cancion con ID " << idCancion << " eliminada de la lista." << endl;
    } else {
        cout << "Error: La cancion con ID " << idCancion << " no se encuentra en tu lista de favoritos." << endl;
    }
}


void ListaFavoritos::seguir_otra_lista() {
    cout << "Funcionalidad 'seguir_otra_lista' pendiente de implementacion." << endl;
}

void ListaFavoritos::ejecutar_lista() {
    cout << "Funcionalidad 'ejecutar_lista' pendiente de implementacion." << endl;
}
*/
