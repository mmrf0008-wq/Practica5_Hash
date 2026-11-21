//
// Created by Maitena on 20/11/2026.
//

#include "CajaNido.h"

#include "Arbol.h"

CajaNido::CajaNido():distrito(""), barrio(""),ubicacion(""),coordenadas(0,0),itsTree(nullptr) {}

CajaNido::CajaNido(const string &distrito,const string &barrio,const string &ubicacion,const UTM &coordenadas,Arbol *itsTree):
                distrito(distrito),barrio(barrio),ubicacion(ubicacion),coordenadas(coordenadas),itsTree(itsTree) {}

CajaNido::CajaNido(const CajaNido &orig):distrito(orig.distrito),barrio(orig.barrio),ubicacion(orig.ubicacion),
                            coordenadas(orig.coordenadas),itsTree(orig.itsTree) {}

CajaNido::~CajaNido() {}

void CajaNido::setDistrito(const string &distrito) {
    this->distrito = distrito;
}

string CajaNido::getDistrito() const {
    return this->distrito;
}

void CajaNido::setBarrio(const string &barrio) {
    this->barrio = barrio;
}

string CajaNido::getBarrio() const {
    return this->barrio;
}

void CajaNido::setUbicacion(const string &ubicacion) {
    this->ubicacion = ubicacion;
}

string CajaNido::getUbicacion() const {
    return this->ubicacion;
}

void CajaNido::setArbol(Arbol *a) {
    this->itsTree = a;
}

Arbol * CajaNido::getArbol() {
    return itsTree;
}

void CajaNido::setCoordenadas(double x, double y) {
    this->coordenadas = UTM(x,y);
}

UTM CajaNido::getCoordenadas() const {
    return coordenadas;
}

bool CajaNido::estaEnArbol() {
    if (itsTree != nullptr) {
        return true;
    }
    return false;
}