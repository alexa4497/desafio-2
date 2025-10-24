/*#ifndef LISTAFAVORITOS_H
#define LISTAFAVORITOS_H

#include "cancion.h"

#include <iostream>
#include <string>

using namespace std;


class ListaFavoritos {
private:
    static const int MAX_CANCIONES = 10000;
    Cancion* canciones[MAX_CANCIONES];
    int numCanciones;

    int buscarCancionEnFavoritos(int idCancion);
    void editar_menu();

public:

    ListaFavoritos();
    ~ListaFavoritos();

    void gestionar_listas();

    void agregarCancion(int idCancion, Cancion** dbCanciones, int numDB);
    void quitarCancion(int idCancion);

    void seguir_otra_lista();
    void ejecutar_lista();
};

#endif
*/
