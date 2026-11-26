//
// Created by almar on 16/11/2026.
//

#ifndef PRACTICA5_HASH_THASHARBOL_H
#define PRACTICA5_HASH_THASHARBOL_H
#include <vector>

#include "Arbol.h"
enum Estado{
    LIBRE //casilla nunca ocupada
    ,OCUPADA,
    BORRADO //casilla que fue borrada, ahora disponible para insertar
};

class Entrada {
public:
    unsigned long clave;
    Estado estado;
    Arbol *dato;
    Entrada(const Estado &marca, Arbol *dato, unsigned long clave):estado(marca),dato(dato),clave(clave){}
    Entrada():estado(LIBRE),dato(),clave(0){}
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
    float lambda;
    vector <Entrada> tabla;

    unsigned int hash1(const unsigned long &clave, const int &intento);   //Función cuadrática
    unsigned int hash2(const unsigned long &clave, const int &intento);   //Función de dispersión doble 1
    unsigned int hash3(const unsigned long &clave, const int &intento);   //Función de dispersión doble 2

    unsigned int hash_auxiliar1(const unsigned long &clave);  //Función auxiliar para la dispersión doble 1
    unsigned int hash_auxiliar2(const unsigned long &clave);  //Función auxiliar para dispersion doble 2

public:
    ThashArbol() = default;
    int PrimoMayor(int numElementos);
    ThashArbol(int numElementos, float lambda = 0.75);
    ThashArbol(const ThashArbol &orig);
    ThashArbol &operator=(const ThashArbol &thash);
    ~ThashArbol();
    bool insertar(unsigned long clave, const Arbol &arbol);
    Arbol* buscar(unsigned long clave);
    bool borrar(unsigned long clave);
    unsigned int numElementos();
    unsigned long max_Colisiones();
    unsigned long numMax10();
    float promedioColisiones();
    float factorCarga() const;
    unsigned tamTabla() const;
    void mostrarEstadoTabla();

    //Metodos auxiliares
    bool esPrimo(const int &num);
    int calculoPrimoMenor();
    void redispersar(unsigned int tam);



};


#endif //PRACTICA5_HASH_THASHARBOL_H