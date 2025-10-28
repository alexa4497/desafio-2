#include "Publicidad.h"
#include "metricas.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <direct.h>
#include <cstdlib>
#include <ctime>
#include <stdexcept>

using namespace std;

string Publicidad::obtenerCategoriaTexto(int prioridad) {
    if (prioridad == 3) return "AAA";
    if (prioridad == 2) return "B";
    return "C";
}

string Publicidad::trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (string::npos == first) return "";
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, (last - first + 1));
    incrementarContador(1);
}

void Publicidad::cargarMensajesDesdeArchivo() {

    string ruta_archivo = "base_datos/publicidad.txt";

    ifstream archivo(ruta_archivo);

    if (!archivo.is_open()) {
        ruta_archivo = "base_datos\\publicidad.txt";
        archivo.open(ruta_archivo);
    }

    if (!archivo.is_open()) {
        cerr << "ADVERTENCIA: No se pudo cargar la base de datos de publicidad." << endl;
        numMensajes = 0;
        return;
    }

    string linea;
    numMensajes = 0;

    while (getline(archivo, linea) && numMensajes < MAX_ADS) {
        incrementarContador(1);
        if (trim(linea).empty()) continue;

        stringstream ss(linea);
        string campo[3];
        string buffer;
        int i = 0;

        while (getline(ss, buffer, ',') && i < 3) {
            campo[i++] = trim(buffer);
            incrementarContador(1);
        }

        if (i == 3) {
            try {
                dbPublicidad[numMensajes].id = stoi(campo[0]);
                int prioridadNumerica = stoi(campo[1]);

                if (prioridadNumerica < 1 || prioridadNumerica > 3) {
                    prioridadNumerica = 1;
                }

                dbPublicidad[numMensajes].prioridad = prioridadNumerica;
                dbPublicidad[numMensajes].categoria = obtenerCategoriaTexto(prioridadNumerica);
                dbPublicidad[numMensajes].contenido = campo[2];

                numMensajes++;

            } catch (const exception&) {
                continue;
            }
        }
    }
    incrementarContador(1);
    archivo.close();
}

Publicidad::Publicidad() : numMensajes(0), ultimoIndiceMostrado(-1), contadorCanciones(0) {
    static bool semillaInicializada = false;
    if (!semillaInicializada) {
        srand(static_cast<unsigned int>(time(nullptr)));
        semillaInicializada = true;
    }
    incrementarContador(1);
    cargarMensajesDesdeArchivo();
}

int Publicidad::seleccionarIndicePonderado() {

    incrementarContador(3);

    if (numMensajes == 0) return -1;
    if (numMensajes == 1) return 0;

    int pesos[MAX_ADS];
    int sumaTotal = 0;

    for (int i = 0; i < numMensajes; ++i) {
        incrementarContador(2);

        if (i == ultimoIndiceMostrado) {
            pesos[i] = 0;
        } else {
            pesos[i] = dbPublicidad[i].prioridad;
            sumaTotal += pesos[i];
            incrementarContador(2);
        }
    }

    if (sumaTotal == 0) {
        return rand() % numMensajes;
    }

    int valorAleatorio = rand() % sumaTotal;
    int sumaAcumulada = 0;

    incrementarContador(2);

    for (int i = 0; i < numMensajes; ++i) {
        incrementarContador(2);
        sumaAcumulada += pesos[i];
        if (valorAleatorio < sumaAcumulada) {
            return i;
        }
    }
    incrementarContador(1);
    return 0;
}

void Publicidad::intentarMostrarPublicidad(const string& membresia) {

    incrementarContador(1);

    if (membresia == "e") {

        contadorCanciones++;
        incrementarContador(2);

        if (contadorCanciones % 2 == 0 && numMensajes > 0) {

            int indice = seleccionarIndicePonderado();

            if (indice != -1) {
                incrementarContador(5);

                cout << "\n=== PUBLICIDAD (" << dbPublicidad[indice].categoria << ") ===" << endl;
                cout << dbPublicidad[indice].contenido << endl;
                cout << "============================\n" << endl;

                ultimoIndiceMostrado = indice;
            }
        }
    }
}
