#include "usuarios.h"
#include "Canciones.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <unistd.h> // Necesario para sleep(duracion)

using namespace std;

Usuario::Usuario() :
    nickname(""),
    membresia(""),
    ciudad(""),
    pais(""),
    fechaInscripcion(""),
    numFavoritos(0)
{

}

bool Usuario::iniciarSesion(Cancion* catalogoCanciones, int tamanoCatalogo) {
    string usuario_ingresado;
    string fecha_ingresada;

    cout << "--- INICIO DE SESION ---" << endl;
    cout << "Ingrese su Nickname : ";
    cin >> usuario_ingresado;

    cout << "Ingrese su Fecha de Inscripcion (formato DD-MM-AA): ";
    cin >> fecha_ingresada;

    const string RUTA_ARCHIVO = "base_datos/usuarios.txt";

    ifstream contadorArchivo(RUTA_ARCHIVO);
    int numUsuarios = 0;
    string lineaContador;

    if (!contadorArchivo.is_open()) {
        cout << "Error: No se pudo abrir el archivo de usuarios en la ruta: " << RUTA_ARCHIVO << endl;
        return false;
    }

    while (getline(contadorArchivo, lineaContador)) {
        numUsuarios++;
    }
    contadorArchivo.close();

    if (numUsuarios == 0) {
        cout << "Error: El archivo de usuarios esta vacio." << endl;
        return false;
    }

    string* registrosUsuario = new string[numUsuarios];
    int indiceUsuario = 0;

    ifstream archivo(RUTA_ARCHIVO);
    string linea;

    while (getline(archivo, linea)) {
        registrosUsuario[indiceUsuario] = linea;
        indiceUsuario++;
    }
    archivo.close();

    bool sesionExitosa = false;

    for (int i = 0; i < numUsuarios; ++i) {
        string lineaCompleta = registrosUsuario[i];

        string nicknameArchivo;
        string fechaArchivo;
        string campo[5];

        // Implementación de parsing sin sstream
        size_t pos = 0;
        size_t next_pos = lineaCompleta.find(',');
        int j = 0;
        while(next_pos != string::npos && j < 5) {
            campo[j] = lineaCompleta.substr(pos, next_pos - pos);
            pos = next_pos + 1;
            next_pos = lineaCompleta.find(',', pos);
            j++;
        }
        if (j < 5) {
            campo[j] = lineaCompleta.substr(pos);
        }

        nicknameArchivo = campo[0];
        fechaArchivo = campo[4];

        size_t inicio_nick = nicknameArchivo.find_first_not_of(" \t");
        if (string::npos != inicio_nick) {
            nicknameArchivo = nicknameArchivo.substr(inicio_nick);
        }

        size_t fin = fechaArchivo.find_last_not_of(" \t\r\n");
        if (string::npos != fin) {
            fechaArchivo = fechaArchivo.substr(0, fin + 1);
        } else {
            fechaArchivo = "";
        }

        if (nicknameArchivo == usuario_ingresado && fechaArchivo == fecha_ingresada) {

            nickname = nicknameArchivo;
            membresia = campo[1];
            ciudad = campo[2];
            pais = campo[3];
            fechaInscripcion = fechaArchivo;

            cout << "Inicio de sesion exitoso para el usuario: " << nickname << endl;
            sesionExitosa = true;

            if (catalogoCanciones != nullptr && tamanoCatalogo > 0) {

                int indiceActual = 0;
                char opcion = '0';
                bool reproduccionActiva = true;

                cout << "\n=======================================================" << endl;
                cout << "  INICIANDO REPRODUCCIÓN EN ORDEN (MODO NORMAL) " << endl;
                cout << "=======================================================" << endl;

                while (reproduccionActiva && indiceActual < tamanoCatalogo) {

                    imprimirInfoCancion(catalogoCanciones[indiceActual], indiceActual, tamanoCatalogo);

                    int duracionCancion = catalogoCanciones[indiceActual].getDuracion();

                    // 2. MENÚ INMEDIATO CON OPCIÓN DE SALTO
                    cout << "\n-------------------- MENÚ REPRODUCTOR --------------------" << endl;
                    cout << "La cancion esta en reproduccion (" << duracionCancion << " segundos restantes). Ingrese una opcion para detener o continuar." << endl;
                    cout << "1. Interrumpir y cambiar a Reproduccion Aleatoria Temporizada (K=5)" << endl;
                    cout << "2. Siguiente cancion (SALTAR inmediatamente)" << endl;
                    cout << "Ingrese una opcion (1, 2, o cualquier otra para CONTINUAR/ESPERAR): ";

                    cin.ignore(1000, '\n');
                    opcion = cin.get();

                    if (opcion == '1') {
                        // 3. CAMBIO A REPRODUCCIÓN ALEATORIA (INTERRUPCIÓN)
                        cout << "\n=======================================================" << endl;
                        cout << "  INTERRUPCIÓN: INICIANDO REPRODUCCIÓN ALEATORIA (K=5) " << endl;
                        cout << "=======================================================" << endl;

                        reproducirAleatorioTemporizado(catalogoCanciones, tamanoCatalogo);

                        reproduccionActiva = false;
                    } else if (opcion == '2') {

                        cout << "\nSaltando a la siguiente cancion..." << endl;
                        indiceActual++;

                    } else {

                        cout << "\nContinuando con la reproduccion secuencial. Espere " << duracionCancion << " segundos (simulacion)..." << endl;
                        sleep(duracionCancion);


                        cout << "Cancion terminada. Pasando a la siguiente." << endl;
                        indiceActual++;
                    }
                }

                if (reproduccionActiva) {
                    cout << "\n=======================================================" << endl;
                    cout << "REPRODUCCIÓN EN ORDEN FINALIZADA: Fin del catalogo." << endl;
                    cout << "=======================================================" << endl;
                }

            } else {
                cout << "Advertencia: No se pudo cargar el catalogo de canciones. La reproduccion no esta disponible." << endl;
            }

            break;
        }
    }
    delete[] registrosUsuario;

    if (!sesionExitosa) {
        cout << "Error: Nombre de usuario o fecha de inscripcion incorrectos." << endl;
    } else {
        cout << "\nAcceso concedido. Bienvenido!" << endl;
    }

    return sesionExitosa;
}
