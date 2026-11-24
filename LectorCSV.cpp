#include "LectorCSV.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
#include "ControlParques.h"
#include "Especie.h"

std::size_t LectorCSV::numeroEspecies_ = 0;
std::size_t LectorCSV::filasDescartadas_ = 0;
/**
 * @brief Carga el fichero CSV indicado, mostrando cada especie por pantalla.
 *
 * Cada línea se analiza y, si tiene el número de campos esperado, se
 * construye una Especie que se almacena en el VDinamico
 * Las filas con un número de campos
 * distinto del esperado se descartan (ver filasDescartadas()).
 *
 * @param rutaFichero Ruta del fichero CSV a leer.
 * @return true si el fichero se ha podido abrir, false en caso contrario.
 */
bool LectorCSV::cargarEspecies(map<string, Especie> &especies, const std::string& rutaFichero) {
    filasDescartadas_ = 0;

    std::ifstream fichero(rutaFichero);
    if (!fichero.is_open()) {
        return false;
    }

    std::string linea;
    if (std::getline(fichero, linea)) {
        eliminarBOM(linea);
    }

    //Declaradas fuera del bucle por eficiencia
    string codigo, nombreComun, nombreCientifico, tipoPlanta;
    while (std::getline(fichero, linea)) {
        if (linea.empty()) {
            continue;
        }


        codigo = extraerCampo(linea, 0);

        if(!extraerCampo(linea, 5).empty()) {
            nombreComun = extraerCampo(linea, 1);
            nombreComun += "," + extraerCampo(linea, 2);
            nombreCientifico = extraerCampo(linea, 2);
            nombreCientifico += "," + extraerCampo(linea, 3);
            tipoPlanta = extraerCampo(linea, 4);
        }
        else if(!extraerCampo(linea, 4).empty()) {
            nombreComun = extraerCampo(linea, 1);
            nombreCientifico = extraerCampo(linea, 2);
            nombreCientifico += "," + extraerCampo(linea, 3);
            tipoPlanta = extraerCampo(linea, 4);
        }
        else {
            nombreComun = extraerCampo(linea, 1);
            nombreCientifico = extraerCampo(linea, 2);
            tipoPlanta = extraerCampo(linea, 3);
        }

        TipoPlanta p = Especie::stringtotp(tipoPlanta);

        Especie e(codigo, nombreComun, nombreCientifico,p);
        especies.insert({codigo, e}); //insertamos la espcie con us codigo

    }
    numeroEspecies_= especies.size();
    return true;
}

/**
 * @brief Número de especies leídas correctamente.
 * @return Cantidad de especies mostradas durante la última llamada a cargar().
 */
std::size_t LectorCSV::numeroEspecies() const {
    return numeroEspecies_;
}

/**
 * @brief Número de filas descartadas por no tener el número de campos esperado.
 * @return Cantidad de filas descartadas durante la última llamada a cargar().
 */
std::size_t LectorCSV::filasDescartadas() const {
    return filasDescartadas_;
}

/**
 * @brief Cuenta los campos de una línea CSV sin almacenarlos.
 *
 * Tiene en cuenta comillas envolventes y comillas escapadas ("") dentro
 * de un campo entrecomillado, tal como especifica RFC4180.
 *
 * @param linea Línea física a analizar.
 * @return Número de campos que contiene la línea.
 */
std::size_t LectorCSV::contarCampos(const std::string& linea) {
    std::size_t numCampos = 1;
    bool dentroComillas = false;
    const std::size_t n = linea.size();

    for (std::size_t i = 0; i < n; ) {
        const char c = linea[i];

        if (dentroComillas) {
            if (c == '"') {
                if (i + 1 < n && linea[i + 1] == '"') {
                    i += 2;
                } else {
                    dentroComillas = false;
                    ++i;
                }
            } else {
                ++i;
            }
        } else if (c == '"') {
            dentroComillas = true;
            ++i;
        } else if (c == ',') {
            ++numCampos;
            ++i;
        }else if (c == ';') {
            ++numCampos;
            ++i;
        } else {
            ++i;
        }
    }

    return numCampos;
}

