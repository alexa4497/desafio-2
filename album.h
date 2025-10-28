
#ifndef ALBUM_H
#define ALBUM_H

#include <string>

using namespace std;

class Album {
public:
    int idAlbum;
    int idArtista;
    string nombre;
    string fecha;
    int duracionTotal;
    string sello;
    string portadaRuta;
    int puntaje;
    string generos;
public:
    Album();
    Album(int idA, int idArt, const string& nombre, const string& fecha, int durTotal,
          const string& sello, const string& portada, int puntaje, const string& generos);
    ~Album();

    int getId() const;
    int getArtistaId() const;
    const string& getNombre() const;
    const string& getPortada() const;
    const string& getGeneros() const;

    // parsea una línea de album
    static bool parseFromLine(const std::string& line, Album &out);
    bool cargarAlbumsDesdeArchivo(const std::string& ruta, Album *&outArray, int &outCount, long &iterations, size_t &memBytes);
};

#endif // ALBUM_H
