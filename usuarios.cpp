#include "funciones_librerias.h"

bool iniciarSesion() {
    string usuario_ingresado;
    string fecha_ingresada;

    cout << "--- INICIO DE SESION ---" << endl;
    cout << "Ingrese su Nickname (primer dato): ";
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
        string lineaMatriz = registrosUsuario[i];
        string nicknameArchivo;
        string fechaArchivo;

        size_t pos1 = lineaMatriz.find(',');
        if (pos1 == string::npos) continue;

        nicknameArchivo = lineaMatriz.substr(0, pos1);
        size_t inicio_nick = nicknameArchivo.find_first_not_of(" \t");
        if (string::npos != inicio_nick) {
            nicknameArchivo = nicknameArchivo.substr(inicio_nick);
        }

        size_t current_pos = pos1 + 1;
        for (int j = 0; j < 3; ++j) {
            current_pos = lineaMatriz.find(',', current_pos);
            if (current_pos == string::npos) break;
            current_pos++;
        }

        if (current_pos != string::npos) {
            size_t fecha_inicio_pos = current_pos;
            fechaArchivo = lineaMatriz.substr(fecha_inicio_pos);

            size_t fin = fechaArchivo.find_last_not_of(" \t\r\n");
            if (string::npos != fin) {
                fechaArchivo = fechaArchivo.substr(0, fin + 1);
            } else {
                fechaArchivo = "";
            }
        } else {
            continue;
        }

        if (nicknameArchivo == usuario_ingresado && fechaArchivo == fecha_ingresada) {
            cout << "Inicio de sesion exitoso para el usuario: " << usuario_ingresado << endl;
            sesionExitosa = true;
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

#ifndef USUARIO_H
#define USUARIO_H

#include <string>
using namespace std;

class Cancion;

class Usuario {
protected:
    string nickname;
    string membresia;
    string ciudad;
    string pais;
    string fechaInscripcion;

    int cancionesReproducidasSinPublicidad;

public:
    Usuario(const string& n, const string& m, const string& c, const string& p);

    bool esPremium() const;
    void mostrarPublicidadSiAplica();
    static const int MAX_FAVORITOS = 10000;
    Cancion* listaFavoritos[MAX_FAVORITOS];
    int numFavoritos;

    void agregarFavorito(Cancion* c);
    void seguirLista(Usuario* otroUsuarioPremium);
};

#endif
