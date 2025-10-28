#ifndef CANCIONES_H
#define CANCIONES_H

#include <string>
#include <chrono>
#include <thread>

using namespace std;
using namespace std::chrono;

// Constantes globales

const seconds TIEMPO_ESPERA(3);

class Cancion {
private:
    long long idCancion;
    string nombre;
    int duracion;
    string rutaAudio128;
    string rutaAudio320;
    long long vecesReproducida;
    // Eliminado: const string& membresia; (Corrige el error del constructor)

public:
    Cancion();
    void asignarDatos(string nom, long long id, int dur, string r128, string r320, long long rep);
    Cancion& operator=(const Cancion& otra);

    string getNombre() const { return nombre; }
    string getRutaAudio128() const { return rutaAudio128; }
    string getRutaAudio320() const { return rutaAudio320; }
    int getDuracion() const { return duracion; }
    string getIdentificador() const;
};

Cancion* cargarCancionesDesdeArchivo(const string& nombreArchivo, int& tamano);
void imprimirInfoCancion(const Cancion& cancion, int indice, int total, const string& membresia);

// EN lista_favoritos.cpp (antes de cualquier función)
// DECLARACIÓN EXPLÍCITA DE 4 ARGUMENTOS (para asegurar que la llamada coincida)
void reproducirEnOrdenSecuencial(Cancion* lista, int numCanciones, const string& membresia, int numUsuariosTotal);
void reproducirAleatorioTemporizado(Cancion* lista, int numCanciones, const string& membresia, int numUsuariosTotal);

#endif

