#include "Canciones.h"
#include "metricas.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cctype>
#include <stdexcept>
#include <algorithm>

using namespace std;

const int CAPACIDAD_INICIAL = 5;
const string RUTA_BASE_DATOS = "base_datos/canciones.txt";

struct DatosCarga {
    Cancion* arreglo;
    int tamano;
    int capacidad;
};

static string trim(const string& str) {
    size_t first = str.find_first_not_of(' ');
    if (string::npos == first) return "";
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
    incrementarContador(1);
}


Cancion::Cancion() : idCancion(0), nombre(""), duracion(0), rutaAudio128(""), rutaAudio320(""), vecesReproducida(0) {}


string Cancion::getIdentificador() const {
    return to_string(idCancion);
}


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
    incrementarContador(1);
}



void redimensionarArreglo(DatosCarga& datos) {
    int nuevaCapacidad = datos.capacidad * 2;
    Cancion* nuevoArreglo = new Cancion[nuevaCapacidad];

    for (int i = 0; i < datos.tamano; ++i) {
        nuevoArreglo[i] = datos.arreglo[i];
    }

    delete[] datos.arreglo;
    datos.arreglo = nuevoArreglo;
    datos.capacidad = nuevaCapacidad;
    incrementarContador(1);
}


Cancion* cargarCancionesDesdeArchivo(const string& nombreArchivo, int& tamano) {
    ifstream archivo(RUTA_BASE_DATOS);

    if (!archivo.is_open()) {
        cout << "Error: No se pudo abrir el archivo de canciones en la ruta: " << RUTA_BASE_DATOS << endl;
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

        while (getline(ss, campo, ',')) {
            if (i < 6) {
                campos[i++] = campo;
            } else {
                break;
            }
        }

        try {
            if (i == 6) {
                string nombre = trim(campos[0]);

                string idStrLimpio = trim(campos[1]);
                long long id = stoll(idStrLimpio);

                int duracion = stoi(trim(campos[2]));
                string ruta128 = trim(campos[3]);
                string ruta320 = trim(campos[4]);
                long long vecesRep = stoll(trim(campos[5]));

                datos.arreglo[datos.tamano++].asignarDatos(nombre, id, duracion, ruta128, ruta320, vecesRep);
            }
        } catch (const std::exception& e) {
            cout << "Advertencia de parseo: Linea ignorada por formato invalido. Error: " << e.what() << endl;
        }
    }

    archivo.close();
    tamano = datos.tamano;
    return datos.arreglo;
    incrementarContador(1);
}

