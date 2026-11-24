//
// Created by Maitena on 20/11/2026.
//

#ifndef CAJANIDO_H
#define CAJANIDO_H
#include "UTM.h"
#include "Arbol.h"

class CajaNido {
private:
    string distrito;
    string barrio;
    string ubicacion;
    UTM coordenadas;
    Arbol *itsTree;

public:
    CajaNido();
    CajaNido(const string &distrito,const string &barrio,const string &ubicacion,const UTM &coordenadas,Arbol *itsTree);
    CajaNido(const CajaNido &orig);
    ~CajaNido();

    void setDistrito(const string &distrito);
    string getDistrito() const;

    void setBarrio(const string &barrio);
    string getBarrio() const;

    void setUbicacion(const string &ubicacion);
    string getUbicacion() const;

    void setCoordenadas(double x, double y);
    UTM getCoordenadas() const;


    void setArbol(Arbol* a);
    Arbol* getArbol();
    bool estaEnArbol();
};



#endif //CAJANIDO_H
