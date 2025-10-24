#include "usuarios.h"
#include "Canciones.h"
#include <iostream>

using namespace std;

// NOTA: Se asume que la implementacion de cargarCancionesDesdeArchivo
// está definida en su archivo .cpp.

int main() {
    // 1. Cargar el catálogo de canciones
    int numCanciones = 0;
    Cancion* catalogoCanciones = cargarCancionesDesdeArchivo("base_datos/canciones.txt", numCanciones);

    if (!catalogoCanciones) {
        cout << "Error critico: No se pudo cargar el catalogo de canciones. Saliendo." << endl;
        return 1;
    }

    // 2. Crear un objeto Usuario
    Usuario usuarioLogueado;

    // 3. Llamar a iniciarSesion, pasando los datos de las canciones.
    // TODO EL FLUJO DE LOGIN Y REPRODUCCIÓN OCURRE DENTRO DE ESTA LLAMADA.
    usuarioLogueado.iniciarSesion(catalogoCanciones, numCanciones);

    // 4. Liberar la memoria del catálogo
    delete[] catalogoCanciones;
    return 0;
}
