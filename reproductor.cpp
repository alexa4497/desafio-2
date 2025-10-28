#include "canciones.h"
#include "metricas.h"
#include "publicidad.h"
#include "usuarios.h"
#include "reproductor.h"
#include "lista_favoritos.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <ctime>
#include <string>



static Publicidad gestorPublicidad; //

using namespace std;
using namespace std::chrono;


void imprimirInfoCancion(const Cancion& cancion, int indice, int total, const string& membresia) {
    cout << "\n-------------------- REPRODUCIENDO --------------------" << endl;
    cout << "  Cancion: " << cancion.getNombre() << " (" << (indice + 1) << " de " << total << ")" << endl;
    cout << "  Ruta a la portada del album: [RUTA_ALBUM]" << endl;


    if (membresia == "p") {
        cout << "  Ruta al archivo de audio (320 kbps - Premium): " << cancion.getRutaAudio320() << endl;
    } else {
        cout << "  Ruta al archivo de audio (128 kbps - Estándar): " << cancion.getRutaAudio128() << endl;
    }

    cout << "-------------------------------------------------------" << endl;
    incrementarContador(1);
}
void reproducirEnOrdenSecuencial(Cancion* lista, int numCanciones, const string& membresia, int numUsuariosTotal) {
    if (numCanciones == 0) {
        cout << "La lista esta vacia." << endl;
        return;
    }

    int indiceActual = 0;
    int opcion = 0;

    // Se asume que 'p' significa Premium, tal como está en el código original.
    bool esPremium = (membresia == "p");

    while (opcion != 3 && indiceActual >= 0 && indiceActual < numCanciones) {

        imprimirInfoCancion(lista[indiceActual], indiceActual, numCanciones, membresia);

        cout << "\n--- CONTROL DE REPRODUCCION SECUENCIAL ---" << endl;
        cout << "1. Siguiente cancion" << endl;

        // INICIO DE LA MODIFICACIÓN: Mostrar opciones 2 y 4 SOLO si es Premium.
        if (esPremium) {
            cout << "2. Volver a la cancion anterior" << endl;
            cout << "4. Repetir cancion actual indefinidamente (Premium)" << endl;
        }
        // Si no es Premium, no se imprime ninguna línea aquí.
        // FIN DE LA MODIFICACIÓN

        cout << "3. Detener reproduccion" << endl;
        cout << "Ingrese su opcion: ";

        if (!(cin >> opcion)) {
            cout << "Entrada invalida. Saltando a la siguiente cancion por defecto." << endl;
            cin.clear();
            cin.ignore(10000, '\n');
            opcion = 1;
            continue;
        }

        if (opcion == 1) {
            gestorPublicidad.intentarMostrarPublicidad(membresia);
            indiceActual++;
        } else if (opcion == 2) {
            if (esPremium) {
                if (indiceActual > 0) {
                    indiceActual--;
                } else {
                    cout << "Ya estas en la primera cancion. No se puede retroceder mas." << endl;
                }
            }else {
                // Mensaje de error si un usuario no Premium intenta ingresar '2'
                cout << "\nMENSAJE: Debes ser usuario Premium para usar la funcion 'Volver a la cancion anterior'." << endl;
            }
        } else if (opcion == 4) {

            if (esPremium) {
                int opcionRepetir = 0;
                cout << "\n=======================================================" << endl;
                cout << "      MODO REPETICION INDEFINIDA ACTIVADO" << endl;
                cout << "=======================================================" << endl;

                while (opcionRepetir != 1) {
                    imprimirInfoCancion(lista[indiceActual], indiceActual, numCanciones, membresia);

                    cout << "\nRepitiendo indefinidamente: " << lista[indiceActual].getNombre() << endl;
                    cout << "1. Salir del modo repetición y volver al control secuencial" << endl;
                    cout << "Ingrese su opcion: ";

                    if (!(cin >> opcionRepetir)) {
                        cout << "Entrada invalida. Intente de nuevo." << endl;
                        cin.clear();
                        cin.ignore(10000, '\n');
                        continue;
                    }

                    if (opcionRepetir != 1) {
                        cout << "Opción no válida. Solo puedes elegir '1' para salir." << endl;
                    }


                    // Asumiendo que TIEMPO_ESPERA y gestorPublicidad son visibles
                    // this_thread::sleep_for(TIEMPO_ESPERA);
                    // gestorPublicidad.intentarMostrarPublicidad(membresia);
                }


                cin.ignore(10000, '\n');

            } else {
                cout << "\nMENSAJE: Debes ser usuario Premium para usar la funcion 'Repetir cancion'." << endl;
            }

        } else if (opcion == 3) {
            cout << "Deteniendo la reproduccion..." << endl;
        } else {
            cout << "Opcion no valida. Mantenemos la cancion actual." << endl;
        }

        if (indiceActual >= numCanciones) {
            break;
        }

        if (opcion != 3 && opcion != 4) {
            cout << "\nPresione ENTER para continuar." << endl;
            cin.ignore(10000, '\n');
        }
    }

    cout << "\n=======================================================" << endl;
    cout << "REPRODUCION EN ORDEN FINALIZADA/DETENIDA." << endl;
    cout << "=======================================================" << endl;
    incrementarContador(1);
}


