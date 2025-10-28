#include "lista_favoritos.h"
#include "metricas.h" // Necesario para DB_USUARIOS_GLOBAL, NUM_USUARIOS_GLOBAL
#include "Canciones.h"
#include "usuarios.h" // Necesario para conocer el tipo Usuario
#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <cctype>
#include <stdexcept>
#include "publicidad.h"

using namespace std;


extern Usuario* DB_USUARIOS_GLOBAL;
extern int NUM_USUARIOS_GLOBAL;
extern void incrementarContador(long long cantidad);


const string RUTA_FAVORITOS_MAESTRO = "base_datos/listas_favoritos.txt";


static string trim(const string& str) {
    size_t first = str.find_first_not_of(' ');
    if (string::npos == first) return "";
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

// 2. Función buscarCancionEnDB
Cancion* buscarCancionEnDB(long long idCancion, Cancion* dbCanciones, int numDB) {
    for (int i = 0; i < numDB; ++i) {
        // Corrección de llamada ambigua
        incrementarContador(1);
        try {
            string idCatalogoLimpio = trim(dbCanciones[i].getIdentificador());
            if (stoll(idCatalogoLimpio) == idCancion) {
                return &dbCanciones[i];
            }
        } catch (...) {
        }
    }
    return nullptr;
}

// --- Declaraciones de Reproducción (Asumidas como globales) ---
void reproducirEnOrdenSecuencial(Cancion* lista, int numCanciones, const string& membresia, int numUsuariosTotal);
void reproducirAleatorioTemporizado(Cancion* lista, int numCanciones, const string& membresia, int numUsuariosTotal);


// --- IMPLEMENTACIÓN DE ListaFavoritos ---

ListaFavoritos::ListaFavoritos() : numCanciones(0), usuarioPropietario("") {
    for (int i = 0; i < MAX_CANCIONES; ++i) {
        canciones[i] = nullptr;
    }
}
ListaFavoritos::~ListaFavoritos() {}

int ListaFavoritos::buscarCancionEnFavoritos(const string& idCancion) {
    string idBuscadoLimpio = trim(idCancion);
    for (int i = 0; i < numCanciones; ++i) {
        incrementarContador(1);
        if (canciones[i] != nullptr && trim(canciones[i]->getIdentificador()) == idBuscadoLimpio) {
            return i;
        }
    }
    return -1;
}

void ListaFavoritos::guardar_favoritos_a_archivo() {
    // 1. OBTENER EL ÍNDICE DEL USUARIO ACTUAL (CRÍTICO)
    int indiceUsuarioActual = -1;

    if (NUM_USUARIOS_GLOBAL == 0 || DB_USUARIOS_GLOBAL == nullptr) {
        cout << "ERROR CRITICO: La base de datos de usuarios no está cargada." << endl;
        return;
    }

    for (int i = 0; i < NUM_USUARIOS_GLOBAL; ++i) {
        // Se requiere DB_USUARIOS_GLOBAL[i].getNickname() (debe ser public en usuarios.h)
        if (DB_USUARIOS_GLOBAL[i].getNickname() == usuarioPropietario) {
            indiceUsuarioActual = i;
            break;
        }
    }

    if (indiceUsuarioActual == -1) {
        cout << "ERROR CRITICO: No se pudo encontrar el indice del usuario '" << usuarioPropietario << "' para guardar." << endl;
        return;
    }


    string idsString = "[";
    for (int i = 0; i < numCanciones; ++i) {
        if (canciones[i] != nullptr) {
            idsString += trim(canciones[i]->getIdentificador());
            if (i < numCanciones - 1) {
                idsString += ",";
            }
        }
    }
    idsString += "]";

    // 3. LEER EL ARCHIVO MAESTRO COMPLETO
    ifstream archivo_lectura(RUTA_FAVORITOS_MAESTRO);
    string lineaCompleta;

    if (!archivo_lectura.is_open() || !getline(archivo_lectura, lineaCompleta)) {
        lineaCompleta = "";
    }
    archivo_lectura.close();


    string* segmentos = new string[NUM_USUARIOS_GLOBAL];
    int segmentosCargados = 0;

    // Inicializar el arreglo de segmentos con la lista vacía para seguridad
    for(int i = 0; i < NUM_USUARIOS_GLOBAL; ++i) {
        segmentos[i] = "[]";
    }

    // 5. CARGAR Y SEPARAR SEGMENTOS EXISTENTES
    stringstream ss_listas(lineaCompleta);
    string segmento;

    while (getline(ss_listas, segmento, '.') && segmentosCargados < NUM_USUARIOS_GLOBAL) {
        segmentos[segmentosCargados] = segmento;
        segmentosCargados++;
    }

    // 6. REEMPLAZAR EL SEGMENTO DEL USUARIO ACTUAL
    if (indiceUsuarioActual < NUM_USUARIOS_GLOBAL) {
        segmentos[indiceUsuarioActual] = idsString;
    }

    // 7. CONSTRUIR LA NUEVA LÍNEA MAESTRA
    string nuevaLineaMaestra;
    for (int i = 0; i < NUM_USUARIOS_GLOBAL; ++i) {
        nuevaLineaMaestra += segmentos[i];
        if (i < NUM_USUARIOS_GLOBAL - 1) {
            nuevaLineaMaestra += ".";
        }
    }

    // 8. LIBERAR MEMORIA DEL ARREGLO DINÁMICO
    delete[] segmentos;

    incrementarContador(1);
    ofstream archivo_escritura(RUTA_FAVORITOS_MAESTRO, ios::trunc);
    if (archivo_escritura.is_open()) {
        archivo_escritura << nuevaLineaMaestra;
        archivo_escritura.close();
        cout << "MENSAJE: Lista de favoritos guardada en el archivo maestro." << endl;
    } else {
        cout << "Error de guardado: No se pudo escribir en el archivo maestro de favoritos." << endl;
    }
}


void ListaFavoritos::agregarCancion(const string& idCancion, Cancion* dbCanciones, int numDB) {
    // 1. INICIO: Reiniciar el contador de iteraciones para esta operación


    string idLimpio = trim(idCancion);


    if (buscarCancionEnFavoritos(idLimpio) != -1) {
        cout << "Mensaje: La cancion con ID " << idLimpio << " ya esta en tu lista de favoritos." << endl;
        // 3. FINAL: Reportar métricas incluso si no se agregó
        size_t memoriaActual = calcularMemoriaTotal(numDB, NUM_USUARIOS_GLOBAL, numCanciones);
        mostrarMetricas(memoriaActual, "VERIFICAR/AGREGAR CANCION");
        return;
    }

    long long idNumerico = 0;
    try {
        idNumerico = stoll(idLimpio);
    } catch(...) {
        cout << "Mensaje: ID invalido. Debe ser un numero." << endl;
        // 3. FINAL: Reportar métricas
        size_t memoriaActual = calcularMemoriaTotal(numDB, NUM_USUARIOS_GLOBAL, numCanciones);
        mostrarMetricas(memoriaActual, "VERIFICAR/AGREGAR CANCION");
        return;
    }

    // *La función 'buscarCancionEnDB' ya usa incrementarContador(1) en su bucle*
    Cancion* cancionDB = buscarCancionEnDB(idNumerico, dbCanciones, numDB);

    if (cancionDB != nullptr) {
        if (numCanciones < MAX_CANCIONES) {
            canciones[numCanciones] = cancionDB;
            numCanciones++; // Aumento de la memoria dinámica
            guardar_favoritos_a_archivo();
            cout << "Mensaje: Cancion agregada a la lista de favoritos." << endl;

            // 3. FINAL: Calcular y mostrar métricas
            size_t memoriaActual = calcularMemoriaTotal(numDB, NUM_USUARIOS_GLOBAL, numCanciones);
            mostrarMetricas(memoriaActual, "AGREGAR CANCION EXITOSO");
        } else {
            cout << "Error: La lista de favoritos esta llena (maximo " << MAX_CANCIONES << ")." << endl;
            // 3. FINAL: Reportar métricas (memoria no cambió)
            size_t memoriaActual = calcularMemoriaTotal(numDB, NUM_USUARIOS_GLOBAL, numCanciones);
            mostrarMetricas(memoriaActual, "AGREGAR CANCION (LISTA LLENA)");
        }
    } else {
        cout << "Mensaje: La cancion con ID " << idLimpio << " no fue encontrada en la base de datos." << endl;
        // 3. FINAL: Reportar métricas (fallo en la búsqueda)
        size_t memoriaActual = calcularMemoriaTotal(numDB, NUM_USUARIOS_GLOBAL, numCanciones);
        mostrarMetricas(memoriaActual, "AGREGAR CANCION (FALLO EN DB)");
    }
}

// lista_favoritos.cpp

void ListaFavoritos::quitarCancion(const string& idCancion) {
    // 1. INICIO: Reiniciar el contador de iteraciones (Se mantiene)


    string idLimpio = trim(idCancion);
    int posicion = buscarCancionEnFavoritos(idLimpio);

    if (posicion != -1) {
        cout << "\n--- ELIMINAR DE FAVORITOS ---" << endl;

        // Bucle de reorganización
        for (int i = posicion; i < numCanciones - 1; ++i) {
            incrementarContador(1);
            canciones[i] = canciones[i + 1];
        }
        numCanciones--; // Disminución de la memoria dinámica
        canciones[numCanciones] = nullptr;

        guardar_favoritos_a_archivo();

        cout << "Cancion con ID " << idLimpio << " eliminada de la lista." << endl;


    } else {
        cout << "Error: La cancion con ID " << idLimpio << " no se encuentra en tu lista de favoritos." << endl;

    }
}

void ListaFavoritos::cargar_favoritos_desde_archivo(const std::string& nicknameUsuario, Cancion* catalogoCanciones, int tamanoCatalogo, int indiceUsuario) {

    usuarioPropietario = nicknameUsuario;
    numCanciones = 0;
    // Limpiar punteros antiguos
    for (int i = 0; i < MAX_CANCIONES; ++i) {
        canciones[i] = nullptr;
    }

    ifstream archivo(RUTA_FAVORITOS_MAESTRO);
    if (!archivo.is_open()) {
        cout << "ERROR: No se pudo abrir el archivo MAESTRO de favoritos en la ruta: " << RUTA_FAVORITOS_MAESTRO << endl;
        return;
    }

    string lineaCompleta;
    if (!getline(archivo, lineaCompleta)) {
        archivo.close();
        cout << "INFORME: El archivo maestro de favoritos está vacío." << endl;
        return;
    }
    archivo.close();

    // 1. Encontrar la lista del usuario actual (segmentada por el punto '.')
    stringstream ss_listas(lineaCompleta);
    string listaUsuarioActual;
    int indice = 0;

    // Busca el segmento de la lista que corresponde al índice del usuario
    while (getline(ss_listas, listaUsuarioActual, '.')) {
        if (indice == indiceUsuario) {
            break;
        }
        indice++;
    }

    // Si no se encontró el segmento o el índice está fuera de rango.
    if (indice != indiceUsuario || listaUsuarioActual.empty()) {
        cout << "Lista de favoritos vacia para " << nicknameUsuario << " (sin segmento o segmento vacio)." << endl;
        return;
    }

    // 2. Limpiar corchetes '[]' y espacios
    string idsConComas = trim(listaUsuarioActual);

    // Quitar corchetes, si existen
    if (idsConComas.length() >= 2 && idsConComas[0] == '[' && idsConComas.back() == ']') {
        idsConComas = idsConComas.substr(1, idsConComas.length() - 2);
    }

    // Si la lista está vacía después de la limpieza
    if (trim(idsConComas).empty()) {
        cout << "INFORME DE CARGA FINAL: Lista de favoritos cargada. Total de canciones: 0" << endl;
        return;
    }

    // 3. Procesar los IDs separados por coma
    stringstream ss_ids(idsConComas);
    string idToken;
    int cancionesCargadas = 0;

    while (getline(ss_ids, idToken, ',')) {
        string idLimpio = trim(idToken);
        if (idLimpio.empty()) continue;
        if (numCanciones >= MAX_CANCIONES) break;

        try {
            long long id = stoll(idLimpio);
            Cancion* cancionPtr = buscarCancionEnDB(id, catalogoCanciones, tamanoCatalogo);

            if (cancionPtr != nullptr) {
                canciones[numCanciones++] = cancionPtr;
                cancionesCargadas++;
            } else {
                cout << "ADVERTENCIA: ID " << idLimpio << " no encontrado en el catalogo principal." << endl;
            }
        } catch (const std::exception& e) {
            cout << "ERROR DE PARSEO: Token '" << idToken << "' no es un ID valido." << endl;
        }
    }

    cout << "INFORME DE CARGA FINAL: Lista de favoritos cargada. Total de canciones: " << cancionesCargadas << endl;
}



void ListaFavoritos::seguir_otra_lista(Cancion* catalogoCanciones, int tamanoCatalogo) {

    string nicknameSeguir;
    size_t memoriaActual;

    cout << "\n--- SEGUIR LISTA DE USUARIO PREMIUM ---" << std::endl;
    cout << "Ingrese el nickname del usuario cuya lista desea seguir: ";

    // Limpieza de buffer
    if (cin.peek() == '\n') cin.ignore(10000, '\n');
    if (!getline(std::cin, nicknameSeguir) || nicknameSeguir.empty()) {
        cout << "Error: Nickname no ingresado o invalido." << std::endl;


        memoriaActual = calcularMemoriaTotal(tamanoCatalogo, NUM_USUARIOS_GLOBAL, numCanciones);
        mostrarMetricas(memoriaActual, "SEGUIR LISTA (FALLO EN ENTRADA)");
        return;
    }


    if (NUM_USUARIOS_GLOBAL == 0 || DB_USUARIOS_GLOBAL == nullptr) {
        std::cout << "ERROR: La base de datos de usuarios no está cargada. No se puede seguir ninguna lista." << std::endl;

        // 2. Cálculo y muestra de métricas (Fallo en globales)
        memoriaActual = calcularMemoriaTotal(tamanoCatalogo, NUM_USUARIOS_GLOBAL, numCanciones);
        mostrarMetricas(memoriaActual, "SEGUIR LISTA (ERROR DE CARGA)");
        return;
    }

    // 1. BUSCAR EL ÍNDICE USANDO LAS VARIABLES GLOBALES
    int indiceUsuarioSeguido = -1;
    for (int i = 0; i < NUM_USUARIOS_GLOBAL; ++i) {
        incrementarContador(1);
        if (DB_USUARIOS_GLOBAL[i].getNickname() == nicknameSeguir) {
            indiceUsuarioSeguido = i;
            break;
        }
    }

    if (indiceUsuarioSeguido == -1) {
        std::cout << "Error: El usuario '" << nicknameSeguir << "' no existe en la base de datos." << std::endl;

        // 3. Cálculo y muestra de métricas (Fallo en búsqueda de usuario)
        memoriaActual = calcularMemoriaTotal(tamanoCatalogo, NUM_USUARIOS_GLOBAL, numCanciones);
        mostrarMetricas(memoriaActual, "SEGUIR LISTA (FALLO EN BÚSQUEDA)");
        return;
    }

    // Bloque 2 y 3: Extracción de la lista del archivo maestro
    std::ifstream archivo(RUTA_FAVORITOS_MAESTRO);
    std::string lineaCompleta;
    if (std::getline(archivo, lineaCompleta)) {
        archivo.close();
    } else {
        archivo.close();
        std::cout << "ADVERTENCIA: El archivo maestro de favoritos esta vacio." << std::endl;

        // 4. Cálculo y muestra de métricas (Archivo maestro vacío)
        memoriaActual = calcularMemoriaTotal(tamanoCatalogo, NUM_USUARIOS_GLOBAL, numCanciones);
        mostrarMetricas(memoriaActual, "SEGUIR LISTA (ARCHIVO MAESTRO VACÍO)");
        return;
    }

    std::stringstream ss_listas(lineaCompleta);
    std::string listaUsuarioSeguido;
    int indice = 0;

    while (std::getline(ss_listas, listaUsuarioSeguido, '.')) {
        incrementarContador(1); // Costo temporal de la búsqueda en el archivo
        if (indice == indiceUsuarioSeguido) break;
        indice++;
    }

    if (indice != indiceUsuarioSeguido || listaUsuarioSeguido.empty()) {
        std::cout << "ADVERTENCIA: Lista de favoritos vacia para " << nicknameSeguir << " en el archivo maestro." << std::endl;

        // 5. Cálculo y muestra de métricas (Lista de usuario objetivo vacía)
        memoriaActual = calcularMemoriaTotal(tamanoCatalogo, NUM_USUARIOS_GLOBAL, numCanciones);
        mostrarMetricas(memoriaActual, "SEGUIR LISTA (LISTA OBJETIVO VACÍA)");
        return;
    }

    std::string idsConComas = trim(listaUsuarioSeguido);
    if (idsConComas.length() >= 2 && idsConComas[0] == '[' && idsConComas.back() == ']') {
        idsConComas = idsConComas.substr(1, idsConComas.length() - 2);
    }
    // Fin Bloque 2 y 3.

    // 4. PROCESAR Y AGREGAR SOLO LOS IDs NO REPETIDOS
    std::stringstream ss_ids(idsConComas);
    std::string idToken;
    int cancionesAgregadas = 0;
    int cancionesOmitidas = 0;

    std::cout << "\n--- AGREGANDO CANCIONES DE " << nicknameSeguir << " ---" << std::endl;

    while (std::getline(ss_ids, idToken, ',')) {
        std::string idLimpio = trim(idToken);
        if (idLimpio.empty()) continue;

        // 4.1. VERIFICAR SI YA ESTÁ EN LA LISTA DEL USUARIO ACTUAL
        if (buscarCancionEnFavoritos(idLimpio) != -1) {
            cancionesOmitidas++;
            continue;
        }

        if (numCanciones >= MAX_CANCIONES) {
            std::cout << "ADVERTENCIA: Tu lista esta llena. No se agregaron mas canciones." << std::endl;
            break;
        }

        try {
            long long id = std::stoll(idLimpio);
            Cancion* cancionPtr = buscarCancionEnDB(id, catalogoCanciones, tamanoCatalogo);

            if (cancionPtr != nullptr) {
                // 4.2. Agregar el puntero a la lista del usuario actual
                canciones[numCanciones++] = cancionPtr;
                cancionesAgregadas++;
            }
        } catch (...) { }
    }

    // 5. GUARDADO Y NOTIFICACIÓN
    if (cancionesAgregadas > 0) {
        guardar_favoritos_a_archivo();
        std::cout << "ÉXITO: Se agregaron " << cancionesAgregadas
                  << " canciones de " << nicknameSeguir << " a tu lista de favoritos." << std::endl;

        // 6. Cálculo y muestra de métricas (Éxito)
        memoriaActual = calcularMemoriaTotal(tamanoCatalogo, NUM_USUARIOS_GLOBAL, numCanciones);
        mostrarMetricas(memoriaActual, "SEGUIR LISTA EXITOSO");
    } else {
        std::cout << "MENSAJE: No se pudo agregar ninguna cancion nueva a tu lista. (" << cancionesOmitidas << " omitidas por duplicado)." << std::endl;

        // 7. Cálculo y muestra de métricas (Sin cambios/Error en adición)
        memoriaActual = calcularMemoriaTotal(tamanoCatalogo, NUM_USUARIOS_GLOBAL, numCanciones);
        mostrarMetricas(memoriaActual, "SEGUIR LISTA (SIN CAMBIOS/FALLO EN ADICIÓN)");
    }
}























// 2. Firma corregida con 4 argumentos
void ListaFavoritos::editar_menu(Cancion* dbCanciones, int numDB, const string& membresia, int numUsuariosTotal) {
    int opcion = 0;
    string idCancion;
    size_t memoriaActual;

    do {
        cout << "\n--- SUBMENU: EDITAR MI LISTA ---" << endl;
        cout << "Canciones actuales: " << numCanciones << "/" << MAX_CANCIONES << endl;
        cout << "1. Agregar cancion por ID" << endl;
        cout << "2. Quitar cancion por ID" << endl;
        cout << "3. Volver al menu anterior" << endl;
        cout << "Ingrese su opcion: ";

        if (!(cin >> opcion)) {
            cout << "Entrada invalida. Intente de nuevo." << endl;
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        switch (opcion) {
        case 1:
            cout << "Ingrese el ID de la cancion que desea agregar: ";
            cin.ignore(10000, '\n');
            if (getline(cin, idCancion)) {
                agregarCancion(idCancion, dbCanciones, numDB);
            }
            break;
        case 2:
            cout << "Ingrese el ID de la cancion que desea quitar: ";
            cin.ignore(10000, '\n');
            if (getline(cin, idCancion)) {
                quitarCancion(idCancion);

                // LÍNEA 483 (Ahora funciona):
                memoriaActual = calcularMemoriaTotal(numDB, numUsuariosTotal, numCanciones);
                // LÍNEA 484 (Ahora funciona):
                mostrarMetricas(memoriaActual, "QUITAR CANCION FINALIZADO");
            }
            break;
        case 3:
            cout << "Volviendo al menu principal de gestion..." << endl;
            break;
        default:
            cout << "Opcion no reconocida. Intente de nuevo." << endl;
            break;
        }

        if (opcion != 3) {
            cout << "\nPresione ENTER para continuar...";
            cin.ignore(10000, '\n');
        }

    } while (opcion != 3);
}

void ListaFavoritos::ejecutar_lista(const string& membresia) {
    if (numCanciones == 0) {
        cout << "Tu lista de favoritos esta vacia. No se puede iniciar la reproduccion." << endl;
        return;
    }

    // Copia de punteros a un arreglo contiguo para la reproducción
    Cancion* listaContigua = new Cancion[numCanciones];
    for(int i = 0; i < numCanciones; ++i) {
        if (canciones[i] != nullptr) {
            listaContigua[i] = *canciones[i];
        }
    }

    int opcion;
    cout << "\n--- INICIAR REPRODUCCIÓN DE FAVORITOS ---" << endl;
    cout << "1. Reproducir en orden secuencial" << endl;
    cout << "2. Reproducir en orden aleatorio (Solo Premium)" << endl;
    cout << "3. Cancelar y volver" << endl;
    cout << "Ingrese su opcion: ";

    if (!(cin >> opcion)) {
        cout << "Entrada invalida. Cancelando reproduccion." << endl;
        cin.clear();
        cin.ignore(10000, '\n');
        delete[] listaContigua;
        return;
    }
    cin.ignore(10000, '\n');

    // La llamada debe pasar 4 argumentos, el cuarto es 0
    switch(opcion) {
    case 1:
        reproducirEnOrdenSecuencial(listaContigua, numCanciones, membresia, 0);
        break;
    case 2:
        if (membresia == "Premium" || membresia == "p" || membresia == "premium") {
            reproducirAleatorioTemporizado(listaContigua, numCanciones, membresia, 0);
        } else {
            cout << "Error: La reproduccion aleatoria es solo para usuarios Premium." << endl;
        }
        break;
    case 3:
        cout << "Reproduccion cancelada." << endl;
        break;
    default:
        cout << "Opcion no valida." << endl;
        break;
    }

    delete[] listaContigua;
}

// 3. Firma corregida con 4 argumentos
void ListaFavoritos::gestionar_listas(Cancion* dbCanciones, int numDB, const string& membresia, int numUsuariosTotal) {
    int opcion = 0;

    do {
        cout << "\n=======================================" << endl;
        cout << "        GESTOR DE LISTAS DE REPRODUCCIÓN        " << endl;
        cout << "=======================================" << endl;
        cout << "1. Reproducir mi lista de favoritos" << endl;
        cout << "2. Seguir otra lista de un usuario Premium (Solo si eres Premium)" << endl;
        cout << "3. Editar/Modificar mi lista de favoritos" << endl;
        cout << "4. Volver al menu principal" << endl;
        cout << "Ingrese su opcion: ";

        if (!(cin >> opcion)) {
            cout << "Entrada invalida. Por favor, ingrese un numero." << endl;
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        switch (opcion) {
        case 1:
            ejecutar_lista(membresia);
            break;
        case 2:
            // Llama a seguir_otra_lista (no requiere verificación de membresía aquí si la haces dentro)
            seguir_otra_lista(dbCanciones, numDB);
            break;
        case 3:
            editar_menu(dbCanciones, numDB, membresia, numUsuariosTotal);
            break;
        case 4:
            cout << "Saliendo del gestor de listas..." << endl;
            break;
        default:
            cout << "Opcion no valida. Intente de nuevo." << endl;
            break;
        }

        if (opcion != 4) {
            cout << "\nPresione ENTER para continuar...";
            cin.ignore(10000, '\n');
        }

    } while (opcion != 4);
}
