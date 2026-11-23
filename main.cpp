#include <iostream>
#include "ThashArbol.h"
#include "Arbol.h"
#include "CajaNido.h"
#include "ControlParques.h"
#include "LectorCSV.h"
#include "Especie.h"
#include "Parque.h"
#include "UTM.h"

void programa1() {

    try{




    }catch (std::out_of_range &e){
        std::cerr << e.what() << std::endl;
    }catch (std::runtime_error &e){
        std::cerr << "Excepcion en fichero: " << e.what() << std::endl;
    }catch (std::bad_alloc &e){
        std::cerr << "No hay memoria suficiente para el objeto dinamico" << std::endl;
    }
}

void programa2() {

}


int main() {
    programa1();
   // programa2();
}