/**
 * @brief Extrae un único campo (por índice) de una línea CSV.
 *
 * Recorre la línea completa pero solo acumula el contenido del campo
 * solicitado; el resto de campos se descarta sin llegar a almacenarse.
 * Tiene en cuenta comillas envolventes y comillas escapadas ("").
 *
 * @param linea Línea física a analizar.
 * @param indiceObjetivo Índice (base 0) del campo a extraer.
 * @return Contenido del campo solicitado.
 */
std::string LectorCSV::extraerCampo(const std::string& linea, std::size_t indiceObjetivo) {
    std::string campo;
    std::size_t indiceActual = 0;
    bool dentroComillas = false;
    const std::size_t n = linea.size();

    for (std::size_t i = 0; i < n; ) {
        const char c = linea[i];

        if (dentroComillas) {
            if (c == '"') {
                if (i + 1 < n && linea[i + 1] == '"') {
                    if (indiceActual == indiceObjetivo) {
                        campo += '"';
                    }
                    i += 2;
                } else {
                    dentroComillas = false;
                    ++i;
                }
            } else {
                if (indiceActual == indiceObjetivo) {
                    campo += c;
                }
                ++i;
            }
        } else if (c == '"') {
            dentroComillas = true;
            ++i;
        } else if (c == ',') {
            if (indiceActual == indiceObjetivo) {
                return campo;
            }
            ++indiceActual;
            ++i;
        }else if(c == ';') {
            if (indiceActual == indiceObjetivo) {
                return campo;
            }
            ++indiceActual;
            ++i;
        }
        else {
            if (indiceActual == indiceObjetivo) {
                campo += c;
            }
            ++i;
        }
    }

    return campo;
}

/**
 * @brief Elimina el BOM UTF-8 (0xEF 0xBB 0xBF) al inicio de la línea, si existe.
 * @param linea Línea a modificar in-place.
 */
void LectorCSV::eliminarBOM(std::string& linea) {
    static const char bom[3] = { static_cast<char>(0xEF), static_cast<char>(0xBB), static_cast<char>(0xBF) };
    if (linea.size() >= 3 && linea[0] == bom[0] && linea[1] == bom[1] && linea[2] == bom[2]) {
        linea.erase(0, 3);
    }
}


void LectorCSV::mostrarEspecie(const Especie& especie) {
    std::cout << " - [" << especie.get_codigo_especie() << "] " << especie.get_nombre_comun()
              << " (" << especie.get_nombre_cientifico() << ") - "
              << Especie::tptostring(especie.get_tipo_planta()) << "\n";
}


float LectorCSV::leerMedida(string campo) {
    std::replace(campo.begin(), campo.end(), ',', '.');

    return campo.empty() ? -1.0f : std::stof(campo);
}


Especie *LectorCSV::buscaEspecie(const string &cod, map<string, Especie> &especies) {

    map<string, Especie>::iterator iterator = especies.find(cod);
    if(iterator != especies.end()) {
        return &iterator->second;
    }
    return nullptr;

}

Arbol LectorCSV::leerArbol_csv(const string &linea, map<string, Especie> &especies) {
    std::stringstream registro(linea);
    std::string campos[12];

    for (unsigned int i = 0; i < 12; ++i)
    {
        if (!std::getline(registro, campos[i], ';'))
        {
            if (i != 11 || linea.empty() || linea.back() != ';')
                throw std::runtime_error("Registro de arbol incompleto");
        }
    }

    std::replace(campos[6].begin(), campos[6].end(), ',', '.');
    std::replace(campos[7].begin(), campos[7].end(), ',', '.');

    Especie *e = buscaEspecie(campos[8], especies);

    Arbol a (
         campos[0],
         std::stoi(campos[1]),
         std::stoi(campos[2]),
         campos[3],
         std::stoi(campos[4]),
         campos[5],
         UTM(std::stod(campos[6]),
         std::stod(campos[7])),
         leerMedida(campos[10]),
         leerMedida(campos[11]),
         new Especie(*e),
         nullptr);                                          //El arbol se carga luego en cargosArbol

    return  a;
}

