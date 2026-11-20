//
// Created by almar on 05/10/2026.
//

#ifndef PRACTICA2_EEDD_CONTROLPARQUES_H
#define PRACTICA2_EEDD_CONTROLPARQUESH
#include <list>
#include <map>
#include <vector>

#include "Parque.h"
#include "ThashArbol.h"
#include "CajaNido.h"

using namespace std;
class ControlParques {
private:
    map<int, Parque> parques; //clave codigo de parque
    map<string, Especie> variantes;
    ThashArbol  arboles; //la clave es assetNum
    std::multimap<string, Arbol*> treeName; //clave nombre comun
    vector<CajaNido> nests;

public:
    ControlParques(const string &nomFichEspecies, const string &nomFichParques, const string &nomFiceroArbol);    //Hay que cargar los ficheros aqui
    ~ControlParques();

    Parque *buscarParque(const string &nombre);
    vector<Especie>& listadoEspecieSubNombre(const string &cadena);
    vector<Especie>& getVariantes();
    map<string, Especie>& getparques();
    ThashArbol& getArboles();


    Arbol *buscarArbol(const string &asnum);
    void eliminarParque(const string &nombre);
    vector<Arbol*>* listaArbolesParque(const string &nombre_parque);
    vector<Arbol*>& buscarArbolesRango(const string &asNumMin, const string &asNumMax);

    Arbol *getArbolMasAlto();
    bool buscar_tipo_planta(const TipoPlanta &tipo_planta,  vector<TipoPlanta> tipos);
    unsigned int contarEspecies(const string &nombreparque);

    unsigned int contarTipos();

    list<Parque> *listarParqueArbol(string nombre_comun);
    vector<Arbol*> *buscarArbolNombreComun(string nombre_comun);
    vector<Arbol*>* buscarArbolNombreComunComienzo(string ncomun);
    int contarArbolesBarrio(int num_barrio);
    void ordenarEspecies();

    Especie* especieMasComunEnParque(const string &nombreParque);

    list<Arbol*> *buscarArbolesPorEspecieYDistrito(const string &nombreComun, int numDistrito);


};


#endif //PRACTICA2_EEDD_CONTROLPARQUES_H