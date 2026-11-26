
#include "ThashArbol.h"
#include <cmath>

/**
 * @brief Función de dispersión cuadrática (hash1).
 * @param clave Clave numérica obtenida del identificador del árbol.
 * @param intento Número de intento actual en la secuencia de exploración.
 * @return Posición calculada dentro de la tabla hash [0, tamaf - 1].
 */
unsigned int ThashArbol::hash1(const unsigned long &clave, const int &intento) {
//h(x)  = ( x+i^2) %t;
    return (clave + (intento * intento)) % this->tamaf;
}

/**
 * @brief Primera función hash auxiliar para la dispersión doble.
 * @param clave Clave numérica del elemento.
 * @return Incremento de salto comprendido entre 1 y primoMenor.
 */
unsigned int ThashArbol::hash_auxiliar1(const unsigned long &clave) {
    return 1 + (clave % primoMenor);
}

/**
 * @brief Primera función de dispersión doble (combina exploración cuadrática y hash_auxiliar1).
 * @param clave Clave numérica del elemento.
 * @param intento Número de intento actual en la secuencia de exploración.
 * @return Posición calculada dentro de la tabla hash [0, tamaf - 1].
 * @pre El atributo primoMenor debe haber sido calculado previamente.
 */
unsigned int ThashArbol::hash2(const unsigned long &clave, const int &intento) {
    //h(x) = (h1(x) + i ⋅ h2 (x)) % t
    return (hash1(clave,intento) + intento * hash_auxiliar1(clave)) % tamaf;
}

/**
 * @brief Segunda función hash auxiliar para la dispersión doble.
 * @param clave Clave numérica del elemento.
 * @return Incremento de salto comprendido entre 1 y primoMenor.
 */
unsigned int ThashArbol::hash_auxiliar2(const unsigned long &clave) {
//h(x) = (x + i) % t;
    return primoMenor - (clave % primoMenor);
}

/**
 * @brief Segunda función de dispersión doble (combina exploración cuadrática y hash_auxiliar2).
 * @param clave Clave numérica del elemento.
 * @param intento Número de intento actual en la secuencia de exploración.
 * @return Posición calculada dentro de la tabla hash [0, tamaf - 1].
 */
unsigned int ThashArbol::hash3(const unsigned long &clave, const int &intento) {
    return (hash1(clave, intento) + intento * hash_auxiliar2(clave)) % tamaf;
}

/**
 * @brief Calcula el número primo igual o inmediatamente superior al valor indicado.
 * @param numElementos Valor base a partir del cual buscar el siguiente número primo.
 * @return El primer número primo mayor o igual que numElementos.
 */
int ThashArbol::PrimoMayor(int numElementos) {
    while (!esPrimo(numElementos)) {
        numElementos++;
    }
    return numElementos;
}

/**
 * @brief Constructor parametrizado de ThashArbol. Dimensiona la tabla con un tamaño primo para garantizar el factor de carga deseado.
 * @param numElementos Cantidad prevista de elementos a almacenar.
 * @param lambda Factor de carga objetivo deseado (entre 0 y 1).
 */
ThashArbol::ThashArbol(int numElementos, float lambda):tamaf(PrimoMayor(numElementos/lambda)),lambda(lambda),tamal(0),
                                                       maxColisiones(0),max10(0),sumaColisiones(0),tabla(tamaf)
{
    primoMenor = calculoPrimoMenor();
}

/**
 * @brief Constructor de copia de la clase ThashArbol.
 * @param orig Objeto ThashArbol original del cual copiar los atributos y la tabla.
 */
ThashArbol::ThashArbol(const ThashArbol &orig):tamaf(orig.tamaf),tamal(orig.tamal),maxColisiones(orig.maxColisiones),
                                         max10(orig.max10),sumaColisiones(orig.sumaColisiones),primoMenor(orig.primoMenor),
                                         lambda(orig.lambda),tabla(orig.tabla) {}

/**
 * @brief Sobrecarga del operador de asignación (=) de la clase ThashArbol.
 * @param thash Objeto ThashArbol a asignar.
 * @return Referencia al objeto actual modificado.
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
 * @brief Destructor de la clase ThashArbol.
 */
ThashArbol::~ThashArbol() {}

/**
 * @brief Inserta un árbol en la tabla hash usando dispersión doble (hash2). No permite claves/árboles duplicados.
 * @param clave Clave hash generada a partir del identificador del árbol.
 * @param arbol Objeto Arbol a insertar en la tabla.
 * @return true si el árbol se ha insertado correctamente, false si ya existía o no se pudo insertar.
 */
bool ThashArbol::insertar(unsigned long clave, const Arbol &arbol) {    //FALTA COMPROBAR SI HAY QUE AUMENTAR LA TABLA!!!(Parejas)
    bool insertado = false;
    int intento = 0, pos = 0;

    while (!this->tabla.empty() && !insertado) {
        pos = hash2(clave,intento);     //Calculo una posicion aleatoria con una de las funciones hash

        if(tabla[pos].estado == OCUPADA && tabla[pos].dato->getAssetNum() == arbol.getAssetNum()) {
            return false;
        }
        if (tabla[pos].estado == LIBRE || tabla[pos].estado == BORRADO) {    //Compruebo si la posicion está libre
            tabla[pos].dato = new Arbol(arbol);                            //Si lo está, inserto el nuevo dato
            tabla[pos].clave = clave;
            tabla[pos].estado = OCUPADA;        //Esta posicion pasa de LIBRE a OCUPADA
            tamal++;                            //Aumento el tamaño logico de la tabla
            insertado = true;
        } else {
            intento++;      //Si no está libre, incrementamos el intento y volvemos a realizar el calculo de la posicion
        }

    }
    this->sumaColisiones += intento; //aumentamos las colisiones
    if(intento == 10) { //aumnetamos el max10
        this->max10++;
    }
    if(this->sumaColisiones > this->maxColisiones) { //actualizamos el maximo de colisiones en las inserciones
        this->maxColisiones = this->sumaColisiones;
    }


    return insertado;
}
/**
 * @brief Busca un árbol en la tabla hash a partir de su clave utilizando dispersión doble (hash2).
 * @param clave Clave hash del árbol a buscar.
 * @return Puntero al objeto Arbol encontrado, o nullptr si no existe en la tabla.
 */
