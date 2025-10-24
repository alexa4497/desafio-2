#include "Canciones.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <ctime>

using namespace std;
using namespace std::chrono;


const int K = 5;
const seconds TIEMPO_ESPERA(3);

void imprimirInfoCancion(const Cancion& cancion, int indice, int total) {
    cout << "\n-------------------- REPRODUCIENDO --------------------" << endl;
    cout << "  Cancion: " << cancion.getNombre() << " (" << (indice + 1) << " de " << total << ")" << endl;
    // La portada del album se asume que se relaciona con la cancion
    cout << "  Ruta a la portada del album: [RUTA_ALBUM]" << endl;
    cout << "  Ruta al archivo de audio (128 kbps): " << cancion.getRutaAudio128() << endl;
    cout << "-------------------------------------------------------" << endl;
}

void reproducirEnOrdenSecuencial(Cancion* arregloCanciones, int tamanoTotal) {
    // 1. Iniciar la reproducción desde el primer elemento.
    for (int i = 0; i < tamanoTotal; ++i) {

        imprimirInfoCancion(arregloCanciones[i], i, tamanoTotal);

        this_thread::sleep_for(seconds(2)); // Pequeña espera para simular el proceso

        cout << "Cancion terminada. Pasando a la siguiente." << endl;
    }

    cout << "\n=======================================================" << endl;
    cout << "REPRODUCCIÓN EN ORDEN FINALIZADA: Fin del catalogo." << endl;
    cout << "=======================================================" << endl;

    cout << "Opciones de reproducción:" << endl;
    cout << "1.- Reproducir 2.- Detener...." << endl;
}



void reproducirAleatorioTemporizado(Cancion* arregloCanciones, int tamanoTotal) {
    if (tamanoTotal == 0) {
        cout << "El catalogo esta vacio." << endl;
        return;
    }

    srand(time(0));

    int cancionesReproducidas = 0;


    while (cancionesReproducidas < K) {
        int indiceAleatorio = rand() % tamanoTotal;

        imprimirInfoCancion(arregloCanciones[indiceAleatorio], indiceAleatorio, tamanoTotal);
        this_thread::sleep_for(TIEMPO_ESPERA);

        cancionesReproducidas++;

        // Mostrar aviso
        if (cancionesReproducidas < K) {
            cout << "\nAuto-avance despues de 3 segundos. Quedan " << (K - cancionesReproducidas) << " canciones para detenerse." << endl;
        }
    }


    cout << "\n=======================================================" << endl;
    cout << "REPRODUCCIÓN ALEATORIA DETENIDA: Limite de K=" << K << " alcanzado." << endl;
    cout << "=======================================================" << endl;

    cout << "Opciones de reproduccion:" << endl;
    cout << "1.- Reproducir 2.- Detener...." << endl;
    cout << "*Todas las opciones que apliquen (siguiente, previa, repetir, etc. para premium)" << endl;
}
