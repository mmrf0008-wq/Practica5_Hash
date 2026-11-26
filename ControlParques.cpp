//
// Created by almar on 05/10/2026.
//

#include "ControlParques.h"
#include <algorithm>
#include <cmath>
#include <ranges>
#include <set>

#include "LectorCSV.h"

/**
 * @brief Constructor parametrizado de ControlParques. Carga especies, parques, árboles y cajas nido desde ficheros CSV.
 * @param nomFichEspecies Ruta del fichero CSV con los datos de las especies.
 * @param nomFichParques Ruta del fichero CSV con los datos de los parques.
 * @param nomFicheroArboles Ruta del fichero CSV con los datos de los árboles.
 * @param nomFicheroNido Ruta del fichero CSV con los datos de las cajas nido.
 * @param factorCarga Factor de carga inicial para la tabla hash de árboles.
 * @throw std::invalid_argument Si los nombres de ficheros de especies o parques están vacíos.
 * @throw std::runtime_error Si ocurre algún error al abrir o leer cualquiera de los ficheros CSV.
 */
ControlParques::ControlParques(const string &nomFichEspecies, const string &nomFichParques, const string &nomFicheroArboles, const string &nomFicheroNido
	, double factorCarga) {
	if(nomFichEspecies.empty() || nomFichParques.empty()) {
		throw invalid_argument("[ControlParques]: parametros no validos ");
	}
	if(!LectorCSV::cargarEspecies(this->variantes, nomFichEspecies)) {
		throw std::runtime_error("[ControlParques:cargaEspecies]: error al abrir fichero ");
	}
	cout << "cargadas las especies " << endl;

	LectorCSV a;

	if(!LectorCSV::cargarParques(this->parques, nomFichParques)) {
		throw std::runtime_error("[ControlParques:cargaParques]: error al abrir fichero ");
	}
	this->arboles = ThashArbol(134449, factorCarga); //ESTABLECEMOS EL FACTOR CARTA Y LA CANT DE ELEMENTOS

	cout << "cargados los parques" << endl;
	if(!a.cargosArbol(this->arboles, nomFicheroArboles, this->parques, this->variantes,treeName)) { // lectura de arboles y ligue entre arbol y parque
		throw std::runtime_error("[ControlParques:cargaArboles]: error al abrir fichero ");
	}
	cout << "Cargados los arboles" << endl;

	if(!LectorCSV::cargarNido(nomFicheroNido, this->nidos)) {
		throw std::runtime_error("[ControlParques:cargaNido]: error al abrir fichero ");
	}
	cout << "Cargados los nidos" << endl;

}

/**
 * @brief Destructor de la clase ControlParques. Libera las estructuras auxiliares.
 */
ControlParques::~ControlParques() {
	this->treeName.clear();
}

/**
 * @brief Busca un parque por su nombre exacto.
 * @param nombre Nombre del parque a buscar.
 * @return Puntero al objeto Parque si se encuentra, o nullptr en caso contrario.
 */
Parque * ControlParques::buscarParque(const string &nombre) {
	for(map<int,Parque>::iterator i = this->parques.begin(); i != this->parques.end(); i++) {
		if(i->second.getNombreParque() == nombre) {
			return &i->second;
		}
	}
	return nullptr;
}

/**
 * @brief Obtiene un listado de especies cuyo nombre científico contiene la subcadena indicada.
 * @param cadena Subcadena a buscar dentro del nombre científico.
 * @return Referencia a un vector dinámico con las especies que coinciden.
 */
vector<Especie> & ControlParques::listadoEspecieSubNombre(const string &cadena) {

	vector<Especie> *listad = new vector<Especie>();
	for(map<string,Especie>::iterator i = this->variantes.begin(); i != this->variantes.end(); i++) {
		if(i->second.get_nombre_cientifico().find(cadena)){
			listad->push_back(i->second);
		}
	}
	return *listad;
}

/**
 * @brief Devuelve el mapa de especies/variantes cargadas en el sistema.
 * @return Referencia al mapa de especies indexado por nombre.
 */
map<string, Especie> & ControlParques::getVariantes() {
	return this->variantes;
}

/**
 * @brief Devuelve el mapa de parques cargados en el sistema.
 * @return Referencia al mapa de parques indexado por su identificador numérico.
 */
map<int,Parque> & ControlParques::getparques() {
	return this->parques;
}

/**
 * @brief Devuelve la tabla hash que almacena todos los árboles.
 * @return Referencia a la estructura ThashArbol de árboles.
 */
ThashArbol & ControlParques::getArboles() {
	return this->arboles;
}

