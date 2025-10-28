#include "Canciones.h"
#include "metricas.h"
#include "usuarios.h"
#include <iostream>
#include <string>

using namespace std;

#include "usuarios.h"
Usuario* DB_USUARIOS_GLOBAL = nullptr; int NUM_USUARIOS_GLOBAL = 0; long long CONTADOR_ITERACIONES_GLOBAL = 0;


void resetearContador() {
    CONTADOR_ITERACIONES_GLOBAL = 0;
}

void incrementarContador(long long cantidad) {
    CONTADOR_ITERACIONES_GLOBAL += cantidad;
}



size_t calcularMemoriaTotal(int numCancionesDB, int numUsuariosTotal, int numCancionesActuales) {

    size_t memCancionesFijas = (size_t)numCancionesDB * sizeof(Cancion);
    size_t memUsuariosFijas = (size_t)numUsuariosTotal * sizeof(Usuario);

    size_t memPunterosFavoritos = (size_t)numCancionesActuales * sizeof(Cancion*);

    size_t memPublicidad = (size_t)MAX_ADS * sizeof(MensajePublicitario);

    size_t memoriaTotal = memCancionesFijas + memUsuariosFijas + memPunterosFavoritos + memPublicidad; // 👈 SE SUMA
    incrementarContador(1);
    return memoriaTotal;

}




void mostrarMetricas(size_t memoriaTotalBytes, const string& funcionalidad) {
    cout << "\n=======================================" << endl;
    cout << "        RESUMEN DE RECURSOS - " << funcionalidad << endl;
    cout << "=======================================" << endl;
    cout << "1. Total de Iteraciones (Costo Temporal): " << CONTADOR_ITERACIONES_GLOBAL << endl;
    cout << "2. Consumo de Memoria Estimada: " << memoriaTotalBytes << " bytes" << endl;
    cout << "=======================================" << endl;
}


long long contadorOperaciones = 0;


void mostrarMetricasFinales(size_t memoriaTotal) {
    std::cout << "\n=======================================" << std::endl;
    std::cout << "         INFORME FINAL DE MÉTRICAS     " << std::endl;
    std::cout << "=======================================" << std::endl;


    std::cout << "Total de Operaciones Atómicas: " << CONTADOR_ITERACIONES_GLOBAL << std::endl;

    std::cout << "Memoria Estimada (Bytes): " << memoriaTotal << std::endl;
    std::cout << "---------------------------------------" << std::endl;
}