void reproducirAleatorioTemporizado(Cancion* lista, int numCanciones, const string& membresia, int numUsuariosTotal) {

    if (numCanciones == 0) {
        cout << "El catalogo esta vacio." << endl;
        return;
    }


    int limiteK = (numCanciones < 5) ? numCanciones : 5;

    srand(time(0));

    bool esPremium = (membresia == "p");
    int indiceActual = -1;
    int cancionesReproducidas = 0;


    bool yaReproducida[numCanciones];

    for(int i = 0; i < numCanciones; ++i) {
        yaReproducida[i] = false;
    }

    const int MAX_HISTORIAL = 5;
    int historial[MAX_HISTORIAL] = {-1, -1, -1, -1, -1};
    int tamanoHistorial = 0;

    cout << "\n=======================================================" << endl;
    cout << "INICIANDO REPRODUCCIÓN ALEATORIA AUTOMATICA (Limite: " << limiteK << " canciones, 3s/cancion)" << endl;
    cout << "=======================================================" << endl;

    while (cancionesReproducidas < limiteK) {


        do {
            indiceActual = rand() % numCanciones;
        } while (yaReproducida[indiceActual]);
        yaReproducida[indiceActual] = true;

        if (tamanoHistorial < MAX_HISTORIAL) {
            historial[tamanoHistorial] = indiceActual;
            tamanoHistorial++;
        } else {
            for (int i = 0; i < MAX_HISTORIAL - 1; ++i) {
                historial[i] = historial[i+1];
            }
            historial[MAX_HISTORIAL - 1] = indiceActual;
        }


        imprimirInfoCancion(lista[indiceActual], indiceActual, numCanciones, membresia);
        cout << "Ciclo " << (cancionesReproducidas + 1) << " de " << limiteK << ". Proxima cancion en 3 segundos..." << endl;


        this_thread::sleep_for(TIEMPO_ESPERA);
        incrementarContador(1);

        cancionesReproducidas++;
    }

    cout << "\n=====================================================================" << endl;
    cout << "LIMITE DE K=" << limiteK << " ALCANZADO. Las canciones no se repitieron." << endl;
    cout << "=======================================================================" << endl;


    int opcion = 0;
    while (opcion != 4) {

        cout << "\n--- CONTROL DE REPRODUCCION FINALIZADO ---" << endl;
        cout << "Cancion actual: " << lista[indiceActual].getNombre() << endl;

        cout << "1. Siguiente cancion aleatoria (Puede repetirse)" << endl;


        if (esPremium) {
            cout << "2. Volver a la cancion anterior (Premium - Historial: " << tamanoHistorial - 1 << " previas)" << endl;
        } else {
            cout << "2. Volver a la cancion anterior (Premium)" << endl;
        }

        cout << "4. Detener reproduccion" << endl;
        cout << "Ingrese su opcion: ";

        if (!(cin >> opcion)) {
            cout << "Entrada invalida. Intente de nuevo." << endl;
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        if (opcion == 1) {

            if (tamanoHistorial < MAX_HISTORIAL) {
                historial[tamanoHistorial++] = indiceActual;
            } else {
                for (int i = 0; i < MAX_HISTORIAL - 1; ++i) historial[i] = historial[i+1];
                historial[MAX_HISTORIAL - 1] = indiceActual;
            }

            indiceActual = rand() % numCanciones;
            cout << "\nReproduciendo siguiente cancion..." << endl;

            imprimirInfoCancion(lista[indiceActual], indiceActual, numCanciones, membresia);
        } else if (opcion == 2) {
            if (esPremium) {
                if (tamanoHistorial > 1) {
                    historial[--tamanoHistorial] = -1;
                    indiceActual = historial[tamanoHistorial - 1];
                    cout << "\nVolviendo a la cancion anterior..." << endl;

                    imprimirInfoCancion(lista[indiceActual], indiceActual, numCanciones, membresia);
                } else {
                    cout << "\nMENSAJE: No hay mas canciones previas en el historial (maximo 4 previas)." << endl;
                }
            } else {
                cout << "\nMENSAJE: Debes ser usuario Premium para usar la funcion 'Volver a la cancion anterior'." << endl;
            }
        } else if (opcion == 4) {
            cout << "Deteniendo la reproduccion..." << endl;
        } else {
            cout << "Opcion no valida. Intente de nuevo." << endl;
        }

        cin.ignore(10000, '\n');
    }

    incrementarContador(1);
}
