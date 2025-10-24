#include "Artista.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <new>     // bad_alloc
#include <cstring> // solo si alguna dependencia lo requiere

//PRUEBA

using namespace std;

// Helpers
void Artista::liberarCatalogo() noexcept {
    if (catalogoAlbumes_) {
        for (int i = 0; i < numAlbumes_; ++i) {
            delete catalogoAlbumes_[i];
            catalogoAlbumes_[i] = nullptr;
        }
        delete[] catalogoAlbumes_;
        catalogoAlbumes_ = nullptr;
    }
    numAlbumes_ = 0;
    capacidadAlbumes_ = 0;
}

// Constructors / Destructor / Copy
Artista::Artista()
    : nombre_(), codigoIdentificador_(0), edad_(0), paisOrigen_(),
    cantidadSeguidores_(0), posicionTendencias_(0),
    catalogoAlbumes_(nullptr), numAlbumes_(0), capacidadAlbumes_(0) {}

Artista::Artista(const std::string& nombre, int codigo, int edad, const std::string& pais)
    : nombre_(nombre),
    codigoIdentificador_((codigo >= 10000 && codigo <= 99999) ? codigo : 0),
    edad_((edad >= 0 && edad <= 120) ? edad : 0),
    paisOrigen_(pais),
    cantidadSeguidores_(0),
    posicionTendencias_(0),
    catalogoAlbumes_(nullptr),
    numAlbumes_(0),
    capacidadAlbumes_(0)
{
    // validaciones ya realizadas en la lista de inicializadores
}

Artista::~Artista() noexcept {
    liberarCatalogo();
}

// Copia profunda (constructor de copia)
Artista::Artista(const Artista& otro)
    : nombre_(otro.nombre_), codigoIdentificador_(otro.codigoIdentificador_),
    edad_(otro.edad_), paisOrigen_(otro.paisOrigen_),
    cantidadSeguidores_(otro.cantidadSeguidores_), posicionTendencias_(otro.posicionTendencias_),
    catalogoAlbumes_(nullptr), numAlbumes_(0), capacidadAlbumes_(0)
{
    if (otro.capacidadAlbumes_ > 0) {
        Album** tmp = new Album*[otro.capacidadAlbumes_];
        for (int i = 0; i < otro.capacidadAlbumes_; ++i) tmp[i] = nullptr;
        try {
            for (int i = 0; i < otro.numAlbumes_; ++i) {
                if (otro.catalogoAlbumes_[i]) tmp[i] = new Album(*otro.catalogoAlbumes_[i]);
            }
        } catch (...) {
            for (int i = 0; i < otro.capacidadAlbumes_; ++i) delete tmp[i];
            delete[] tmp;
            throw;
        }
        catalogoAlbumes_ = tmp;
        capacidadAlbumes_ = otro.capacidadAlbumes_;
        numAlbumes_ = otro.numAlbumes_;
    }
}

// swap para copy-and-swap
void swap(Artista& a, Artista& b) noexcept {
    using std::swap;
    swap(a.nombre_, b.nombre_);
    swap(a.codigoIdentificador_, b.codigoIdentificador_);
    swap(a.edad_, b.edad_);
    swap(a.paisOrigen_, b.paisOrigen_);
    swap(a.cantidadSeguidores_, b.cantidadSeguidores_);
    swap(a.posicionTendencias_, b.posicionTendencias_);
    swap(a.catalogoAlbumes_, b.catalogoAlbumes_);
    swap(a.numAlbumes_, b.numAlbumes_);
    swap(a.capacidadAlbumes_, b.capacidadAlbumes_);
}

Artista& Artista::operator=(const Artista& otro) {
    if (this == &otro) return *this;
    Artista copia(otro); // strong guarantee
    swap(*this, copia);
    return *this;
}

// Private resize
void Artista::redimensionarAlbumes(int nuevaCapacidad) {
    if (nuevaCapacidad <= 0) return;
    Album** nuevo = new Album*[nuevaCapacidad];
    for (int i = 0; i < nuevaCapacidad; ++i) nuevo[i] = nullptr;
    for (int i = 0; i < numAlbumes_ && i < nuevaCapacidad; ++i) nuevo[i] = catalogoAlbumes_[i];
    delete[] catalogoAlbumes_;
    catalogoAlbumes_ = nuevo;
    capacidadAlbumes_ = nuevaCapacidad;
}

// Getters
const std::string& Artista::getNombre() const { return nombre_; }
int Artista::getCodigoIdentificador() const { return codigoIdentificador_; }
int Artista::getEdad() const { return edad_; }
const std::string& Artista::getPaisOrigen() const { return paisOrigen_; }
int Artista::getCantidadSeguidores() const { return cantidadSeguidores_; }
int Artista::getPosicionTendencias() const { return posicionTendencias_; }
int Artista::getNumAlbumes() const { return numAlbumes_; }

// Setters con validación básica
void Artista::setNombre(const std::string& n) { nombre_ = n; }
void Artista::setCodigoIdentificador(int codigo) {
    if (codigo >= 10000 && codigo <= 99999) codigoIdentificador_ = codigo;
}
void Artista::setEdad(int edad) { if (edad >= 0 && edad <= 120) edad_ = edad; }
void Artista::setPaisOrigen(const std::string& pais) { paisOrigen_ = pais; }
void Artista::setCantidadSeguidores(int seguidores) { if (seguidores >= 0) cantidadSeguidores_ = seguidores; }
void Artista::setPosicionTendencias(int posicion) { if (posicion >= 0) posicionTendencias_ = posicion; }

