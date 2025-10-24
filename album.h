#ifndef ALBUM_H
#define ALBUM_H

#include "canciones.h"
#include <cstddef>
#include <cstdio>

using namespace std;

class Album {
public:
    static const int MAX_GENEROS = 4;
    static const int INITIAL_CAPACITY = 10;
    static const char* const GENE_LIST[]; // Declaración de lista de géneros permitidos
    static const int GENE_LIST_COUNT;

    // Regla de tres
    Album();
    Album(const char* nombre, int id, const char* fecha, const char* sello, const char* portada);
    Album(const Album& otro);
    Album& operator=(const Album& otro);
    ~Album();

    const char* getNombre() const;
    int getIdentificador() const;
    const char* getFechaLanzamiento() const;
    int getDuracionTotal() const;
    const char* getSelloDisquero() const;
    const char* getPortada() const;
    float getPuntuacion() const;
    int getNumCanciones() const;
    int getNumGeneros() const;

    // Setters (copian la cadena)
    void setNombre(const char* n);
    void setIdentificador(int id);
    void setFechaLanzamiento(const char* fecha);
    void setSelloDisquero(const char* sello);
    void setPortada(const char* port); // valida extensión .png
    void setPuntuacion(float punt); // valida 1..10

    // Generos
    bool agregarGenero(const char* genero);
    bool tieneGenero(const char* genero) const;
    void mostrarGeneros() const; // imprime en stdout

    // Canciones
    void agregarCancion(const Cancion& cancion); // deep copy
    bool eliminarCancion(const char* idCancion); // busca por id (cadena terminada en '\0')
    void mostrarCanciones() const; // imprime en stdout

    // Utilidad
    void actualizarDuracionTotal();

    bool operator==(const Album& otro) const;

private:
    char* duplicarCadena(const char* s) const;
    void liberarCadenas();
    void inicializarGeneros();
    void inicializarSlotsCanciones(int from, int to);
    void asegurarCapacidad(int minCapacity);

    friend void swapAlbum(Album& a, Album& b) noexcept;

private:
    // Datos
    char* nombre_;
    int identificador_;
    char* fechaLanzamiento_;
    int duracionTotal_; // en segundos
    char* selloDisquero_;
    char* portada_;
    float puntuacion_;

    // Generos
    char* generos_[MAX_GENEROS];
    int numGeneros_;

    // Canciones: array dinámico de punteros a Cancion
    Cancion** canciones_;
    int numCanciones_;
    int capacidadCanciones_;
};

#endif // ALBUM_H
