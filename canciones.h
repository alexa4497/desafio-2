#ifndef CANCIONES_H
#define CANCIONES_H

#include <string>
#include <chrono>   // Para std::chrono::seconds
#include <thread>   // Para std::this_thread::sleep_for

using namespace std;
using namespace std::chrono;


class Cancion {
private:
    long long idCancion;
    string nombre;
    int duracion;
    string rutaAudio128;
    string rutaAudio320;
    long long vecesReproducida;

public:
    Cancion();
    void asignarDatos(string nom, long long id, int dur, string r128, string r320, long long rep);
    Cancion& operator=(const Cancion& otra);

    string getNombre() const { return nombre; }
    string getRutaAudio128() const { return rutaAudio128; }
    int getDuracion() const { return duracion; }
};
Cancion* cargarCancionesDesdeArchivo(const string& nombreArchivo, int& tamano);
void imprimirInfoCancion(const Cancion& cancion, int indice, int total);
void reproducirEnOrdenSecuencial(Cancion* arregloCanciones, int tamanoTotal);
void reproducirAleatorioTemporizado(Cancion* arregloCanciones, int tamanoTotal);
#endif
