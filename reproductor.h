#ifndef REPRODUCTOR_H
#define REPRODUCTOR_H

#include "Canciones.h"
#include "metricas.h"
#include <string>


using namespace std;


void reproducirEnOrdenSecuencial(Cancion* arregloCanciones, int tamanoTotal, const string& membresia, int numUsuariosTotal);
void reproducirAleatorioTemporizado(Cancion* arregloCanciones, int tamanoTotal, const string& membresia, int numUsuariosTotal);

void imprimirInfoCancion(const Cancion& cancion, int indice, int total, const string& membresia);

#endif // REPRODUCTOR_H
