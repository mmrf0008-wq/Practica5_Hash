
#include "ThashArbol.h"

unsigned ThashArbol::hash1(unsigned long clave, int intento) {
}

unsigned ThashArbol::hash2(unsigned long clave, int intento) {
}

unsigned ThashArbol::hash3(unsigned long clave, int intento) {
}

ThashArbol::ThashArbol(int maxElementos, float lambda):tamaf(),lambda() {}

ThashArbol::ThashArbol(ThashArbol &orig):tamaf(orig.tamaf),tamal(orig.tamal),maxColisiones(orig.maxColisiones),
            max10(orig.max10),sumaColisiones(orig.sumaColisiones),primoMenor(orig.primoMenor),
            lambda(orig.lambda),tabla(orig.tabla) {}

ThashArbol & ThashArbol::operator=(ThashArbol &thash) {
    if (this!=&thash) {
        this->tamaf = thash.tamaf;
        this->tamal = thash.tamal;
        this->maxColisiones = thash.maxColisiones;
        this->max10 = thash.max10;
        this->sumaColisiones = thash.sumaColisiones;
        this->primoMenor = thash.primoMenor;
        this->lambda = thash.lambda;
        this->tabla = thash.tabla;
    }
    return *this;
}

ThashArbol::~ThashArbol() {
}

bool ThashArbol::insertar(unsigned long clave, const Arbol &arbol) {
}

Arbol * ThashArbol::buscar(unsigned long clave) {
}

bool ThashArbol::borrar(unsigned long clave) {
}

unsigned int ThashArbol::numElementos() {
}

int ThashArbol::calculoPrimoMenor() {
}

unsigned long ThashArbol::max_Colisiones() {
    return this->maxColisiones;
}

unsigned long ThashArbol::numMax10() {
}

float ThashArbol::promedioColisiones() {
    return (float)sumaColisiones/(float)tamal;  //Transformando un entero en un float en tiempo de ejecución
}

float ThashArbol::factorCarga() {
    return this->lambda;
}

unsigned ThashArbol::tamTabla() {
    return this->tamaf;
}

void ThashArbol::mostrarEstadoTabla() {

}
