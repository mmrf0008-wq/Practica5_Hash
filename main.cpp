#include <cmath>
#include <exception>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
//Arreglar caracteres especiales por pantalla
#ifdef _WIN32
#include <windows.h>
#endif


#include "Parque.h"
#include "ControlParques.h"
#include "Arbol.h"
#include "CajaNido.h"
using namespace std;

//Rutas de los archivos
const string RUTA_ESPECIES = "./data/arbolado-especies.csv";
const string RUTA_PARQUES = "./data/parques.csv";
const string RUTA_ARBOLES = "./data/arboladoMadrid.csv";
const string RUTA_CAJAS_NIDO = "./data/cajas-nido.csv";


/**
 * @brief Muestra una posición UTM con sus decimales
 */
void mostrarCoordenadas(const UTM &coord) {
    cout << fixed << setprecision(3) << "(" << coord.getx() << ", " << coord.gety() << ")"
            << defaultfloat << setprecision(6);
}

void programaPrueba1(ControlParques control) {

    ThashArbol arboles = control.getArboles();

    cout << "ESTADO TABLA " << endl;
     arboles.mostrarEstadoTabla() ;

}

void programaPrueba2(ControlParques control ) {

    cout << "Especies cargadas " << control.getVariantes().size() << endl;
    cout << "Arboles cargadas " << control.getArboles().numElementos() << endl;
    cout << "Parques cargados " << control.getparques().size() << endl;
    cout << "Nidos cargados " << control.getTamNidos() << endl;

     cout << "\nAPARTADO 1: BÚSQUEDA DE ÁRBOLES POR ASSETNUM\n";
        const string ARBOLES_APARTADO_1[] = {"6925456", "6635839", "5222166"};
        for (const string &assetnum: ARBOLES_APARTADO_1) {
            Arbol *arbol = control.buscarArbol(assetnum);
            if (arbol == nullptr) {
                cout << "Árbol " << assetnum << ": no encontrado" << endl;
                continue;
            }
            cout << "---------" << endl;
            cout << "Árbol " << assetnum << "\n especie " << arbol->getNombreComun()
                    << " (" << arbol->getNombreCientifico() << "), \naltura " << arbol->get_altura()
                    << " m \n parque " << arbol->get_num_parque() << endl;

        }
        //asignación caja nido a arboles
        control.asginarNidosArboles();
    const string DISTRITO = "CENTRO";

        cout << "\nAPARTADO 2: ÁRBOLES CON CAJA NIDO EN EL DISTRITO CENTRO\n";

        set<Arbol*> arbolesConCaja = control.arbolesCajaDistrito(DISTRITO);
        cout << "\tÁrboles encontrados: " << arbolesConCaja.size() << endl;
        for (Arbol *arbol: arbolesConCaja) {
            cout << "\t" << arbol->getAssetNum() << " - " << arbol->getNombreComun()
                    << " - " << arbol->getNombreDistrito() << endl;
        }

     cout << "\nAPARTADO 3: CAJAS NIDO ASOCIADAS A UN ÁRBOL\n";
        unsigned int cajasEnArbol = control.contarCajasEnArbol();
        cout << "\tCajas nido asociadas a un árbol: " << cajasEnArbol << endl;
        cout << "\tCajas nido sin asociar: " << control.getTamNidos() - cajasEnArbol << endl;

        cout << "\nAPARTADO 4: CAJAS NIDO EN EL DISTRITO CENTRO\n";
        vector<CajaNido *> cajasDistrito = control.cajasEnDistrito(DISTRITO);
        cout << "\tCajas nido encontradas: " << cajasDistrito.size() << endl;
        int i =1;
        for (CajaNido *caja: cajasDistrito) {
            cout << "\tCaja " << i << ": ";
            if (caja->estaEnArbol()) {
                cout << "asociada al árbol " << caja->getArbol()->getAssetNum() << endl;
            } else {
                cout << "sin asociar" << endl;
            }
            i++;
        }

        cout << "\nAPARTADO 5: DETALLE DE CINCO CAJAS NIDO\n";
        const unsigned int CAJAS_APARTADO_5[] = {70, 75, 80, 85, 90};
        for (unsigned int linea: CAJAS_APARTADO_5) {
            CajaNido *caja = control.getNido(linea);
            if (caja == nullptr) {
                cout << "\tCaja de la línea " << linea << " no encontrada" << endl << endl;
                continue;
            }
            cout << "\t" << "caja de la línea " << linea << endl;
            cout << "\t" << "coordenadas de la caja: ";
            mostrarCoordenadas(caja->getCoordenadas());
            cout << endl;
            if (caja->estaEnArbol()) {
                Arbol *arbol = caja->getArbol();
                double distancia2 = caja->getCoordenadas().distancia2(arbol->getCoordenadas());
                cout << "\t" << "identificador del árbol: " << arbol->getAssetNum() << endl;
                cout << "\t" << "coordenadas del árbol: ";
                mostrarCoordenadas(arbol->getCoordenadas());
                cout << endl;
                cout << "\t" << "distancia: " << fixed << setprecision(2) << sqrt(distancia2) << " m"
                        << " (al cuadrado " << distancia2 << ")" << defaultfloat << setprecision(6) << endl;
                cout << "\t" << (distancia2 < 100 ? "cumple" : "NO cumple")
                        << " la condición de estar a menos de 10 metros" << endl;
            } else {
                cout << "\t" << "sin árbol asociado: no hay ninguno a menos de 10 metros" << endl;
            }
            cout << endl;
        }

        cout << "APARTADO 6: PARQUES CON ALGUNA CAJA NIDO\n";
        vector<Parque *> *parquesConCaja = control.parquesConCajaNido();
        cout << "\tParques encontrados: " << parquesConCaja->size() << endl;

        Parque *parque;
       for(int j =0; j < parquesConCaja->size(); j++){
           parque = ((*parquesConCaja)[j]);
            cout << "\t" << parque->getCodigoParque() << " - " << parque->getNombreParque() << endl;
        }

      /*  //Solo parejas: redispersión forzada a una tabla un 30 % más grande. Si no se trabaja en pareja, borrar
        cout << "\nREDISPERSIÓN DE LA TABLA\n";
        control.redispersar(static_cast<unsigned>(control.getTamTabla() * 1.3));
        control.mostrarEstadoTabla();*/
}

int main() {
    //Arreglar caracteres especiales por pantalla
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
    try {
        ControlParques control(
            RUTA_ESPECIES,
            RUTA_PARQUES,
            RUTA_ARBOLES,
            RUTA_CAJAS_NIDO, 0.68);

        cout << "\nPROGRAMA DE PRUEBA 1\n";

        //programaPrueba1(control);
        programaPrueba2(control);


    } catch (const std::exception &error) {
        std::cerr << "\nError: " << error.what() << '\n';
        return 1;
    }

    return 0;
}
