#ifndef METRICAS_H
#define METRICAS_H

#include <string>
#include <iostream>
#include <cstddef>

const int MAX_CANCIONES_METRICAS = 10000;


extern long long CONTADOR_ITERACIONES_GLOBAL;

void resetearContador();
void incrementarContador(long long cantidad = 1);
size_t calcularMemoriaTotal(int numCancionesCatalogo, int numUsuariosTotal, int numFavoritosActual);
void mostrarMetricas(size_t memoriaTotalBytes, const std::string& funcionalidad);
// Necesita recibir la memoria calculada
void mostrarMetricasFinales(size_t memoriaTotal);
// Y asegurar que la variable global del contador sea accesible (extern)
extern long long contadorOperaciones;

#endif // METRICAS_H