/**
 * @brief Carga los parques desde un fichero CSV (código y nombre) y los añade a la lista.
 *
 * Se salta la cabecera (eliminando el BOM si existe) y se descartan las filas con menos de 2 campos.
 * @param parques Lista donde se almacenan los parques leídos.
 * @param rutaFichero Ruta del fichero CSV de parques.
 * @return true si el fichero se ha podido abrir, false en caso contrario.
 */
bool  LectorCSV::cargarParques(map<int, Parque> &parques, const std::string &rutaFichero) {
    filasDescartadas_ = 0;

    std::ifstream fichero(rutaFichero);
    if (!fichero.is_open()) {
        return false;
    }

    std::string linea;
    if (std::getline(fichero, linea)) {
        eliminarBOM(linea);
    }

    //Declaradas fuera del bucle por eficiencia

    while (std::getline(fichero, linea)) {
        if (linea.empty()) {
            continue;
        }
        if (contarCampos(linea) < 2) {
            ++filasDescartadas_;
            continue;
        }

        int codigo_parque;              //Id de parque
        string nombre_parque;

        codigo_parque = stoi(extraerCampo(linea, 0));
        nombre_parque = extraerCampo(linea, 1);
        parques.insert({codigo_parque, Parque(codigo_parque,nombre_parque)});

    }
    numeroEspecies_= parques.size();
    return true;
}

/**
 * @brief Carga los árboles desde un fichero CSV. Cada árbol se inserta en el mapa por assetnum y en el multimap por nombre común, y se enlaza con su parque.
 * @param arboles Mapa de árboles (clave: assetnum) donde se almacenan los árboles leídos.
 * @param rutaFichero Ruta del fichero CSV de árboles.
 * @param parques Lista de parques con los que se enlazan los árboles.
 * @param especies Vector de especies para asignar la especie de cada árbol.
 * @param treeName Multimap (clave: nombre común) con punteros a los árboles del mapa.
 * @return true si la carga se ha completado.
 * @throw std::runtime_error Si no se puede abrir el fichero.
 */
bool  LectorCSV::cargosArbol(ThashArbol & arboles, const std::string &rutaFichero, map<int, Parque> &parques, map<string, Especie> &especies, multimap<string,Arbol*> &treeName){

    std::ifstream fichero(rutaFichero);

    if (!fichero.is_open())
        throw std::runtime_error("No se puede abrir: " + rutaFichero);

    std::string linea;
    std::getline(fichero, linea); // Saltar cabecera.

    while (std::getline(fichero, linea))
    {
        if (!linea.empty() && linea.back() == '\r')
            linea.pop_back();

        if (linea.empty())
            continue;

        Arbol arbol = leerArbol_csv(linea, especies);

        long a = ControlParques::djb2(arbol.getAssetNum().c_str());
       arboles.insertar(a, arbol);
        treeName.insert({arbol.getNombreComun(), new Arbol(arbol)});

        bool ligado = false;
        for(map<int, Parque>::iterator i = parques.begin(); i != parques.end() && !ligado; i++) { //ligamos el arbol con un parque

            if(i->second.getCodigoParque() == arbol.get_num_parque()) {

                i->second.insertarArbol(&(arbol));
                arbol.set_is_into(&i->second);
                ligado = true;
            }
        }

    }

    return true;
}

bool LectorCSV::cargarNido(const string &ficheroNido, vector<CajaNido> nidos) {
    filasDescartadas_ = 0;

    std::ifstream fichero(ficheroNido);
    if (!fichero.is_open()) {
        return false;
    }

    std::string linea;
    if (std::getline(fichero, linea)) {
        eliminarBOM(linea);
    }

    //Declaradas fuera del bucle por eficiencia

    while (std::getline(fichero, linea)) {
        if (linea.empty()) {
            continue;
        }
        if (contarCampos(linea) < 2) {
            ++filasDescartadas_;
            continue;
        }

        string distrito;
        string barrio;
        string ubicacion;
        UTM coordenadas;


        distrito = extraerCampo(linea, 0);
        barrio = extraerCampo(linea,1);
        ubicacion = extraerCampo(linea, 2);
        coordenadas = UTM(stof(extraerCampo(linea,3)),stof(extraerCampo(linea,4))) ;

        CajaNido nido(distrito, barrio, ubicacion, coordenadas, nullptr);

        nidos.push_back(nido);
    }

    return true;

}
