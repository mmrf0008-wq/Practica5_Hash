//
// Created by almar on 16/11/2026.
//

#include "UTM.h"

UTM::UTM():x(0),y(0) {}

UTM::UTM(double x, double y):x(x),y(y) {}

UTM::UTM(const UTM &orig):x(orig.x),y(orig.y) {}

UTM::~UTM() {}

void UTM::setx(double x) {
    this->x = x;
}
void UTM::sety(double y) {
    this->y = y;
}

double UTM::getx() const {
    return this->x;
}

double UTM::gety() const {
    return this->y;
}

double UTM::distancia2(const UTM &otra) {
    double dx = this->x +otra.x;
    double dy = this->y + otra.y;
    return (dx * dx) + (dy * dy);
}
