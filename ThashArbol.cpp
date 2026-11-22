
#include "ThashArbol.h"
#include <cmath>

/**
 * @brief funcion cuadratica de hash
 * @param clave
 * @param intento
 * @return
 */
unsigned int ThashArbol::hash1(const unsigned long &clave, const int &intento) {
//h(x)  = ( x+i^2) %t;
    return (clave + (intento * intento)) % this->tamaf;
}

/**
 * @brief Metodo hash auxiliar con una funcion hash distinta para ser usada en la dispersión doble
 * @param clave
 * @return
 */
unsigned int ThashArbol::hash_auxiliar1(const unsigned long &clave) {
    return 1 + (clave % primoMenor);
}

/**
 * @brief Primera funcion de dispersion doble (usando dispersion cuadratica y una auxiliar)
 * @param clave
 * @param intento
 * @return
 * @pre creada la funcion de calculo del sigueinte valor primo primo
 */
unsigned int ThashArbol::hash2(const unsigned long &clave, const int &intento) {
    //h(x) = (h1(x) + i ⋅ h2 (x)) % t
    return (hash1(clave,intento) + intento * hash_auxiliar1(clave)) % tamaf;
}

/**
 * @brief funcion de dispersion lineal
 * @param clave
 * @param intento
 * @return
 */
unsigned int ThashArbol::hash_auxiliar2(const unsigned long &clave) {
//h(x) = (x + i) % t;
    return primoMenor - (clave % primoMenor);
}

/**
 * @brief Segunda función de dispersión doble(usando dispersion cuadratica y un metodo auxiliar)
 * @param clave
 * @param intento
 * @return
 */
unsigned int ThashArbol::hash3(const unsigned long &clave, const int &intento) {
    return (hash1(clave, intento) + intento * hash_auxiliar2(clave)) % tamaf;
}

/**
 * @brief Calcula el numero primo siguiente al numero de elementos que metemos en el vector
 * @param numElementos
 * @return
 */

int ThashArbol::PrimoMayor(int numElementos) {
    while (!esPrimo(numElementos)) {
        numElementos++;
    }
    return numElementos;
}

/**
* @brief Constructor que construye una tabla garantizando que haya un
* factor de carga determinado al insertar todos los datos previstos.
 * @param numElementos
 * @param lambda
 */
ThashArbol::ThashArbol(int numElementos, float lambda):tamaf(PrimoMayor(numElementos/lambda)),lambda(lambda),tamal(0),
                                                       maxColisiones(0),max10(0),sumaColisiones(0),tabla(tamaf)
{
    primoMenor = calculoPrimoMenor();
}

/**
 * @brief Constructor por copia de la clase ThashArbol
 * @param orig
 */
ThashArbol::ThashArbol(const ThashArbol &orig):tamaf(orig.tamaf),tamal(orig.tamal),maxColisiones(orig.maxColisiones),
                                         max10(orig.max10),sumaColisiones(orig.sumaColisiones),primoMenor(orig.primoMenor),
                                         lambda(orig.lambda),tabla(orig.tabla) {}

/**
 * @brief Operador = de la clase ThashArbol
 * @param thash
 * @return
 */
