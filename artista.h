#ifndef ARTISTA_H
#define ARTISTA_H

#include <string>
#include <iosfwd>   // forward declaration de std::ostream
#include "Album.h"

class Artista {
public:
    static constexpr int INITIAL_CAPACITY = 4;

    // Regla de tres
    Artista();
    Artista(const std::string& nombre, int codigo, int edad, const std::string& pais);
    Artista(const Artista& otro);
    Artista& operator=(const Artista& otro);
    ~Artista() noexcept;

    // Getters
    const std::string& getNombre() const;
    int getCodigoIdentificador() const;
    int getEdad() const;
    const std::string& getPaisOrigen() const;
    int getCantidadSeguidores() const;
    int getPosicionTendencias() const;
    int getNumAlbumes() const;

    // Setters
    void setNombre(const std::string& n);
    void setCodigoIdentificador(int codigo); // valida 5 dígitos
    void setEdad(int edad);                   // valida >= 0
    void setPaisOrigen(const std::string& pais);
    void setCantidadSeguidores(int seguidores); // valida >= 0
    void setPosicionTendencias(int posicion);   // valida >= 0

    // Gestión del catálogo: Artista posee copias de Album (deep copy)
    bool agregarAlbum(const Album& album);     // agrega copia profunda, devuelve éxito
    bool eliminarAlbum(int idAlbum);           // elimina por identificador
    Album* buscarAlbum(int idAlbum);           // versión no-const
    const Album* buscarAlbum(int idAlbum) const; // versión const
    void mostrarCatalogo() const;              // imprime resumen

    // Estadísticas
    int getTotalCanciones() const;
    int getDuracionTotalDiscografia() const;   // en segundos
    void mostrarEstadisticas() const;

    // Operadores
    bool operator==(const Artista& otro) const noexcept; // comparación por código
    bool operator>(const Artista& otro) const noexcept;  // comparación por seguidores

    friend void swap(Artista& a, Artista& b) noexcept;
    friend std::ostream& operator<<(std::ostream& os, const Artista& artista);

private:
    void redimensionarAlbumes(int nuevaCapacidad);
    void liberarCatalogo() noexcept;

private:
    std::string nombre_;
    int codigoIdentificador_; // 5 dígitos
    int edad_;
    std::string paisOrigen_;
    int cantidadSeguidores_;
    int posicionTendencias_;

    Album** catalogoAlbumes_;
    int numAlbumes_;
    int capacidadAlbumes_;
};

#endif // ARTISTA_H
