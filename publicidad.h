#ifndef PUBLICIDAD_H
#define PUBLICIDAD_H

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <sstream>

using namespace std;

// ELIMINA la constante y usa string directo
const int MAX_ADS = 50;

struct MensajePublicitario {
    int id;
    string contenido;
    string categoria;
    int prioridad;
};

class Publicidad {
private:
    MensajePublicitario dbPublicidad[MAX_ADS];
    int numMensajes;
    int ultimoIndiceMostrado;
    int contadorCanciones;

    void cargarMensajesDesdeArchivo();
    int seleccionarIndicePonderado();
    string obtenerCategoriaTexto(int prioridad);
    string trim(const string& str);

public:
    Publicidad();
    void intentarMostrarPublicidad(const string& membresia);
};

#endif