ThashArbol& ThashArbol::operator=(const ThashArbol &thash) {
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

/**
 * @brief Destructor de la clase ThashArbol
 */
ThashArbol::~ThashArbol() {}

/**
 * @brief inserte un arbol en la tabla. Como NO SE PERMITEN REPETIDOS, si se localiza la clave en la
 * secuencia de exploración, NO se realiza la insercion, devolviendo false
 * @param clave
 * @param arbol
 * @return
 **/
bool ThashArbol::insertar(unsigned long clave, const Arbol &arbol) {    //FALTA COMPROBAR SI HAY QUE AUMENTAR LA TABLA!!!(Parejas)
    bool insertado = false;
    int intento = 0, pos = 0;

    while (insertado == false) {
        pos = hash3(clave,intento);     //Calculo una posicion aleatoria con una de las funciones hash
        if (tabla[pos].estado == LIBRE || tabla[pos].estado == BORRADO) {    //Compruebo si la posicion está libre
            tabla[pos].dato = arbol;                            //Si lo está, inserto el nuevo dato
            tabla[pos].clave = clave;
            tabla[pos].estado = OCUPADA;        //Esta posicion pasa de LIBRE a OCUPADA
            tamal++;                            //Aumento el tamaño logico de la tabla
            insertado = true;
        } else {
            intento++;      //Si no está libre, incrementamos el intento y volvemos a realizar el calculo de la posicion
        }
    }
    return insertado;
}

//LA IA DICE QUE ESTA MAL
Arbol * ThashArbol::buscar(unsigned long clave) {
    int intento = 0, pos = 0;

    while ( pos < tamaf ) {
        pos = hash3(clave, intento);
        if (tabla[pos].estado == OCUPADA && tabla[pos].clave == clave) {
            return &tabla[pos].dato;
        }
        intento++;
    }

    return nullptr;
}

//LA IA DICE QUE ESTÁ MAL
bool ThashArbol::borrar(unsigned long clave) {
    int intento = 0, pos = 0;
    bool borrado = false;

    while (borrado == false && pos < tamaf) {
        pos = hash3(clave, intento);
        if (tabla[pos].estado == OCUPADA && tabla[pos].clave == clave) {
            tabla[pos].clave = 0;
            tabla[pos].estado = BORRADO;
            tamal--;
            borrado = true;
        }
        intento ++;
    }

    return borrado;
}

/**
* @brief devuelva de forma eficiente el número de
elementos que contiene la tabla.
 * @return
 */
unsigned int ThashArbol::numElementos() {
    return tamal;
}

/**
 * @brief Devuelve un booleano que indica si un numero es primo (true) o no (false)
 * @param num
 * @return
 */
bool ThashArbol::esPrimo(const int &num) {
    if (num <= 1) {
        return false;
    }
    for(int i=2; i * i <= num; i++) {
        if(num % i == 0) {
            return false;
        }
    }
    return true;
}

/**
 * @brief Calcula el numero primo mayor, que a su vez sea menor que el tamaño de la tabla
 * @return
 */
int ThashArbol::calculoPrimoMenor() {
    for (int i = tamaf - 1; i > 0 ; i--) {
        if (esPrimo(i) == true) {
            return i;
        }
    }

    return -1;
}

/**
* @brief Devuelve el número máximo de colisiones que se han producido
en la operación de inserción más costosa realizada sobre la tabla
 * @return
 */
unsigned long ThashArbol::max_Colisiones() {
    return this->maxColisiones;
}

/**
* @brief Devuelve el número de veces que se superan 10 colisiones al
intentar realizar la operación de inserción sobre la tabla de un dato.
 * @return
 */
unsigned long ThashArbol::numMax10() {
    return max10;
}

/**
 * @brief Calcula la cantidad de colisiones que hay (Las colisiones totales entre el numero total de elementos que hay)
 * @return
 */
float ThashArbol::promedioColisiones() {
    return (float)sumaColisiones/(float)tamal;
}

/**
 * @brief Devuelve el factor de carga de la tabla
 * @return
 */
float ThashArbol::factorCarga() const {
    return (float)tamal / (float)tamaf;
}

/**
 * @brief Devuelve el tamaño total de la tabla
 * @return
 */
unsigned ThashArbol::tamTabla() const {
    return this->tamaf;
}

/**
 * @brief Muestra por pantalla el tamaño y el factor de carga de la tabla, el maximo de colisiones
 * que se alcanzó, las veces que se superaron las 10 colisiones y el promedio de colisiones por insercion
 */
void ThashArbol::mostrarEstadoTabla() {
    cout << "------------ESTADO DE LA TABLA HASH------------"<<endl;
    cout << "Colisiones en la insercion mas costosa: " << max_Colisiones() << endl;
    cout << "Veces que se superan las 10 colisiones: " << numMax10() << endl;
    cout << "Promedio de colisiones por insercion: " << promedioColisiones() << endl;
    cout << "Factor de carga de la tabla: " << factorCarga() << endl;
    cout << "Tamanio de la tabla: " << tamTabla() << endl;
}
