//
// Created by almar on 05/10/2026.
//

#include "ControlParques.h"
#include <algorithm>
#include <ranges>
#include <set>

#include "LectorCSV.h"


ControlParques::ControlParques(const string &nomFichEspecies, const string &nomFichParques, const string &nomFicheroArboles) {
	if(nomFichEspecies.empty() || nomFichParques.empty()) {
		throw invalid_argument("[ControlParques]: parametros no validos ");
	}
	if(!LectorCSV::cargarEspecies(this->variantes, nomFichEspecies)) {
		throw std::runtime_error("[ControlParques:cargaEspecies]: error al abrir fichero ");
	}
	cout << "cargadas las especies " << endl;
	  ordenarEspecies();   //ordenamos por id de menor

	LectorCSV a;

	if(!LectorCSV::cargarParques(this->parques, nomFichParques)) {
		throw std::runtime_error("[ControlParques:cargaParques]: error al abrir fichero ");
	}
	cout << "cargados los parques" << endl;
	if(!a.cargosArbol(this->arboles, nomFicheroArboles, this->parques, this->variantes,treeName)) { // lectura de arboles y ligue entre arbol y parque
		throw std::runtime_error("[ControlParques:cargaArboles]: error al abrir fichero ");
	}
	cout << "Cargados los arboles" << endl;

}


ControlParques::~ControlParques() {
	this->variantes.clear();
}


Parque * ControlParques::buscarParque(const string &nombre) {
	for(map<int,Parque>::iterator i = this->parques.begin(); i != this->parques.end(); i++) {
		if(i->second.getNombreParque() == nombre) {
			return &i->second;
		}
	}
	return nullptr;
}


vector<Especie> & ControlParques::listadoEspecieSubNombre(const string &cadena) {

	vector<Especie> *listad = new vector<Especie>();
	for(map<string,Especie>::iterator i = this->variantes.begin(); i != this->variantes.end(); i++) {
		if(i->second.get_nombre_cientifico().find(cadena)){
			listad->push_back(i->second);
		}
	}
	return *listad;
}


map<string, Especie> & ControlParques::getVariantes() {
	return this->variantes;
}


map<int,Parque> & ControlParques::getparques() {
	return this->parques;
}


ThashArbol & ControlParques::getArboles() {
	return this->arboles;
}


Arbol *ControlParques::buscarArbol(const string &asnum) {
	return this->arboles.buscar(djb2(asnum.c_str()));
}


void ControlParques::eliminarParque(const string &nombre) {
	bool terminar = false;

	for(map<int,Parque>::iterator i= this->parques.begin(); i!=this->parques.end() &&!terminar; i++) {
		if(i->second.getNombreParque() == nombre) {
			this->parques.erase(i);
			terminar=true;
		}
	}
}


vector<Arbol*> * ControlParques::listaArbolesParque(const string &nombre_parque) {

	for(map<int,Parque>::iterator i = this->parques.begin(); i != this->parques.end(); i++) {
		if(i->second.getNombreParque() == nombre_parque) {
			i->second.ordenaArboles();

			return &i->second.get_contenedor();
		}
	}
	return nullptr;
}


vector<Arbol *> & ControlParques::buscarArbolesRango(const string &asNumMin, const string &asNumMax) {
	vector<Arbol*> *nuevo = new vector<Arbol *>;

	if(asNumMin > asNumMax) {
		throw invalid_argument("[BUSCAR_ARBOLES_RANGO] se a introducido una cota inferior con valor mayor que la cota superior");
	}

	int min = stoi(asNumMin);
	int max = stoi (asNumMax);
	Arbol *aux;
	for(int i=min; i <= max; i++) {
		aux = this->buscarArbol(std::to_string(i));
		if(aux) nuevo->push_back(aux);
	}

	return *nuevo;
}

Arbol *ControlParques::getArbolMasAlto() {

	Arbol *encontrado = nullptr;
	Arbol *aux; 

	for(map<int,Parque>::iterator i = this->parques.begin(); i != this->parques.end(); i++) {

		aux = i->second.arbolMasAlto();
		if((aux ) && (!encontrado || aux->get_altura() > encontrado->get_altura())) {
			encontrado = aux;
		}
	}
	return encontrado; 
}


bool ControlParques::buscar_tipo_planta(const TipoPlanta &tipo_planta,  vector<TipoPlanta> tipos) {
	for(int i =0; i < tipos.size(); i++) {
		if(tipos[i] == tipo_planta) {
			return true;
		}
	}
	return false;
}


unsigned int ControlParques::contarEspecies(const string &nombreparque) {
	Parque *p = buscarParque(nombreparque);
	if(p) return p->contarEspecies();
	return 0;
}


unsigned int ControlParques::contarTipos() {

	vector<TipoPlanta> encontradas;
	TipoPlanta aux;
	for (map<int, Parque>::iterator i = this->parques.begin(); i != this->parques.end(); i++ ){
		for(int j=0; j < i->second.get_contenedor().size(); j++ ) {
			aux = Especie::stringtotp(  i->second.get_contenedor()[j]->getTipoEspecie());
			if( !buscar_tipo_planta(aux, encontradas) && aux != TipoPlanta::NONE) {
				encontradas.push_back(aux);
			}
		}
	}
	return encontradas.size();
}