Arbol * ThashArbol::buscar(unsigned long clave) {
    int intento = 0, pos = 0;
    bool encontrado = false;
    Arbol *dato = nullptr;


    while ( pos < tamaf  && !encontrado) {
        pos = hash2(clave, intento);

        if (tabla[pos].estado == OCUPADA && tabla[pos].clave == clave) {
            dato =  tabla[pos].dato;
            encontrado = true;
        }
        // dejamos de buscar
        if( tabla[pos].estado == LIBRE) encontrado = true;
        intento++;
    }

    return dato;
}
/**
 * @brief Realiza el borrado lógico (marcando como BORRADO) de un árbol en la tabla a partir de su clave.
 * @param clave Clave hash del árbol que se desea eliminar.
 * @return true si el elemento se encontró y se marcó como borrado, false en caso contrario.
 */
bool ThashArbol::borrar(unsigned long clave) {
    int intento = 0, pos = 0;

    while (pos < tamaf && tabla[pos].estado != LIBRE) {
        pos = hash2(clave, intento);
        if (tabla[pos].estado == OCUPADA && tabla[pos].clave == clave) {
            tabla[pos].clave = 0;
            tabla[pos].estado = BORRADO;
            tamal--;
            return true;
        }
        intento ++;
    }

    return false;
}

/**
 * @brief Devuelve de forma eficiente el número de elementos activos almacenados en la tabla.
 * @return Tamaño lógico actual de la tabla (tamal).
 */
unsigned int ThashArbol::numElementos() {
    return tamal;
}

/**
 * @brief Comprueba si un número entero es primo.
 * @param num Número entero a evaluar.
 * @return true si el número es primo, false en caso contrario.
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
 * @brief Calcula el mayor número primo estrictamente menor que el tamaño físico de la tabla (tamaf).
 * @return El primo menor encontrado, o -1 si no existe ninguno.
 */
int ThashArbol::calculoPrimoMenor() {
    for (int i = tamaf - 1; i > 0 ; i--) {
        if (esPrimo(i) == true) {
            return i;
        }
    }

    return -1;
}

void ThashArbol::redispersar(unsigned int tam) {

    tam = this->PrimoMayor(tam);

    vector<Entrada> tablaAux(tam); //creamos una tabla auxiliar para los datos

    int intento=0;
    //pasamos los datos a la nueva tabla
    for(int i =0; i < this->tabla.size(); i++) {
        intento=0;
        if(this->tabla[i].estado ==OCUPADA) { // reinsertamos las ocupadas
            unsigned int nuevoIndice= hash2(this->tabla[i].clave, intento); //utilizamos la dispersión probada

            while (tablaAux[nuevoIndice].estado ==OCUPADA) { //tratamos las colisiones
                intento++;
                nuevoIndice = hash2(tabla[i].clave, intento);
            }
            if(intento >= 10) this->maxColisiones++;
            if((unsigned long)intento > this->maxColisiones ) maxColisiones = (unsigned long) intento;

            this->sumaColisiones+= intento;
            tablaAux[nuevoIndice] = tabla[i]; //una vez encontremos una posicion que no este ocupada la asignamos
        }
    }
    this->tabla = tablaAux; //reasignamos la tabla y el tamaño que ahora posee
    this->tamaf = tam;
}

/**
 * @brief Devuelve el número máximo de colisiones registradas.
 * @return Número máximo de colisiones de tipo unsigned long.
 */
unsigned long ThashArbol::max_Colisiones() {
    return this->maxColisiones;
}

/**
 * @brief Devuelve el número de inserciones en las que se alcanzaron o superaron las 10 colisiones.
 * @return Contador de inserciones con 10 o más colisiones.
 */
unsigned long ThashArbol::numMax10() {
    return max10;
}

/**
 * @brief Calcula el promedio de colisiones por elemento insertado en la tabla.
 * @return Cociente entre el total de colisiones acumuladas y el tamaño lógico de la tabla.
 */
float ThashArbol::promedioColisiones() {
    return (float)sumaColisiones/(float)tamal;
}

/**
 * @brief Calcula el factor de carga actual de la tabla hash (tamal / tamaf).
 * @return Valor flotante del factor de carga actual.
 */
float ThashArbol::factorCarga() const {
    return (float)tamal / (float)tamaf;
}

/**
 * @brief Devuelve la capacidad física total de la tabla hash.
 * @return Tamaño físico de la tabla (tamaf).
 */
unsigned ThashArbol::tamTabla() const {
    return this->tamaf;
}

/**
 * @brief Muestra por salida estándar las métricas de rendimiento, ocupación y colisiones de la tabla hash.
 */
void ThashArbol::mostrarEstadoTabla() {
    cout << "------------ESTADO DE LA TABLA HASH------------"<<endl;
    cout << "Colisiones en la insercion mas costosa: " << max_Colisiones() << endl;
    cout << "Veces que se superan las 10 colisiones: " << numMax10() << endl;
    cout << "Promedio de colisiones por insercion: " << promedioColisiones() << endl;
    cout << "Factor de carga de la tabla: " << factorCarga() << endl;
    cout << "Tamanio de la tabla: " << tamTabla() << endl;
}
