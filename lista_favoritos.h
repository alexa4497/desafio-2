#ifndef LISTAFAVORITOS_H
#define LISTAFAVORITOS_H

#include "Canciones.h"
#include <iostream>
#include <string>

using namespace std;

class ListaFavoritos {
private:
    static const int MAX_CANCIONES = 10000;
    Cancion* canciones[MAX_CANCIONES];
    int numCanciones;

    int buscarCancionEnFavoritos(const string& idCancion);
    string usuarioPropietario;

public:
    ListaFavoritos();
    ~ListaFavoritos();

    int getNumCanciones() const { return numCanciones; } // Getter necesario para métricas

    void agregarCancion(const string& idCancion, Cancion* dbCanciones, int numDB);
    void quitarCancion(const string& idCancion);

    void seguir_otra_lista(Cancion* catalogoCanciones, int tamanoCatalogo);

    void ejecutar_lista(const string& membresia);
    void guardar_favoritos_a_archivo();

    void cargar_favoritos_desde_archivo(const std::string& nicknameUsuario, Cancion* catalogoCanciones, int tamanoCatalogo, int indiceUsuario);
    void editar_menu(Cancion* dbCanciones, int numDB, const string& membresia, int numUsuariosTotal);
    // 2. Firma corregida
    void gestionar_listas(Cancion* dbCanciones, int numDB, const string& membresia, int numUsuariosTotal);

};


#endif
