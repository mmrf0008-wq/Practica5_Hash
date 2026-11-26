//
// Created by almar on 16/11/2026.
//

#include "UTM.h"
/**
 * @brief Constructor por defecto de UTM. Inicializa las coordenadas (x, y) a 0.
 */
UTM::UTM():x(0),y(0) {}

/**
 * @brief Constructor parametrizado de UTM.
 * @param x Coordenada horizontal (Este) en metros.
 * @param y Coordenada vertical (Norte) en metros.
 */
UTM::UTM(double x, double y):x(x),y(y) {}

/**
 * @brief Constructor de copia de UTM.
 * @param orig Objeto UTM original del cual copiar las coordenadas.
 */
UTM::UTM(const UTM &orig):x(orig.x),y(orig.y) {}

/**
 * @brief Destructor de la clase UTM.
 */
UTM::~UTM() {}


/**
 * @brief Establece el valor de la coordenada X.
 * @param x Nuevo valor para la coordenada X.
 */
void UTM::setx(double x) {
    this->x = x;
}

/**
 * @brief Establece el valor de la coordenada Y.
 * @param y Nuevo valor para la coordenada Y.
 */
void UTM::sety(double y) {
    this->y = y;
}

/**
 * @brief Devuelve el valor de la coordenada X.
 * @return Valor de tipo double correspondiente a la coordenada X.
 */
double UTM::getx() const {
    return this->x;
}

/**
 * @brief Devuelve el valor de la coordenada Y.
 * @return Valor de tipo double correspondiente a la coordenada Y.
 */
double UTM::gety() const {
    return this->y;
}

/**
 * @brief Calcula la distancia euclídea al cuadrado entre esta coordenada y otra dada.
 * @param otra Coordenada UTM destino respecto a la cual calcular la distancia.
 * @return Distancia al cuadrado (dx² + dy²) entre ambas coordenadas.
 */
double UTM::distancia2(const UTM &otra) {
    double dx = this->x -otra.x;
    double dy = this->y - otra.y;
    return (dx * dx) + (dy * dy);
}
