#include "funciones_librerias.h"
int ListaFavoritos::buscarCancionEnFavoritos(int idCancion) {
    for (int i = 0; i < numCanciones; ++i) {

        if (canciones[i]->getId() == idCancion) {
            return i;
        }
    }
    return -1;
}

void ListaFavoritos::agregarCancion(int idCancion) {

    if (buscarCancionEnFavoritos(idCancion) != -1) {
        cout << "Mensaje: La cancion con ID " << idCancion << " ya esta en tu lista de favoritos." << endl;
        return;
    }

    Cancion* nuevaCancion = buscarYCargarCancionDesdeTXT(idCancion);

    if (nuevaCancion) {

        char opcion;
        cout << "\nSe encontro la cancion. ¿Desea agregarla a su lista de favoritos (S/N)? ";
        cin >> opcion;

        if (opcion == 'S' || opcion == 's') {
            if (numCanciones < MAX_CANCIONES) {
                canciones[numCanciones] = nuevaCancion;
                numCanciones++;

                cout << "Mensaje: Cancion agregada a la lista de favoritos." << endl;
            } else {

                delete nuevaCancion;
                cout << "Error: La lista de favoritos esta llena (maximo " << MAX_CANCIONES << ")." << endl;
            }
        } else {
            delete nuevaCancion;
            cout << "Mensaje: Cancion no agregada." << endl;
        }
    } else {
        cout << "Mensaje: La cancion con ID " << idCancion << " no fue encontrada en la base de datos." << endl;
    }
}

void ListaFavoritos::editar_mi_lista_de_favoritos() {
    int opcion = 0;
    int idCancion = 0;

    do {
        cout << "\n--- EDITAR LISTA DE FAVORITOS ---" << endl;
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
                agregarCancion(idCancion);
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
            cout << "Volviendo al menu principal..." << endl;
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

#ifndef LISTAFAVORITOS_H
#define LISTAFAVORITOS_H

#include "funciones_librerias.h"

class ListaFavoritos {
private:
    static const int MAX_CANCIONES = 10000;
    Cancion* canciones[MAX_CANCIONES];
    int numCanciones;

public:
    ListaFavoritos();
    ~ListaFavoritos();
    void editar_mi_lista_de_favoritos();
    void seguir_otra_lista();
    void ejecutar_lista();
    void agregarCancion(int idCancion); // <--- ESTE SE MANTIENE
    void quitarCancion(int idCancion);
};

#endif