/**
 * @brief Busca un árbol en la tabla hash a partir de su número de activo (AssetNum).
 * @param asnum Cadena con el identificador único del árbol.
 * @return Puntero al Arbol encontrado, o nullptr si no existe.
 */
Arbol *ControlParques::buscarArbol(const string &asnum) {
	Arbol *a = this->arboles.buscar(djb2(asnum.c_str()));

	return a;
}

/**
 * @brief Elimina un parque del sistema dado su nombre.
 * @param nombre Nombre exacto del parque a eliminar.
 */
void ControlParques::eliminarParque(const string &nombre) {
	bool terminar = false;

	for(map<int,Parque>::iterator i= this->parques.begin(); i!=this->parques.end() &&!terminar; i++) {
		if(i->second.getNombreParque() == nombre) {
			this->parques.erase(i);
			terminar=true;
		}
	}
}

/**
 * @brief Ordena y devuelve los árboles pertenecientes a un parque concreto.
 * @param nombre_parque Nombre del parque del cual obtener los árboles.
 * @return Puntero al vector de punteros a Arbol del parque, o nullptr si el parque no existe.
 */
vector<Arbol*> * ControlParques::listaArbolesParque(const string &nombre_parque) {

	for(map<int,Parque>::iterator i = this->parques.begin(); i != this->parques.end(); i++) {
		if(i->second.getNombreParque() == nombre_parque) {
			i->second.ordenaArboles();

			return &i->second.get_contenedor();
		}
	}
	return nullptr;
}

/**
 * @brief Busca todos los árboles cuyos identificadores numéricos estén dentro de un rango [asNumMin, asNumMax].
 * @param asNumMin Límite inferior del rango de búsqueda (en formato cadena).
 * @param asNumMax Límite superior del rango de búsqueda (en formato cadena).
 * @return Referencia a un vector dinámico con los punteros a los árboles encontrados.
 * @throw std::invalid_argument Si la cota inferior es mayor que la superior.
 */
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
/**
 * @brief Busca el árbol de mayor altura entre todos los parques registrados.
 * @return Puntero al Arbol más alto encontrado, o nullptr si no hay árboles.
 */
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

/**
 * @brief Comprueba si un tipo de planta ya se encuentra dentro de un vector dado.
 * @param tipo_planta Tipo de planta a comprobar.
 * @param tipos Vector de tipos de planta donde realizar la búsqueda.
 * @return true si el tipo ya está en el vector, false en caso contrario.
 */
bool ControlParques::buscar_tipo_planta(const TipoPlanta &tipo_planta,  vector<TipoPlanta> tipos) {
	for(int i =0; i < tipos.size(); i++) {
		if(tipos[i] == tipo_planta) {
			return true;
		}
	}
	return false;
}

/**
 * @brief Cuenta cuántas especies distintas hay en un parque determinado.
 * @param nombreparque Nombre del parque a consultar.
 * @return Número de especies distintas en el parque (0 si el parque no existe).
 */
unsigned int ControlParques::contarEspecies(const string &nombreparque) {
	Parque *p = buscarParque(nombreparque);
	if(p) return p->contarEspecies();
	return 0;
}

/**
 * @brief Cuenta el número total de tipos de planta distintos (excluyendo NONE) presentes en todos los parques.
 * @return Cantidad de tipos de planta diferentes encontrados.
 */
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

/**
 * @brief Obtiene una lista con los parques que contienen al menos un árbol del nombre común indicado.
 * @param nombre_comun Nombre común del árbol a buscar.
 * @return Puntero a una lista dinámica de objetos Parque, o nullptr si no se encuentra ninguno.
 */
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

/**
 * @brief Busca todos los árboles cuyo nombre común coincida exactamente con el especificado.
 * @param nombre_comun Nombre común exacto a buscar.
 * @return Puntero a un vector dinámico con copias de los árboles encontrados, o nullptr si no hay coincidencias.
 */
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
/**
 * @brief Busca árboles cuya primera palabra del nombre común coincida con la cadena dada.
 * @param ncomun Primera palabra del nombre común a buscar.
 * @return Puntero a un vector dinámico con copias de los árboles encontrados, o nullptr si no hay coincidencias.
 */
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

/**
 * @brief Cuenta el número total de árboles ubicados en un barrio específico.
 * @param num_barrio Número identificador del barrio.
 * @return Cantidad de árboles en dicho barrio.
 */
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
/**
 * @brief Determina cuál es la especie más frecuente dentro de un parque concreto.
 * @param nombreParque Nombre del parque a analizar.
 * @return Puntero a la Especie más común en ese parque, o nullptr si el parque no existe.
 */