// Gestión de catálogo
bool Artista::agregarAlbum(const Album& album) {
    if (numAlbumes_ == capacidadAlbumes_) {
        int nueva = (capacidadAlbumes_ == 0) ? INITIAL_CAPACITY : capacidadAlbumes_ * 2;
        try {
            redimensionarAlbumes(nueva);
        } catch (const bad_alloc&) {
            return false;
        }
    }
    if (!catalogoAlbumes_) return false;
    Album* copia = nullptr;
    try {
        copia = new Album(album);
    } catch (...) {
        delete copia;
        return false;
    }
    catalogoAlbumes_[numAlbumes_] = copia;
    ++numAlbumes_;
    return true;
}

bool Artista::eliminarAlbum(int idAlbum) {
    if (!catalogoAlbumes_) return false;
    for (int i = 0; i < numAlbumes_; ++i) {
        if (catalogoAlbumes_[i] && catalogoAlbumes_[i]->getIdentificador() == idAlbum) {
            delete catalogoAlbumes_[i];
            for (int j = i; j < numAlbumes_ - 1; ++j) catalogoAlbumes_[j] = catalogoAlbumes_[j + 1];
            catalogoAlbumes_[numAlbumes_ - 1] = nullptr;
            --numAlbumes_;
            return true;
        }
    }
    return false;
}

Album* Artista::buscarAlbum(int idAlbum) {
    if (!catalogoAlbumes_) return nullptr;
    for (int i = 0; i < numAlbumes_; ++i) {
        if (catalogoAlbumes_[i] && catalogoAlbumes_[i]->getIdentificador() == idAlbum) return catalogoAlbumes_[i];
    }
    return nullptr;
}

const Album* Artista::buscarAlbum(int idAlbum) const {
    if (!catalogoAlbumes_) return nullptr;
    for (int i = 0; i < numAlbumes_; ++i) {
        if (catalogoAlbumes_[i] && catalogoAlbumes_[i]->getIdentificador() == idAlbum) return catalogoAlbumes_[i];
    }
    return nullptr;
}

void Artista::mostrarCatalogo() const {
    cout << "Catálogo de " << nombre_ << " (" << numAlbumes_ << " álbumes):" << endl;
    for (int i = 0; i < numAlbumes_; ++i) {
        if (catalogoAlbumes_[i]) {
            cout << "  " << i + 1 << ". " << catalogoAlbumes_[i]->getNombre()
            << " (ID: " << catalogoAlbumes_[i]->getIdentificador() << ")" << endl;
        }
    }
}

// Estadísticas
int Artista::getTotalCanciones() const {
    int total = 0;
    for (int i = 0; i < numAlbumes_; ++i) {
        if (catalogoAlbumes_[i]) total += catalogoAlbumes_[i]->getNumCanciones();
    }
    return total;
}

int Artista::getDuracionTotalDiscografia() const {
    int total = 0;
    for (int i = 0; i < numAlbumes_; ++i) {
        if (catalogoAlbumes_[i]) total += catalogoAlbumes_[i]->getDuracionTotal();
    }
    return total;
}

void Artista::mostrarEstadisticas() const {
    int totalCanciones = getTotalCanciones();
    int duracionTotal = getDuracionTotalDiscografia();
    int horas = duracionTotal / 3600;
    int minutos = (duracionTotal % 3600) / 60;
    int segundos = duracionTotal % 60;

    cout << "\n=== ESTADÍSTICAS DE " << nombre_ << " ===" << endl;
    cout << "  Código: " << codigoIdentificador_ << endl;
    cout << "  Edad: " << edad_ << " años | País: " << paisOrigen_ << endl;
    cout << "  Seguidores: " << cantidadSeguidores_ << " | Posición en tendencias: " << posicionTendencias_ << endl;
    cout << "  Álbumes: " << numAlbumes_ << " | Canciones: " << totalCanciones << endl;
    cout << "  Duración total discografía: " << horas << "h " << minutos << "m " << segundos << "s" << endl;
    cout << "=====================================" << endl;
}

// Operadores
bool Artista::operator==(const Artista& otro) const noexcept {
    return codigoIdentificador_ == otro.codigoIdentificador_;
}

bool Artista::operator>(const Artista& otro) const noexcept {
    return cantidadSeguidores_ > otro.cantidadSeguidores_;
}

// Output stream
ostream& operator<<(ostream& os, const Artista& artista) {
    int totalCanciones = artista.getTotalCanciones();
    int duracionTotal = artista.getDuracionTotalDiscografia();
    int horas = duracionTotal / 3600;
    int minutos = (duracionTotal % 3600) / 60;

    os << "\nARTISTA: " << artista.nombre_ << " (Cód. " << artista.codigoIdentificador_ << ")" << endl;
    os << "  Edad: " << artista.edad_ << " años | País: " << artista.paisOrigen_ << endl;
    os << "  Seguidores: " << artista.cantidadSeguidores_;
    if (artista.posicionTendencias_ > 0) os << " | Posición en tendencias: #" << artista.posicionTendencias_;
    os << endl;
    os << "  Discografía: " << artista.numAlbumes_ << " álbumes, " << totalCanciones << " canciones" << endl;
    os << "  Duración total: " << horas << "h " << minutos << "m" << endl;
    if (artista.numAlbumes_ > 0) {
        os << "  Álbumes: ";
        for (int i = 0; i < artista.numAlbumes_; ++i) {
            if (artista.catalogoAlbumes_[i]) {
                os << artista.catalogoAlbumes_[i]->getNombre();
                if (i < artista.numAlbumes_ - 1) os << ", ";
            }
        }
        os << endl;
    }
    os << "=====================================" << endl;
    return os;
}
