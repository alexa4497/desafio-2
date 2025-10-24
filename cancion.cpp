#include "Canciones.h"
#include <iostream>
#include <fstream>
#include <sstream>

const int CAPACIDAD_INICIAL = 5;
const string RUTA_BASE_DATOS = "base_datos/canciones.txt";

struct DatosCarga {
    Cancion* arreglo;
    int tamano;
    int capacidad;
};

Cancion::Cancion() : idCancion(0), nombre(""), duracion(0), rutaAudio128(""), rutaAudio320(""), vecesReproducida(0) {}

void Cancion::asignarDatos(string nom, long long id, int dur, string r128, string r320, long long rep) {
    nombre = nom;
    idCancion = id;
    duracion = dur;
    rutaAudio128 = r128;
    rutaAudio320 = r320;
    vecesReproducida = rep;
}

Cancion& Cancion::operator=(const Cancion& otra) {
    if (this != &otra) {
        idCancion = otra.idCancion;
        nombre = otra.nombre;
        duracion = otra.duracion;
        rutaAudio128 = otra.rutaAudio128;
        rutaAudio320 = otra.rutaAudio320;
        vecesReproducida = otra.vecesReproducida;
    }
    return *this;
}

// Función auxiliar: Redimensionamiento del arreglo dinámico
void redimensionarArreglo(DatosCarga& datos) {
    int nuevaCapacidad = datos.capacidad * 2;
    Cancion* nuevoArreglo = new Cancion[nuevaCapacidad];

    for (int i = 0; i < datos.tamano; ++i) {
        nuevoArreglo[i] = datos.arreglo[i];
    }

    delete[] datos.arreglo;
    datos.arreglo = nuevoArreglo;
    datos.capacidad = nuevaCapacidad;
}


Cancion* cargarCancionesDesdeArchivo(const string& nombreArchivo, int& tamano) {

    ifstream archivo(RUTA_BASE_DATOS);

    if (!archivo.is_open()) {
        tamano = 0;
        return nullptr;
    }

    DatosCarga datos;
    datos.tamano = 0;
    datos.capacidad = CAPACIDAD_INICIAL;
    datos.arreglo = new Cancion[datos.capacidad];

    string linea;
    while (getline(archivo, linea)) {
        if (datos.tamano == datos.capacidad) {
            redimensionarArreglo(datos);
        }

        stringstream ss(linea);
        string campo;
        string campos[6];
        int i = 0;

        // Extracción de campos separados por coma
        while (getline(ss, campo, ',')) {
            campos[i++] = campo;
        }

        // Asume el orden del archivo: Nombre, ID, Duración, Ruta128, Ruta320, VecesReproducida
        if (i == 6) {
            string nombre = campos[0];
            long long id = stoll(campos[1]);
            int duracion = stoi(campos[2]);
            string ruta128 = campos[3];
            string ruta320 = campos[4];
            long long vecesRep = stoll(campos[5]);

            datos.arreglo[datos.tamano++].asignarDatos(nombre, id, duracion, ruta128, ruta320, vecesRep);
        }
    }

    archivo.close();
    tamano = datos.tamano;
    return datos.arreglo;
}
