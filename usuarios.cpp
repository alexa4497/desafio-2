#include "usuarios.h"
#include "Canciones.h"
#include "metricas.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

extern Usuario* DB_USUARIOS_GLOBAL;
extern int NUM_USUARIOS_GLOBAL;
extern void incrementarContador(long long cantidad);


Usuario::Usuario() :
    nickname(""),
    membresia(""),
    ciudad(""),
    pais(""),
    fechaInscripcion("")
{
}


extern void incrementarContador(long long cantidad);



Usuario* cargarUsuariosDesdeArchivo(const std::string& ruta, int& numUsuarios) {
    incrementarContador(1);
    std::ifstream archivo(ruta);
    if (!archivo.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo de usuarios: " << ruta << std::endl;
        numUsuarios = 0;
        return nullptr;
    }


    std::string linea;
    int contadorLineas = 0;
    while (std::getline(archivo, linea)) {
        contadorLineas++;
    }

    numUsuarios = contadorLineas;


    archivo.clear();
    archivo.seekg(0, std::ios::beg);

    if (numUsuarios == 0) {
        return nullptr;
    }
    Usuario* db = new Usuario[numUsuarios];
    int i = 0;


    while (std::getline(archivo, linea) && i < numUsuarios) {
        std::stringstream ss(linea);


        string nick, memb, ciu, pais, fecha;


        if (getline(ss, nick, ',') &&
            getline(ss, memb, ',') &&
            getline(ss, ciu, ',') &&
            getline(ss, pais, ',') &&
            getline(ss, fecha))
        {

            size_t fin = fecha.find_last_not_of(" \t\r\n");
            if (string::npos != fin) {
                fecha = fecha.substr(0, fin + 1);
            } else {
                fecha = "";
            }


            db[i].nickname = nick;
            db[i].membresia = memb;
            db[i].ciudad = ciu;
            db[i].pais = pais;
            db[i].fechaInscripcion = fecha;
        } else {
            std::cerr << "Advertencia: Linea de usuario malformada omitida: " << linea << std::endl;
        }


        incrementarContador(1);
        i++;
    }

    archivo.close();
    return db;
}

bool Usuario::iniciarSesion(Cancion* catalogoCanciones, int tamanoCatalogo, Usuario* dbUsuarios, int numUsuariosTotal) {
    string usuario_ingresado;
    string fecha_ingresada;

    cout << "--- INICIO DE SESION ---" << endl;
    cout << "Ingrese su Nickname : ";
    cin >> usuario_ingresado;

    cout << "Ingrese su Fecha de Inscripcion (formato DD-MM-AA): ";
    cin >> fecha_ingresada;


    cin.ignore(10000, '\n');

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
        string membresiaArchivo;
        string ciudadArchivo;
        string paisArchivo;
        string fechaArchivo;

        stringstream ss(lineaCompleta);


        if (getline(ss, nicknameArchivo, ',') &&
            getline(ss, membresiaArchivo, ',') &&
            getline(ss, ciudadArchivo, ',') &&
            getline(ss, paisArchivo, ',') &&
            getline(ss, fechaArchivo))
        {

            size_t fin = fechaArchivo.find_last_not_of(" \t\r\n");
            if (string::npos != fin) {
                fechaArchivo = fechaArchivo.substr(0, fin + 1);
            } else {
                fechaArchivo = "";
            }


            if (nicknameArchivo == usuario_ingresado && fechaArchivo == fecha_ingresada) {


                nickname = nicknameArchivo;
                membresia = membresiaArchivo;
                ciudad = ciudadArchivo;
                pais = paisArchivo;
                fechaInscripcion = fechaArchivo;

                cout << "Inicio de sesion exitoso para el usuario: " << nickname << endl;
                sesionExitosa = true;


                DB_USUARIOS_GLOBAL = dbUsuarios;
                NUM_USUARIOS_GLOBAL = numUsuariosTotal;


                miListaFavoritos.cargar_favoritos_desde_archivo(nickname, catalogoCanciones, tamanoCatalogo, i);


                if (catalogoCanciones != nullptr && tamanoCatalogo > 0) {
                    int op_menu = 0;

                    do {
                        cout << "\n=======================================================" << endl;
                        cout << "              MENÚ PRINCIPAL DE REPRODUCCION             " << endl;
                        cout << "=======================================================" << endl;
                        cout << "1. Reproducir Catálogo Completo (Orden Secuencial)" << endl;
                        cout << "2. Reproducir Catálogo Completo (Modo Aleatorio)" << endl;
                        cout << "3. Gestionar Lista de Favoritos" << endl;
                        cout << "4. Cerrar Sesion" << endl;
                        cout << "Ingrese su opcion: ";

                        if (!(cin >> op_menu)) {
                            cout << "Entrada invalida. Saliendo de la sesion." << endl;
                            cin.clear();

                            cin.ignore(10000, '\n');
                            op_menu = 4;
                            continue;
                        }

                        switch (op_menu) {
                        case 1:
                            reproducirEnOrdenSecuencial(catalogoCanciones, tamanoCatalogo, membresia,numUsuariosTotal);

                            break;
                        case 2:
                            if (membresia == "p") {
                                reproducirAleatorioTemporizado(catalogoCanciones, tamanoCatalogo, membresia,numUsuariosTotal);
                            } else {
                                cout << "\nMENSAJE: Debes ser usuario Premium para acceder a la Reproduccion Aleatoria." << endl;
                            }
                            break;

                        case 3:
                            if (membresia == "p") {
                                miListaFavoritos.gestionar_listas(catalogoCanciones, tamanoCatalogo, membresia, numUsuariosTotal);

                            } else {
                                cout << "\nMENSAJE: Debes ser usuario Premium para acceder a la Reproduccion Aleatoria." << endl;
                            }
                            break;

                        case 4:
                            cout << "Cerrando sesion..." << endl;
                            break;
                        default:
                            cout << "Opcion no valida. Intente de nuevo." << endl;
                            break;
                        }

                        // Pausa
                        if (op_menu != 4) {
                            cout << "\nPresione ENTER para volver al menu principal...";
                            // Usar getline para esperar ENTER de forma segura.
                            string dummy;
                            getline(cin, dummy);
                        }

                    } while (op_menu != 4);

                } else {
                    cout << "Advertencia: No se pudo cargar el catalogo de canciones. La reproduccion no esta disponible." << endl;
                }

                break;
            }
        }
    }
    delete[] registrosUsuario;
    incrementarContador(1);
    if (!sesionExitosa) {
        cout << "Error: Nombre de usuario o fecha de inscripcion incorrectos." << endl;
    } else {
        cout << "\nAcceso concedido. Bienvenido!" << endl;
    }

    return sesionExitosa;
}

