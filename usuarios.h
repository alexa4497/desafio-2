#ifndef USUARIOS_H
#define USUARIOS_H

#include <fstream>
#include <string>
#include <iostream>
#include <sstream>


using namespace std;

class Cancion;

class Usuario {
private:
    string nickname;
    string membresia;
    string ciudad;
    string pais;
    string fechaInscripcion;

public:

    Usuario();

    Usuario(const string& n, const string& m, const string& c, const string& p);

    bool esPremium() const;
    void mostrarPublicidadSiAplica();
    static const int MAX_FAVORITOS = 10000;
    Cancion* listaFavoritos[MAX_FAVORITOS];
    int numFavoritos;

    void agregarFavorito(Cancion* c);
    void seguirLista(Usuario* otroUsuarioPremium);
    bool iniciarSesion(Cancion* catalogoCanciones, int tamanoCatalogo);
};

#endif
