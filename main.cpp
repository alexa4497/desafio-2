#include "usuarios.h"
#include "Canciones.h"
#include "metricas.h"
#include "publicidad.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;


extern void resetearContador();
extern void incrementarContador(long long cantidad);
extern size_t calcularMemoriaTotal(int tamanoCatalogo, int numUsuariosTotal, int numFavoritosActuales);
extern void mostrarMetricas(size_t memoriaTotal, const std::string& titulo);
extern void mostrarMetricasFinales(size_t memoriaTotal);

extern Cancion* cargarCancionesDesdeArchivo(const string& ruta, int& numCanciones);
extern Usuario* cargarUsuariosDesdeArchivo(const string& ruta, int& numUsuarios);


int main() {

    Usuario usuarioLogueado;

    Usuario* usuariosDB = nullptr;
    int numUsuariosTotal = 0;
    int numCanciones = 0;

    Cancion* catalogoCanciones = nullptr;
    usuariosDB = cargarUsuariosDesdeArchivo("base_datos/usuarios.txt", numUsuariosTotal);
    catalogoCanciones = cargarCancionesDesdeArchivo("base_datos/canciones.txt", numCanciones);

    if (!catalogoCanciones || !usuariosDB) {
        cout << "Error critico: No se pudo cargar el catalogo o los usuarios. Saliendo." << endl;
        if (catalogoCanciones) delete[] catalogoCanciones;
        if (usuariosDB) delete[] usuariosDB;
        return 1;
    }


    size_t memoriaTotalInicial = calcularMemoriaTotal(numCanciones, numUsuariosTotal, 0);
    mostrarMetricas(memoriaTotalInicial, "CARGA INICIAL DE CATALOGO Y USUARIOS");


    bool exitoLogin = usuarioLogueado.iniciarSesion(
        catalogoCanciones,
        numCanciones,
        usuariosDB,
        numUsuariosTotal
        );


    if (exitoLogin) {

        int cancionesFinales = usuarioLogueado.miListaFavoritos.getNumCanciones();


        size_t memoriaFinal = calcularMemoriaTotal(numCanciones, numUsuariosTotal, cancionesFinales);


        mostrarMetricasFinales(memoriaFinal);
    }


    delete[] catalogoCanciones;
    delete[] usuariosDB;

    return 0;
}