Especie * ControlParques::especieMasComunEnParque(const string &nombreParque) {

	for(map<int, Parque>::iterator i = this->parques.begin(); i != this->parques.end(); i++) {
		if(i->second.getNombreParque() == nombreParque) {
			return i->second.especieMasComun();
		}
	}
	return nullptr;
}
/**
 * @brief Calcula el código hash de una cadena de caracteres utilizando el algoritmo djb2.
 * @param str Cadena de caracteres terminada en nulo a dispersar.
 * @return Valor hash de tipo unsigned long generado.
 */
unsigned long ControlParques::djb2(const char *str) {
	unsigned long hash = 5381;
	int c;
	while (c = *str++) hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
	return hash;
}
/**
 * @brief Devuelve la cantidad total de cajas nido cargadas en el sistema.
 * @return Número de cajas nido almacenadas.
 */
unsigned int ControlParques::getTamNidos() const {
	return nidos.size();
}

/**
 * @brief Busca árboles que pertenezcan a un nombre común y a un número de distrito concretos.
 * @param nombreComun Nombre común del árbol.
 * @param numDistrito Número identificador del distrito.
 * @return Puntero a una lista dinámica de punteros a Arbol que cumplen ambas condiciones.
 */
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
 * @param ndistrito Nombre del distrito a consultar.
 * @return Conjunto (std::set) de punteros a los árboles con caja nido en ese distrito.
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
 * @brief Cuenta cuántas cajas nido están actualmente asociadas a algún árbol.
 * @return Cantidad de cajas nido instaladas en árboles.
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
 * @brief Devuelve todas las cajas nido ubicadas en el distrito indicado.
 * @param nbre_distrito Nombre del distrito a consultar.
 * @return Vector de punteros a las cajas nido ubicadas en dicho distrito.
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
/**
 * @brief Muestra por salida estándar las estadísticas de rendimiento y colisiones de la tabla hash de árboles.
 */
void ControlParques::mostrarEstadoTabla() {
	cout << "------------ESTADO DE LA TABLA HASH------------"<<endl;
	cout << "Colisiones en la insercion mas costosa: " << arboles.max_Colisiones() << endl;
	cout << "Veces que se superan las 10 colisiones: " << arboles.numMax10() << endl;
	cout << "Promedio de colisiones por insercion: " << arboles.promedioColisiones() << endl;
	cout << "Factor de carga de la tabla: " << arboles.factorCarga() << endl;
	cout << "Tamanio de la tabla: " << arboles.tamTabla() << endl;
}

/**
 * @brief Devuelve el tamaño físico actual de la tabla hash de árboles.
 * @return Capacidad total de la tabla hash.
 */
unsigned int ControlParques::getTamTabla() {
	return arboles.tamTabla();
}

/**
 * @brief Asigna cada caja nido a su árbol más cercano si la distancia al cuadrado entre ambos es menor a 100.
 */
void ControlParques::asginarNidosArboles() {

	bool asignado = false;
	double distancia =0;
	double masCercano= UINT_MAX;
	Arbol *arbol_mascercano= nullptr;
	int cont =0;
	int j=0;

	for(int i =0; i < this->nidos.size(); i++) {
		masCercano = UINT_MAX;

		for(multimap<string,Arbol*>::iterator it = this->treeName.begin(); it != this->treeName.end(); it++) {
			distancia = it->second->getCoordenadas().distancia2(nidos[i].getCoordenadas());
			if(distancia < masCercano) {
				masCercano = distancia;
				arbol_mascercano = it->second;
			}
		}
		if(masCercano < 100) {
			nidos[i].setArbol(arbol_mascercano);
		}
	}


}

/**
 * @brief Obtiene un puntero a la caja nido situada en la posición indicada del vector.
 * @param i Índice de la caja nido en el contenedor.
 * @return Puntero al objeto CajaNido correspondiente.
 */
CajaNido * ControlParques::getNido(int i) {
	return &this->nidos[i];
}
/**
 * @brief Obtiene el conjunto de parques que tienen al menos un árbol con una caja nido asignada.
 * @return Puntero a un std::set dinámico con los punteros a los parques correspondientes.
 */
set<Parque *> *ControlParques::parquesConCajaNido() {
	set<Parque*> *aux = new set<Parque*>();
	Parque *parque;

	for(int i =0;i < this->nidos.size(); i++) {
		if(nidos[i].getArbol()) {
			parque = this->nidos[i].getArbol()->getParque();
			if(parque && !aux->contains(parque)) {
				aux->insert(parque);
			}
		}
	}
	return aux;
}



