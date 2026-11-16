//
// Created by almar on 16/11/2026.
//

#ifndef PRACTICA5_HASH_THASHARBOL_H
#define PRACTICA5_HASH_THASHARBOL_H
#include <vector>

#include "Arbol.h"
enum Estado{LIBRE,OCUPADA,DISPONIBLE};

class Entrada {
public:
    unsigned long clave;
    Estado marca;
    Arbol dato;
    Entrada(const Estado &_marca,const Arbol &dato, unsigned long clave):marca(LIBRE),dato(),clave(0){}
    ~Entrada(){}
};
class ThashArbol {
private:
    unsigned long tamaf;
    unsigned long tamal;
    unsigned long maxColisiones;
    unsigned long max10;
    unsigned long sumaColisiones;
    unsigned long primoMenor;
    vector <Entrada> tabla;

    unsigned hash1(unsigned long clave, int intento);
    unsigned hash2(unsigned long clave, int intento);
    unsigned hash3(unsigned long clave, int intento);

public:
    ThashArbol() = default;
    ThashArbol(int maxElementos, float lambda = 0.7);
    ThashArbol(ThashArbol &hash);
    ThashArbol &operator=(ThashArbol &thash);
    ~ThashArbol();
    bool insertar(unsigned long clave, const Arbol &arbol);
    Arbol* buscar(unsigned long clave);
    bool borrar(unsigned long clave);
    unsigned int numElementos();


};


#endif //PRACTICA5_HASH_THASHARBOL_H