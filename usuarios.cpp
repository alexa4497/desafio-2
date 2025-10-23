#include "usuarios.h"

bool Usuario::iniciarSesion() {
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
        stringstream ss(lineaCompleta);
        string campo[5];

        for (int j = 0; j < 5; ++j) {
            if (!getline(ss, campo[j], ',')) {
                break;
            }
        }

        if (ss.fail() && !ss.eof()) continue;


        string nicknameArchivo = campo[0];
        string fechaArchivo = campo[4];

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


Usuario::Usuario() :
    nickname(""),
    membresia(""),
    ciudad(""),
    pais(""),
    fechaInscripcion(""),

    numFavoritos(0)
{

}

