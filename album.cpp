#include "publicidad.h"
#include "Album.h"
#include <fstream>
#include <cstdlib>
#include <cstring>

Album::Album()
    : idAlbum(0), idArtista(0), nombre(""), fecha(""), duracionTotal(0), sello(""), portadaRuta(""), puntaje(0), generos("") {}

Album::Album(int idA, int idArt, const string& nombre_, const string& fecha_, int durTotal,
             const string& sello_, const string& portada_, int puntaje_, const string& generos_)
    : idAlbum(idA), idArtista(idArt), nombre(nombre_), fecha(fecha_), duracionTotal(durTotal), sello(sello_), portadaRuta(portada_), puntaje(puntaje_), generos(generos_) {}

Album::~Album() {}

int Album::getId() const { return idAlbum; }
int Album::getArtistaId() const { return idArtista; }
const string& Album::getNombre() const { return nombre; }
const string& Album::getPortada() const { return portadaRuta; }
const string& Album::getGeneros() const { return generos; }

bool Album::parseFromLine(const std::string& line, Album &out) {
    if (line.empty()) return false;
    if (line[0] == '#') return false;

    // Soporta dos formatos:
    // recomendado: idAlbum,idArtista,nombre,duracion,fecha,sello,portada,puntaje,generos
    // minimo: idAlbum,nombre,duracion,fecha,sello,portada,puntaje,generos (idArtista = 0)
    int field = 0;
    size_t start = 0;
    size_t p = 0;
    int idAlbum_v = 0;
    int idArt_v = 0;
    string nombre_v;
    string dur_s;
    string fecha_v;
    string sello_v;
    string portada_v;
    string punt_s;
    string generos_v;

    while (p <= line.size()) {
        if (p == line.size() || line[p] == ',') {
            string token = line.substr(start, p - start);
            size_t a = 0, b = token.size();
            while (a < b && (token[a] == ' ' || token[a] == '\t')) ++a;
            while (b > a && (token[b-1] == ' ' || token[b-1] == '\t')) --b;
            string t = token.substr(a, b-a);

            switch (field) {
            case 0: idAlbum_v = std::atoi(t.c_str()); break;
            case 1:
                // aquí puede ser idArtista o el nombre si formato mínimo
                idArt_v = std::atoi(t.c_str());
                nombre_v = t; // provisional: si es nombre en formato mínimo quedará correcto luego
                break;
            case 2: nombre_v = t; break;
            case 3: dur_s = t; break;
            case 4: fecha_v = t; break;
            case 5: sello_v = t; break;
            case 6: portada_v = t; break;
            case 7: punt_s = t; break;
            case 8: generos_v = t; break;
            default: break;
            }
            ++field;
            start = p + 1;
        }
        ++p;
    }

    if (field == 8) {
        // formato mínimo: 0:idAlbum 1:nombre 2:dur 3:fecha 4:sello 5:portada 6:punt 7:generos
        int dur = std::atoi(dur_s.c_str());
        int punt = std::atoi(punt_s.c_str());
        out = Album(idAlbum_v, 0, nombre_v, fecha_v, dur, sello_v, portada_v, punt, generos_v);
        return true;
    }

    if (field < 9) return false;
    int dur = std::atoi(dur_s.c_str());
    int punt = std::atoi(punt_s.c_str());
    out = Album(idAlbum_v, idArt_v, nombre_v, fecha_v, dur, sello_v, portada_v, punt, generos_v);
    return true;
}

static size_t approxStrMem(const string &s) {
    return sizeof(string) + s.capacity();
}

bool Album::cargarAlbumsDesdeArchivo(const std::string& ruta, Album *&outArray, int &outCount, long &iterations, size_t &memBytes) {
    std::ifstream in(ruta.c_str());
    if (!in.is_open()) return false;
    outArray = nullptr;
    outCount = 0;
    iterations = 0;
    memBytes = 0;
    string line;
    while (std::getline(in, line)) {
        ++iterations;
        Album a;
        if (!Album::parseFromLine(line, a)) continue;
        Album* tmp = new Album[outCount + 1];
        for (int i = 0; i < outCount; ++i) tmp[i] = outArray[i];
        tmp[outCount] = a;
        delete[] outArray;
        outArray = tmp;
        ++outCount;
        memBytes += sizeof(Album) + approxStrMem(a.getNombre()) + approxStrMem(a.getPortada());
    }
    in.close();
    return true;
}
