#ifndef USUARIOS_H
#define USUARIOS_H

#include <string>
#include "Canciones.h"
#include "lista_favoritos.h"
#include "publicidad.h"


using namespace std;

class Usuario {
public:
    string nickname;
    string membresia;
    string ciudad;
    string pais;
    string fechaInscripcion;
    ListaFavoritos miListaFavoritos;

public:
    Usuario();

    bool iniciarSesion(Cancion* catalogoCanciones, int tamanoCatalogo, Usuario* dbUsuarios, int numUsuariosTotal);

    const std::string& getNickname() const { return nickname; }


};

Usuario* cargarUsuariosDesdeArchivo(const std::string& ruta, int& numUsuarios);

#endif // USUARIOS_H
