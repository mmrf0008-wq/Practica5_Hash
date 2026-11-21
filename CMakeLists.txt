cmake_minimum_required(VERSION 3.29)
project(Practica5_Hash)

set(CMAKE_CXX_STANDARD 20)

add_executable(Practica5_Hash main.cpp
        ThashArbol.cpp
        ThashArbol.h
        UTM.cpp
        UTM.h
        ControlParques.h
        ControlParques.cpp
        CajaNido.cpp
        Arbol.cpp
        Parque.cpp
        LectorCSV.cpp
        Especie.cpp)