list<Parque>* ControlParques::listarParqueArbol(string nombre_comun) {

	list<Parque> * encontrados = new list<Parque>();
	bool hay = false;

	//recorremos los parques
	for(map<int, Parque>::iterator i = this->parques.begin(); i!= this->parques.end(); i++) {
		hay = false;
		//recorremos los arboles dentro de los parques
		for(vector<Arbol*>::iterator j = i->second.get_contenedor().begin(); j != i->second.get_contenedor().end() && !hay; j++ ) {
			if(j.operator*()->getNombreComun() == nombre_comun ) {
				hay = true;
			}
		}
		if(hay) encontrados->push_back(i->second);
	}
	if(encontrados->size() >0) return  encontrados;

	return nullptr;
}


vector<Arbol *>* ControlParques::buscarArbolNombreComun(string nombre_comun) {
	vector<Arbol*> *encontrados = new vector<Arbol *>();

	for(multimap<string,Arbol*>::iterator i = this->treeName.begin(); i != this->treeName.end(); i++) {
		if(i->second->getNombreComun() == nombre_comun) {
			encontrados->push_back(new Arbol(*i->second));
		}
	}
	if(encontrados->size() >0) return encontrados;

	return nullptr;
}

vector<Arbol*> *ControlParques::buscarArbolNombreComunComienzo(string ncomun) {

	vector<Arbol*> *encontrados = new vector<Arbol *>();

	string aux;
	for(multimap<string,Arbol*>::iterator i = this->treeName.begin(); i != this->treeName.end(); i++) {

		size_t pos = i->second->getNombreComun().find(" ");
		aux = i->second->getNombreComun().substr(0, pos);
		if(aux == ncomun) {
			encontrados->push_back(new Arbol(*i->second));
		}
	}
	if(encontrados->size() >0) return encontrados;

	return nullptr;
}


int ControlParques::contarArbolesBarrio(int num_barrio) {

	int contador=0;

	//recorremos todos los arboles
	for(multimap<string,Arbol*>::iterator i = this->treeName.begin(); i != this->treeName.end(); i++) {
		if(i->second->get_num_barrio() == num_barrio) {
			contador++;
		}
	}

	return contador;
}

Especie * ControlParques::especieMasComunEnParque(const string &nombreParque) {

	for(map<int, Parque>::iterator i = this->parques.begin(); i != this->parques.end(); i++) {
		if(i->second.getNombreParque() == nombreParque) {
			return i->second.especieMasComun();
		}
	}
	return nullptr;
}

unsigned long ControlParques::djb2(const char *str) {
	unsigned long hash = 5381;
	int c;
	while (c = *str++) hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
	return hash;
}

unsigned int ControlParques::getTamNidos() const {
	return nidos.size();
}


list<Arbol*> *ControlParques::buscarArbolesPorEspecieYDistrito(const string &nombreComun, int numDistrito) {
	list<Arbol *> *lista = new list<Arbol *>();

	for(multimap<string,Arbol*>::iterator i = this->treeName.begin(); i != this->treeName.end(); i++) {
		if(i->second->getNombreComun() == nombreComun && i->second->getNumDistrito() == numDistrito) {
			lista->push_back(i->second);
		}
	}
	return lista;
}

/**
* @brief Devuelve todos los árboles del distrito indicado que tengan al menos una caja nido asociada.
 * @param ndistrito
 * @return
 */
set<Arbol*> ControlParques::arbolesCajaDistrito(const string& ndistrito) {
	set<Arbol*> arbolesConCaja;
	for (int i = 0; i < nidos.size(); i++) {
		if (nidos[i].getArbol() != nullptr && nidos[i].getArbol()->getNombreDistrito() == ndistrito) {
			arbolesConCaja.insert(nidos[i].getArbol());
		}
	}

	return arbolesConCaja;
}

/**
 * @brief Cuenta cuantas cajasNido estan asociadas a un arbol
 * @return
 */
unsigned int ControlParques::contarCajasEnArbol() {
	unsigned int contador = 0;

	for (int i = 0; i < nidos.size(); i++) {
		if (nidos[i].estaEnArbol() == true) {
			contador++;
		}
	}

	return contador;
}

/**
* @brief Devuelve todas las cajas nido ubicadas en el distrito indicado
 * @param nbre_distrito
 * @return
 */
vector<CajaNido*> ControlParques::cajasEnDistrito(const string& nbre_distrito) {
	vector<CajaNido*> vectorNidos;

	for (int i = 0; i < nidos.size(); i++) {
		if (nidos[i].getDistrito() == nbre_distrito) {
			vectorNidos.push_back(&nidos[i]);
		}
	}

	return vectorNidos;
}

void ControlParques::mostrarEstadoTabla() {
	cout << "------------ESTADO DE LA TABLA HASH------------"<<endl;
	cout << "Colisiones en la insercion mas costosa: " << arboles.max_Colisiones() << endl;
	cout << "Veces que se superan las 10 colisiones: " << arboles.numMax10() << endl;
	cout << "Promedio de colisiones por insercion: " << arboles.promedioColisiones() << endl;
	cout << "Factor de carga de la tabla: " << arboles.factorCarga() << endl;
	cout << "Tamanio de la tabla: " << arboles.tamTabla() << endl;
}

unsigned int ControlParques::getTamTabla() {
	return arboles.tamTabla();
}

