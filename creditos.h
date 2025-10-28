#ifndef CREDITOS_H
#define CREDITOS_H

#include <string>

using namespace std;


struct Colaborador {
    string nombres;
    string apellidos;
    string codigoAfiliacion;
};

class Creditos {
public:

    Colaborador* productores;
    int numProductores;
    Colaborador* musicos;
    int numMusicos;
    Colaborador* compositores;
    int numCompositores;
};

#endif // CREDITOS_H
